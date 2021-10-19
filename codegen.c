#include "9cc.h"

void gen(Node *node) {
  if (node->kind == ND_NUM) {
    printf("  push %d\n", node->val);
    return;
  }
  if (node->kind == ND_LVA) {
    gen_lval(node);
    printf("  pop rax\n");
    printf("  mov rax, [rax]\n"); // Consider the value of rax as an address and
    // load the value into rax
    printf("  push rax\n");
    return;
  }
  if (node->kind == ND_ASS) {
    gen_lval(node->lhs);
    gen(node->rhs);

    printf("  pop rdi\n");
    printf("  pop rax\n");
    printf("  mov [rax], rdi\n"); // Store the value of rdi into the address
    // where rax indicates
    printf("  push rdi\n");
    return;
  }
  if (node->kind == ND_RET) {
    gen(node->lhs); // the return value
    printf("  pop rax\n");
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return;
  }
  if (node->kind == ND_IF) {
    // "if" "(" cond ")" then
    int label_end = label_num();
    gen(node->cond);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je .Lend%d\n", label_end);
    gen(node->then);
    printf("  .Lend%d:\n", label_end);
    return;
  }
  if (node->kind == ND_IF_ELSE) {
    // "if" "(" cond ")" then els
    int label_else = label_num();
    int label_end = label_num();
    gen(node->cond);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je .Lelse%d\n", label_else);
    gen(node->then);
    printf("  jmp .Lend%d\n", label_end);
    printf("  .Lelse%d:\n", label_else);
    gen(node->els);
    printf("  .Lend%d:\n", label_end);
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
