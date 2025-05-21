#include "s21_string.h"

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  s21_size_t strcount = 0;
  while (*src != '\0' && strcount < n) {
    dest[strcount] = *src;
    src++;
    strcount++;
  }
  dest[strcount] = 0;
  return dest;
}