#include "s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  int res = 0;
  unsigned char *buf1 = (unsigned char *)str1;
  unsigned char *buf2 = (unsigned char *)str2;
  for (s21_size_t i = 0; i < n && res == 0; i++) res = buf1[i] - buf2[i];
  return res;
}