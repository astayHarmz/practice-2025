#include "s21_string.h"

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  for (; (*str1 != '\0' && *str2 != '\0') && (*str1 == *str2) && n != 0; n--) {
    str1++;
    str2++;
  }
  return n == 0 ? 0 : *(unsigned char *)str1 - *(unsigned char *)str2;
}
