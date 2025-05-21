#include "s21_string.h"

bool s21_is_digit(int c) { return c >= '0' && c <= '9'; }

bool s21_is_space(int c) {
  return c == 0x20 || c == 0x09 || c == 0x0a || c == 0x0b || c == 0x0c ||
         c == 0x0d;
}

long long s21_atou(char *str, int n, state *s_state) {
  long long res = 0;
  int i = 0;
  if (!str || !(*str) || !s21_is_digit(*str)) {
    s_state->status = ERROR;
  } else {
    while (*str && s21_is_digit(*str) && n--) {
      res = res * 10 + *str++ - 48;
      i++;
    }
    s_state->count++;
  }
  s_state->shift += i;
  return res;
}

int is_hexidecimal(char c) {
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
         (c >= 'A' && c <= 'F');
}

int is_octodecimal(char c) { return (c >= '0' && c <= '7'); }

int hex_to_dec(char c) {
  int res = 0;
  if (c >= '0' && c <= '9')
    res = c - 48;
  else if (c >= 'a' && c <= 'f')
    res = c - 87;
  else if (c >= 'A' && c <= 'F')
    res = c - 55;
  return res;
}

long s21_atox(char *str, int n, state *s_state) {
  long res = 0;
  if (!str || !(*str) || !is_hexidecimal(*(str)))
    s_state->status = ERROR;
  else {
    if (*str == '0' && (*(str + 1) == 'x' || *(str + 1) == 'X') &&
        is_hexidecimal(*(str + 2))) {
      str += 2;
      n -= 2;
      s_state->shift += 2;
    }
    int i = 0;
    char buf[1024] = "";
    while (*str && is_hexidecimal(*str) && n--) {
      buf[i] = *str++;
      i++;
    }
    for (int j = 0; j < i; j++) {
      res += hex_to_dec(buf[j]) * pow(16, i - j - 1);
    }
    s_state->count++;
    s_state->shift += i;
  }
  return res;
}

long s21_atoo(char *str, int n, state *s_state) {
  long res = 0;
  if (!str || !(*str) || !is_octodecimal(*(str)))
    s_state->status = ERROR;
  else {
    if (*str == '0' && is_octodecimal(*(str + 1))) {
      str++;
      s_state->shift++;
      n -= 1;
    }
    int i = 0;
    char buf[1024] = "";
    while (*str && is_octodecimal(*str) && n--) {
      buf[i] = *str++;
      i++;
    }
    for (int j = 0; j < i; j++) {
      res += (buf[j] - 48) * pow(8, i - j - 1);
    }
    s_state->count++;
    s_state->shift += i;
  }
  return res;
}

long double s21_atof(char *str, int n, state *s_state) {
  long double res = 0;
  if (!str || !(*str) || !n || (!s21_is_digit(*(str)) && *str != '.')) {
    char *tmp = s21_to_lower(str);
    if (!s21_strncmp(tmp, "nan", 3)) {
      s_state->count++;
      s_state->shift += 3;
      res = NAN;
    } else if (!s21_strncmp(tmp, "inf", 3)) {
      s_state->count++;
      s_state->shift += 3;
      res = POS_INF;
    } else
      s_state->status = ERROR;
    free(tmp);
  } else {
    char *ptr = str;
    while (n && s21_is_digit(*str)) {
      res = res * 10 + *str++ - 48;
      n--;
    }
    if (n && *str == '.') {
      str++;
      n--;
      int i = 0;
      long double decimal = 0;
      while (n && s21_is_digit(*str)) {
        decimal = decimal * 10 + *str++ - 48;
        i++;
        n--;
      }
      res += decimal * powl(10, -i);
      if (n && (*str == 'e' || *str == 'E')) {
        str++;
        n--;
      }
      i = 0;
      int exp = 0;
      int exp_sign = 0;
      if (n && (*str == '-' || *str == '+')) {
        if (*str == '-') exp_sign = 1;
        str++;
        n--;
      }
      while (s21_is_digit(*str) && n-- && i < 5) {
        exp = exp * 10 + *str++ - 48;
        i++;
      }
      exp = (exp > 4932 ? 4932 : exp) * (exp_sign ? -1 : 1);
      res *= pow(10, exp);
    }
    s_state->count++;
    s_state->shift = str - ptr;
  }
  return res;
}