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
  TK_IDENT, // identifier
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

extern void error(char *fmt, ...);
extern void error_at(char *loc, char *fmt, ...);

extern bool consume(char *op);
extern Token *consume_ident();
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
  ND_ASS, // =
  ND_LVA, // local variable
  ND_NUM, // Number
} NodeKind;

typedef struct Node Node;

struct Node {
  NodeKind kind; // The type of node
  Node *lhs; // Left-hand side
  Node *rhs; // Right-hand side
  int val; // The value of node if it is number
  int offset; // The offset from the base pointer to the local variable
};

extern Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
extern Node *new_node_num(int val);
extern void program();
extern Node *stmt();
extern Node *expr();
extern Node *assign();
extern Node *equality();
extern Node *relational();
extern Node *add();
extern Node *mul();
extern Node *unary();
extern Node *primary();
extern void gen(Node *node);
extern void gen_lval(Node *node);

extern Node *code[100];
