#include "9cc.h"

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
    if (memcmp("==", p, 2) == 0 || memcmp("!=", p, 2) == 0 ||
        memcmp(">=", p, 2) == 0 || memcmp("<=", p, 2) == 0) {
      cur = new_token(TK_RESERVED, cur, p);
      p += 2;
      cur->len = 2;
      continue;
    }
    if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' ||
        *p == ')' || *p == '>' || *p == '<' || *p == ';' || *p == '=') {
      cur = new_token(TK_RESERVED, cur, p++);
      cur->len = 1;
      continue;
    }
    if (strncmp(p, "return", 6) == 0 && !is_alnum(p[6])) {
      cur = new_token(TK_RET, cur, p);
      cur->str = p;
      cur->len = 6;
      p += 6;
      continue;
    }
    if (strncmp(p, "if", 2) == 0 && !is_alnum(p[2])) {
      cur = new_token(TK_IF, cur, p);
      cur->str = p;
      cur->len = 2;
      p += 2;
      continue;
    }
    if (strncmp(p, "else", 4) == 0 && !is_alnum(p[4])) {
      cur = new_token(TK_ELSE, cur, p);
      cur->str = p;
      cur->len = 4;
      p += 4;
      continue;
    }
    if (strncmp(p, "while", 5) == 0 && !is_alnum(p[5])) {
      cur = new_token(TK_WHILE, cur, p);
      cur->str = p;
      cur->len = 5;
      p += 5;
      continue;
    }
    if (is_alphabet(p)) {
      int len = 0;
      while (is_alphabet(p + len)) {
        len++;
      }
      cur = new_token(TK_IDENT, cur, p);
      cur->len = len;
      p += len;
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
