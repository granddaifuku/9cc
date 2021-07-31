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

typedef struct Token Token;

struct Token {
  TokenKind kind; // The type of token
  Token *next; // The next token
  int val; // The value of token if it is number
  char *str; // Token string
};

// Current token
Token *token;

// The function to report error
// Take same arguments as "printf" e.g., printf("%d, %d", 1, 2);
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
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
	error("The next token is not '%c'", op);
  token = token->next;
}

// Read the token and return its value if current token is number.
// Otherwise report the error
int expect_number() {
  if (token->kind != TK_NUM)
	error("Not a number");
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
Token *tokenize(char *p) {
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while (*p) {
	// Skip white space
	if (isspace(*p)) {
	  p++;
	  continue;
	}
	if (*p == '+' || *p == '-') {
	  cur = new_token(TK_RESERVED, cur, p++);
	  continue;
	}

	if (isdigit(*p)) {
	  cur = new_token(TK_NUM, cur, p);
	  cur->val = strtol(p, &p, 10);
	  continue;
	}

	error("Could not tokenize");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}

int main(int argc, char **argv) {
  if (argc != 2) {
	fprintf(stderr, "Invalid number of args\n");
	return 1;
  }

  // Tokenize
  token = tokenize(argv[1]);

  // Export the first part of Assembly
  printf(".intel_syntax noprefix\n");
  printf(".globl main\n");
  printf("main:\n");

  // The first part of the expression should be a number.
  // Check it and call mov.
  printf("  mov rax, %d\n", expect_number());

  while (!at_eof()) {
	if (consume('+')) {
	  printf("  add rax, %d\n", expect_number());
	  continue;
	}

	expect('-');
	printf("  sub rax, %d\n", expect_number());
  }
  
  printf("  ret\n");
  return 0;
}
