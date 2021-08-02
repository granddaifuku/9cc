#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Type of tokens
  typedef enum {
  TK_RESERVED, // symbols
  TK_NUM, // numbers
  TK_EOF, // EOF
} TokenKind;

// Type of nodes in abstract syntax tree
typedef enum {
  ND_ADD, // +
  ND_SUB, // -
  ND_MUL, // *
  ND_DIV, // /
  ND_NUM, // Number
} NodeKind;

typedef struct Token Token;
typedef struct Node Node;

Node *expr();
Node *mul();
Node *primary();

struct Token {
  TokenKind kind; // The type of token
  Token *next; // The next token
  int val; // The value of token if it is number
  char *str; // Token string
};

struct Node {
  NodeKind kind; // The type of node
  Node *lhs; // Left-hand side
  Node *rhs; // Right-hand side
  int val; // The value of node if it is number
};

// Current token
Token *token;

char *user_input;

// The function to report error
// Take same arguments as "printf" e.g., printf("%d, %d", 1, 2);
// va_list is variable length arguments
void error_at(char *loc, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  int pos = loc - user_input;
  fprintf(stderr, "%s\n", user_input);
  fprintf(stderr, "%*s", pos, " "); // Output pos number of spaces
  fprintf(stderr, "^ ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// Return true and read the next pointer if current token is the expected symbol
// Otherwise return false
bool consume(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op)
	return false;
  token = token->next;
  return true;
}

// Read the next pointer if current token is the expected symbol
// Otherwise report the error
void expect(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op)
	error_at(token->str, "The next token is not '%c'", op);
  token = token->next;
}

// Read the token and return its value if current token is number.
// Otherwise report the error
int expect_number() {
  if (token->kind != TK_NUM)
	error_at(token->str, "Not a number");
  int val = token->val;
  token = token->next;
  return val;
}

bool at_eof() {
  return token->kind == TK_EOF;
}

// Creat a new token and set it as a next token
Token *new_token(TokenKind kind, Token *cur, char *str) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  cur->next = tok;
  return tok;
}

// Tokenize the input strint
Token *tokenize() {
  char *p = user_input;
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while (*p) {
	// Skip white space
	if (isspace(*p)) {
	  p++;
	  continue;
	}
	if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')') {
	  cur = new_token(TK_RESERVED, cur, p++);
	  continue;
	}

	if (isdigit(*p)) {
	  cur = new_token(TK_NUM, cur, p);
	  cur->val = strtol(p, &p, 10);
	  continue;
	}

	error_at(token->str, "Could not tokenize");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}


Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_num(int val) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_NUM;
  node->val = val;
  return node;
}

Node *expr() {
  Node *node = mul();

  for (;;) {
	if (consume('+'))
	  node = new_node(ND_ADD, node, mul());
	else if (consume('-'))
	  node = new_node(ND_SUB, node, mul());
	else
	  return node;
  }
}

Node *mul() {
  Node *node = primary();

  for (;;) {
	if (consume('*'))
	  node = new_node(ND_MUL, node, primary());
	else if (consume('/'))
	  node = new_node(ND_DIV, node, primary());
	else
	  return node;
  }
}

Node *primary() {
  if (consume('(')) {
	Node *node = expr();
	expect(')');
	return node;
  }

  return new_node_num(expect_number());
}

void gen(Node *node) {
  if (node->kind == ND_NUM) {
	printf("  push %d\n", node->val);
	return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch (node->kind) {
  case ND_ADD:
	printf("  add rax, rdi\n");
	break;
  case ND_SUB:
	printf("  sub rax, rdi\n");
	break;
  case ND_MUL:
	printf("  imul rax, rdi\n");
	break;
  case ND_DIV:
	printf("  cqo\n");
	printf("  idiv rdi\n");
	break;
  }

  printf("  push rax\n");
}


int main(int argc, char **argv) {
  if (argc != 2) {
	fprintf(stderr, "Invalid number of args\n");
	return 1;
  }
  
  user_input = argv[1];
  
  // Tokenize
  token = tokenize();
  // Parse
  Node *node = expr();

  // Export the first part of Assembly
  printf(".intel_syntax noprefix\n");
  printf(".globl main\n");
  printf("main:\n");

  // Generate code with recurisvely visiting the abstract syntax tree
  gen(node);

  // The value of the entire expression remains the stack top
  printf("  pop rax\n");
  printf("  ret\n");
  return 0;
}
