#include "s21_string.h"

char *s21_strrchr(const char *str, int c) {
  char *curr = (char *)str;
  char *res = S21_NULL;
  while (*curr != 0) {
    if (*curr == c) res = curr;
    curr++;
  }
  if (c == 0) res = curr;
  return res;
}
