#include "9cc.h"

// Current token
Token *token;

char *user_input;

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
