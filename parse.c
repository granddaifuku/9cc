#include "9cc.h"

// Return true and read the next pointer if current token is the expected symbol
// Otherwise return false
bool consume(char *op) {
  if (token->kind != TK_RESERVED || strlen(op) != token->len || memcmp(token->str, op, token->len))
	return false;
  token = token->next;
  return true;
}

Token *consume_ident() {
  if (token->kind != TK_IDENT)
	return 0;
  Token *tok = token;
  token = token->next;
  return tok;
}

// Read the next pointer if current token is the expected symbol
// Otherwise report the error
void expect(char *op) {
  if (token->kind != TK_RESERVED || strlen(op) != token->len || memcmp(token->str, op, token->len))
	error_at(token->str, "The next token is not '%s'", op);
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

// program = stmt*
void program() {
  int i = 0;
  while (!at_eof()) 
	code[i++] = stmt();

  code[i] = NULL;
}

// stmt = expr ";"
Node *stmt() {
  Node *node = expr();
  expect(";");
  return node;
}

// expr = assign
Node *expr() {
  return assign();
}

// assign = equality ("=" assign)?
Node *assign() {
  Node *node = equality();
  if (consume("="))
	node = new_node(ND_ASS, node, assign());
  return node;
}

// equality = relational ("==" relational | "!=" relational)*
Node *equality() {
  Node *node = relational();

  for (;;) {
	if (consume("=="))
	  node = new_node(ND_EQT, node, relational());
	else if (consume("!="))
	  node = new_node(ND_NEQ, node, relational());
	else
	  return node;
  }
}

// relational = add ("<" add | "<=" add | ">" add | ">=" add)*
Node *relational() {
  Node *node = add();

  for (;;) {
	if (consume("<="))
	  node = new_node(ND_LEQ, node, add());
	else if (consume("<"))
	  node = new_node(ND_LES, node, add());
	else if (consume(">="))
	  node = new_node(ND_LEQ, add(), node);
	else if (consume(">"))
	  node = new_node(ND_LES, add(), node);
	else
	  return node;
  }
}

// add = mul ("+" mul | "-" mul)*
Node *add() {
  Node *node = mul();

  for (;;) {
	if (consume("+"))
	  node = new_node(ND_ADD, node, mul());
	else if (consume("-"))
	  node = new_node(ND_SUB, node, mul());
	else
	  return node;
  }
}

// mul = unary ("*" unary | "/" unary)*
Node *mul() {
  Node *node = unary();

  for (;;) {
	if (consume("*"))
	  node = new_node(ND_MUL, node, unary());
	else if (consume("/"))
	  node = new_node(ND_DIV, node, unary());
	else
	  return node;
  }
}

// unary = ("+" | "-")? primary
Node *unary() {
  if (consume("+")) 
	return primary();
  
  if (consume("-")) 
	return new_node(ND_SUB, new_node_num(0), primary());
  
  return primary();
}

// primary = num | ident | "(" expr ")"
Node *primary() {
  if (consume("(")) {
	Node *node = expr();
	expect(")");
	return node;
  }
  Token *tok = consume_ident();
  if (tok) {
	Node *node = calloc(1, sizeof(Node));
	node->kind = ND_LVA;
	node->offset = (tok->str[0] - 'a' + 1) * 8;
	return node;
  }

  return new_node_num(expect_number());
}
