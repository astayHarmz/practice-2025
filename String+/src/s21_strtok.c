#include "s21_string.h"

char* s21_strtok(char* str, const char* delim) {
  static char* next;
  char* start = S21_NULL;

  if (str) {
    next = str;
    while (*next && s21_strchr(delim, *next)) next++;
  }
  if (*next) {
    while (*next && s21_strchr(delim, *next)) next++;
    start = next;
    while (*next && !s21_strchr(delim, *next)) next++;
    if (*next) {
      char* ptr = next;
      while (*next && s21_strchr(delim, *next)) next++;
      *ptr = '\0';
    }
  }
  return start;
}