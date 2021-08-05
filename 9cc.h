#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char *user_input;

//
// Tokenizer
//

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
  int len; // The length of token
};

extern void error_at(char *loc, char *fmt, ...);
extern bool consume(char *op);
extern void expect(char *op);
extern int expect_number();
extern bool at_eof();

extern Token *new_token(TokenKind kind, Token *cur, char *str);
extern Token *tokenize();

extern Token *token;

//
// Parser
//

// Type of nodes in abstract syntax tree
typedef enum {
  ND_ADD, // +
  ND_SUB, // -
  ND_MUL, // *
  ND_DIV, // /
  ND_EQT, // ==
  ND_NEQ, // !=
  ND_LEQ, // <=
  ND_LES, // <
  ND_NUM, // Number
} NodeKind;

typedef struct Node Node;

struct Node {
  NodeKind kind; // The type of node
  Node *lhs; // Left-hand side
  Node *rhs; // Right-hand side
  int val; // The value of node if it is number
};

Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);
Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();
void gen(Node *node);

