#include "s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  unsigned char *ptr = (unsigned char *)str;
  for (s21_size_t i = 0; i < n && *ptr != c; i++) ptr++;
  if (ptr == (unsigned char *)str + n) ptr = S21_NULL;
  return ptr;
}
