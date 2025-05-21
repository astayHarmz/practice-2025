#include "s21_string.h"

int s21_sscanf(const char *str, const char *format, ...) {
  state s_state = {0};
  va_list ptr_args;
  va_start(ptr_args, format);
  scan_format((char *)str, (char *)format, ptr_args, &s_state);
  va_end(ptr_args);
  return s_state.count;
}

void refresh_options(state *s_state) {
  s_state->is_ignored = 0;
  s_state->length = 0;
  s_state->spec = 0;
  s_state->width = 0;
  s_state->shift = 0;
}

void scan_format(char *str, char *format, va_list ptr_args, state *s_state) {
  s_state->ptr = str;
  while (*format && !s_state->status) {
    if (s21_is_space(*format)) {
      format++;
      while (s21_is_space(*str)) str++;
    } else if (*format == '%') {
      if (*str || *(format + 1) == 'n') {
        format++;
        refresh_options(s_state);
        parse_options(&format, s_state);
        scan_arg(str, ptr_args, s_state);
        if (s_state->status != ERROR) str += s_state->shift;
      } else {
        s_state->status = ERROR;
        if (!s_state->count) s_state->count = -1;
      }
    } else {
      if (*str++ != *format++) s_state->status = ERROR;
    }
  }
}

void parse_options(char **format, state *s_state) {
  for (; **format && !s21_strchr(SCAN_SPECS, **format); (*format)++) {
    if (**format == '*') {
      s_state->is_ignored = 1;
    } else if (s21_is_digit(**format)) {
      s_state->width = s_state->width * 10 + **format - 48;
    } else if (s21_strchr("hlL", **format)) {
      s_state->length = **format;
    }
  }
  s_state->spec = *(*format)++;
}

void scan_arg(char *str, va_list ptr_args, state *s_state) {
  switch (s_state->spec) {
    case 'c':
      scan_char(*str, ptr_args, s_state);
      break;
    case 'd':
    case 'i':
    case 'u':
    case 'x':
    case 'X':
    case 'o':
    case 'p':
      scan_integer(str, ptr_args, s_state);
      break;
    case 'f':
    case 'e':
    case 'E':
    case 'g':
    case 'G':
      scan_float(str, ptr_args, s_state);
      break;
    case 's':
      scan_string(str, ptr_args, s_state);
      break;
    case 'n':
      *va_arg(ptr_args, int *) = str - s_state->ptr;
      break;
    case '%':
      s_state->shift += 1;
      break;
  }
}

void scan_char(char c, va_list ptr_args, state *s_state) {
  if (!s_state->is_ignored) {
    s_state->count++;
    *(va_arg(ptr_args, char *)) = c;
  }
  s_state->shift = 1;
}

void scan_string(char *str, va_list ptr_args, state *s_state) {
  while (s21_is_space(*str)) {
    str++;
    s_state->shift++;
  }
  char *ptr;
  if (!s_state->is_ignored) ptr = va_arg(ptr_args, char *);
  int i = 0;
  if (!s_state->width) s_state->width--;
  while (*str && !s21_is_space(*str) && s_state->width--) {
    if (!s_state->is_ignored)
      *ptr++ = *str++;
    else
      str++;
    i++;
  }
  if (!s_state->is_ignored) {
    *ptr = 0;
    s_state->count++;
  }
  s_state->shift += i;
}

void input_signed_data(long long val, va_list ptr_args, state *s_state) {
  if (!s_state->is_ignored) {
    switch (s_state->length) {
      case 'h':
        *va_arg(ptr_args, short *) = (short)val;
        break;
      case '\0':
        *va_arg(ptr_args, int *) = (int)val;
        break;
      case 'l':
        *va_arg(ptr_args, long *) = (long)val;
        break;
    }
  } else
    s_state->count--;
}

void input_unsigned_data(long long val, va_list ptr_args, state *s_state) {
  if (!s_state->is_ignored) {
    switch (s_state->length) {
      case 'h':
        *va_arg(ptr_args, unsigned short *) = (unsigned short)val;
        break;
      case '\0':
        *va_arg(ptr_args, unsigned int *) = (unsigned int)val;
        break;
      case 'l':
        *va_arg(ptr_args, unsigned long *) = (unsigned long)val;
        break;
    }
  } else
    s_state->count--;
}

void input_float_data(long double val, va_list ptr_args, state *s_state) {
  if (!s_state->is_ignored) {
    switch (s_state->length) {
      case '\0':
        *va_arg(ptr_args, float *) = (float)val;
        break;
      case 'l':
        *va_arg(ptr_args, double *) = (double)val;
        break;
      case 'L':
        *va_arg(ptr_args, long double *) = val;
        break;
    }
  } else
    s_state->count--;
}

void input_pointer_data(long long val, va_list ptr_args, state *s_state) {
  if (!s_state->is_ignored)
    *va_arg(ptr_args, void **) = (void *)(0x0 + val);
  else
    s_state->count--;
}

long long get_integer_value(char *str, int max_symb, state *s_state) {
  long long val;
  switch (s_state->spec) {
    case 'i':
      if (*str == '0' && (is_octodecimal(*(str + 1)) || *(str + 1) == 'x' ||
                          *(str + 1) == 'X')) {
        if (*(str + 1) == 'x' || *(str + 1) == 'X') {
          val = s21_atox(str, max_symb, s_state);
        } else {
          val = s21_atoo(str, max_symb, s_state);
        }
      } else
        val = s21_atou(str, max_symb, s_state);
      break;
    case 'd':
    case 'u':
      val = s21_atou(str, max_symb, s_state);
      break;
    case 'o':
      val = s21_atoo(str, max_symb, s_state);
      break;
    case 'x':
    case 'X':
    case 'p':
      val = s21_atox(str, max_symb, s_state);
      break;
  }
  return val;
}

void scan_integer(char *str, va_list ptr_args, state *s_state) {
  while (s21_is_space(*str)) {
    str++;
    s_state->shift++;
  }
  long long val;
  int max_symb = s_state->width ? s_state->width : __INT32_MAX__;
  int sign = 0;
  if (*str == '-' || *str == '+') {
    if (*str == '-') sign = 1;
    str++;
    max_symb--;
    s_state->shift++;
  }
  val = get_integer_value(str, max_symb, s_state) * (sign ? -1 : 1);
  if (s21_strchr("id", s_state->spec)) {
    if (s_state->status != ERROR) input_signed_data(val, ptr_args, s_state);
  } else if (s21_strchr("uoxX", s_state->spec)) {
    if (s_state->status != ERROR) input_unsigned_data(val, ptr_args, s_state);
  } else if (s_state->spec == 'p')
    if (s_state->status != ERROR) input_pointer_data(val, ptr_args, s_state);
}

void scan_float(char *str, va_list ptr_args, state *s_state) {
  while (s21_is_space(*str)) {
    str++;
    s_state->shift++;
  }
  long double val;
  int max_symb = s_state->width ? s_state->width : __INT32_MAX__;
  int sign = 0;
  if (*str == '-' || *str == '+') {
    if (*str == '-') sign = 1;
    str++;
    max_symb--;
    s_state->shift++;
  }
  val = s21_atof(str, max_symb, s_state) * (sign ? -1 : 1);
  if (s_state->status != ERROR) input_float_data(val, ptr_args, s_state);
}