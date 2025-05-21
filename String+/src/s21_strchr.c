#include "s21_string.h"

char *s21_strchr(const char *str, int c) {
  char *pointer = S21_NULL;

  while (*str && *str != c) {
    str++;
  }
  if (*str == c) {
    pointer = (char *)str;
  }
  return pointer;
}