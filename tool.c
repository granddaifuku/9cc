#include "9cc.h"
#include <stdlib.h>

// The function to report error
// Take same arguments as "printf" e.g., printf("%d, %d", 1, 2);
// va_list is variable length arguments
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// The funcion that extends above to show where the error occurs.
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

int count_offset() {
  int i = 0;
  for (LVar *var = locals; var; var = var->next) {
    i++;
  }

  return i * 8;
}

bool is_alphabet(char *p) { return 'a' <= *p && *p <= 'z'; }

int is_alnum(char c) {
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') ||
         ('0' <= c && c <= '9') || (c == '_');
}

int label_num() { return label_number++; }

Vector *new_vector() {
  Vector *vec = malloc(sizeof(Vector));
  int size = 16;

  vec->data = malloc(sizeof(void *) * size);
  vec->capacity = size;
  vec->len = 0;

  return vec;
}

void vec_push(Vector *v, void *e) {
  if (v->len == v->capacity) {
    v->capacity *= 2;
    v->data = realloc(v->data, sizeof(void *) * v->capacity);
  }
  v->data[v->len++] = e;
}
