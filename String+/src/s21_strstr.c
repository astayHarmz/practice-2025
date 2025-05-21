#include "s21_string.h"

char *s21_strstr(const char *haystack, const char *needle) {
  int hscounter = 0;
  int ndlcounter = 0;
  while (haystack[hscounter] != '\0' && needle[ndlcounter] != '\0') {
    if (haystack[hscounter + ndlcounter] == needle[ndlcounter]) {
      ndlcounter++;
    } else {
      hscounter++;
      ndlcounter = 0;
    }
  }
  return needle[ndlcounter] == '\0' ? (char *)haystack + hscounter : S21_NULL;
}