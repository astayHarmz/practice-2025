#include "s21_string.h"

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  s21_size_t srcstr_count = 0;
  s21_size_t deststr_count = s21_strlen(dest);
  while (*src != '\0' && srcstr_count < n) {
    dest[deststr_count + srcstr_count] = *src;
    srcstr_count++;
    src++;
  }
  dest[deststr_count + srcstr_count] = '\0';
  return dest;
}