#include "s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  char *pointer = S21_NULL;
  int found = 0;

  while (*str1 && !found) {
    const char *s = str2;
    while (*s && !found) {
      if (*str1 == *s) {
        pointer = (char *)str1;
        found = 1;
      }
      s++;
    }
    if (!found) {
      str1++;
    }
  }

  return pointer;
}
