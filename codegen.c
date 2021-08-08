#include "9cc.h"

void gen(Node *node) {
  switch (node->kind) {
  case ND_NUM:
	printf("  push %d\n", node->val);
	return;
  case ND_LVA:
	gen_lval(node);
	printf("  pop rax\n");
	printf("  mov rax, [rax]\n"); // Consider the value of rax as an address and load the value into rax
	printf("  push rax\n");
	return;
  case ND_ASS:
	gen_lval(node->lhs);
	gen(node->rhs);

	printf("  pop rdi\n");
	printf("  pop rax\n");
	printf("  mov [rax], rdi\n"); // Store the value of rdi into the address where rax indicates
	printf("  push rdi\n");
	return;
  case ND_RET:
	gen(node->lhs); // the return value
	printf("  pop rax\n");
	printf("  mov rsp, rbp\n");
	printf("  pop rbp\n");
	printf("  ret\n");
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
  case ND_EQT:
	printf("  cmp rax, rdi\n");
	printf("  sete al\n");
	printf("  movzb rax, al\n");
	break;
  case ND_NEQ:
	printf("  cmp rax, rdi\n");
	printf("  setne al\n");
	printf("  movzb rax, al\n");
	break;
  case ND_LEQ:
	printf("  cmp rax, rdi\n");
	printf("  setle al\n");
	printf("  movzb rax, al\n");
	break;
  case ND_LES:
	printf("  cmp rax, rdi\n");
	printf("  setl al\n");
	printf("  movzb rax, al\n");
	break;
  }

  printf("  push rax\n");
}

// push the variable address
void gen_lval(Node *node) {
  if (node->kind != ND_LVA)
	error("Invalid left value");
  printf("  mov rax, rbp\n");
  printf("  sub rax, %d\n", node->offset);
  printf("  push rax\n");
}
