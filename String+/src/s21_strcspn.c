#include "s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t count = 0;
  int found = 0;

  while (*str1 && !found) {
    const char *s = str2;
    found = 0;
    while (*s && !found) {
      if (*str1 == *s) {
        found = 1;
      }
      s++;
    }
    if (!found) {
      count++;
      str1++;
    }
  }

  return count;
}