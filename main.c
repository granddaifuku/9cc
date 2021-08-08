#include "9cc.h"

// Current token
Token *token;

char *user_input;

Node *code[100];

LVar *locals;

int main(int argc, char **argv) {
  if (argc != 2) {
	error("Invalid number of args");
	return 1;
  }

  user_input = argv[1];
  // Tokenize
  token = tokenize();
  program();

  // Export the first part of Assembly
  printf(".intel_syntax noprefix\n");
  printf(".globl main\n");
  printf("main:\n");

  // Prologue
  // Reserve the memory for 26 variables
  printf("  push rbp\n");
  printf("  mov rbp, rsp\n");
  printf("  sub rsp, %d\n", count_offset());

  // Generate code with recurisvely visiting the abstract syntax tree
  for (int i = 0; code[i]; i++) {
	gen(code[i]);

	//   // The value of the entire expression remains the stack top
	printf("  pop rax\n");
  }

  // Epilogue
  printf("  mov rsp, rbp\n"); // Let rsp shows the return address
  printf("  pop rbp\n");
  printf("  ret\n");
  return 0;
}
