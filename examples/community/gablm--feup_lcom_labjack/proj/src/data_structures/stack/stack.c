#include "stack.h"

my_stack_t *stack_create(size_t size) {
  my_stack_t *stack;

  stack = malloc(sizeof(my_stack_t));
  if (stack == NULL)
    return NULL;

  stack->content = malloc(size * sizeof(void *));
  if (stack->content == NULL) {
    free(stack);
    return NULL;
  }

  stack->base_size = size;
  stack->curr_size = 0;
  stack->end_pos = 0;

  return stack;
}

size_t stack_size(my_stack_t *stack) {
  if (stack == NULL)
    return 0;

  return stack->base_size;
}

int stack_full(my_stack_t *stack) {
  if (stack == NULL)
    return 0;

  return stack->base_size == stack->curr_size;
}

int stack_empty(my_stack_t *stack) {
  if (stack == NULL)
    return 0;

  return stack->curr_size == 0;
}

void stack_destroy(my_stack_t **stack, void (*f)(void *)) {
  if (stack == NULL || *stack == NULL || f == NULL)
    return;

  for (size_t i = 0; i <= (*stack)->curr_size; i++)
    f((*stack)->content[i]);

  free(*stack);
  *stack = NULL;
}

void *stack_pop(my_stack_t *stack) {
  if (stack_empty(stack))
    return NULL;

  void *result = stack->content[stack->end_pos];

  stack->curr_size -= 1;
  stack->end_pos--;

  return result;
}

int stack_push(my_stack_t *stack, void *content) {
  if (stack_full(stack) || content == NULL)
    return 1;

  stack->curr_size += 1;
  stack->end_pos++;
  stack->content[stack->end_pos] = content;
  return 0;
}

void *stack_at(my_stack_t *stack, size_t pos) {
  if (pos >= stack->curr_size)
    return NULL;

  size_t f_pos = stack->curr_size - pos;
  return stack->content[f_pos];
}

void stack_destroy_nothing(void *ptr) {
  (void) ptr;
}
