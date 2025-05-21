#include "s21_string.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  unsigned char *dest_a = (unsigned char *)dest;
  const unsigned char *src_a = (unsigned char *)src;

  for (s21_size_t i = 0; i < n; i++) dest_a[i] = src_a[i];

  return dest;
}
