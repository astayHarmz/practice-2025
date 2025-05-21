#include "s21_string.h"

int s21_sprintf(char *str, const char *flow, ...) {
  size_t len = 0;
  if (str != S21_NULL && flow != S21_NULL) {
    str[0] = '\0';
    char *str_begin = str;
    va_list num;
    va_start(num, flow);
    int i = 0, position = 0, c_flag_zero = 0;
    Flags flags;
    zero_set_flag(&flags);
    while (flow[i] != '\0') {
      if (flow[i] == '%' && position == 0) {
        zero_set_flag(&flags);
        position = 1;
      } else if (FLAGSS(flow[i]) && position > 0) {
        flags_in_work(flow[i], &flags);
      } else if ((ZERO_NINE_IF(flow[i]) || flow[i] == '*') && position == 1) {
        if (ZERO_NINE_IF(flow[i])) {
          flags.width = int_read(flow, &i);
          i--;
        } else {
          int wd = va_arg(num, int);
          if (wd < 0) {
            flags.alignment = true;
            flags.width = -wd;
          } else {
            flags.width = wd;
          }
        }
        position = 2;
      } else if (flow[i] == '.' && position > 0 && position < 3) {
        i++;
        if (flow[i] == '*') {
          flags.accuracy = va_arg(num, int);
        } else {
          flags.accuracy = int_read(flow, &i);
          i--;
        }
        position = 3;
      } else if (LENGTH(flow[i]) && position > 0 && position < 4) {
        flags.size = flow[i];
        position = 4;
      } else if (SPECIFICATOR(flow[i]) && position > 0) {
        if (sim_search("eEfgG", flow[i]) && flags.accuracy == -1)
          flags.accuracy = 6;
        char *ptr = str + str_len(str);
        switch (flow[i]) {
          case '%':
            c_to_str(&str, '%', &flags);
            break;
          case 's':
            s_to_str(str, va_arg(num, char *), &flags);
            break;
          case 'd':
          case 'i':
            id_to_str(str, INT_TYPE, 10, &flags);
            break;
          case 'o':
            oxu_to_str(str, UNT_TYPE, 8, &flags);
            break;
          case 'X':
            flags.up = true;
            oxu_to_str(str, UNT_TYPE, 16, &flags);
            break;
          case 'x':
            oxu_to_str(str, UNT_TYPE, 16, &flags);
            break;
          case 'u':
            flags.plus = false;
            oxu_to_str(str, UNT_TYPE, 11, &flags);
            break;
          case 'E':
            flags.up = true;
            eE_to_str(str, FLOAT_TYPE, flags);
            break;
          case 'e':
            eE_to_str(str, FLOAT_TYPE, flags);
            break;
          case 'f':
            f_to_str(str, FLOAT_TYPE, flags);
            break;
          case 'G':
            flags.up = true;
            flags.g_to_G = true;
            gG_to_str(str, FLOAT_TYPE, &flags);
            break;
          case 'g':
            flags.g_to_G = true;
            gG_to_str(str, FLOAT_TYPE, &flags);
            break;
          case 'p':
            p_to_str(str, va_arg(num, void *), &flags);
            break;
          case 'n': {
            if (flags.size == 'l') {
              long *n = va_arg(num, long *);
              if (n != S21_NULL) *n = (int)(str - str_begin + str_len(str));
            } else if (flags.size == 'h') {
              short *n = va_arg(num, short *);
              if (n != S21_NULL) *n = (int)(str - str_begin + str_len(str));
            } else {
              int *n = va_arg(num, int *);
              if (n != S21_NULL) *n = (int)(str - str_begin + str_len(str));
            }
            break;
          }
          case 'c': {
            int ch = va_arg(num, int);
            if (ch == 0) {
              *str = '\0';
              c_flag_zero++;
              len = 1;
            } else {
              c_to_str(&str, ch, &flags);
            }
            break;
          }
        }
        if (flags.up) {
          sim_upp(ptr);
        }
        position = 0;
      } else if (position == 0) {
        buff_to_buff(str, &(flow[i]), 1);
      }
      i++;
    }
    va_end(num);
    if (c_flag_zero == 0) len = str - str_begin + str_len(str);
  }
  return len;
}

void flags_in_work(char c, Flags *flags) {
  switch (c) {
    case '-':
      flags->alignment = true;
      break;
    case '+':
      flags->plus = true;
      break;
    case ' ':
      flags->space = true;
      break;
    case '#':
      flags->sharp = true;
      break;
    case '0':
      flags->zero = true;
      break;
  }
  if (flags->alignment && flags->zero) {
    flags->zero = false;
  }
  if (flags->plus && flags->space) {
    flags->space = false;
  }
}

int int_read(const char *str, int *i) {
  unsigned long long int ret_int = 0;
  while (ZERO_NINE_IF(str[*i])) {
    ret_int *= 10;
    ret_int += str[*i] - '0';
    (*i)++;
    if (ret_int > __INT32_MAX__)
      ret_int = (unsigned long long int)__INT32_MAX__ + 1;
  }
  if (ret_int > __INT32_MAX__) ret_int = -1;
  return ret_int;
}

void int_flags_work(char *res, char *str, char *prefix, Flags *flags) {
  if (res != S21_NULL && str != S21_NULL && prefix != S21_NULL) {
    int count_space = 0;
    int count_zero = 0;
    if (!flags->alignment && flags->accuracy == -1 &&
        flags->width > str_len(prefix) + str_len(str) && flags->zero) {
      flags->accuracy = flags->width - str_len(prefix);
    }
    count_zero = flags->accuracy - str_len(str);
    if (count_zero > 0) {
      count_space = flags->width - count_zero - str_len(str) - str_len(prefix);
    } else {
      count_space = flags->width - str_len(str) - str_len(prefix);
    }

    if (!flags->alignment) {
      for (int i = 0; i < count_space; i++) {
        buff_to_buff(res, " ", 1);
      }
    }
    buff_to_buff(res, prefix, str_len(prefix));
    for (int i = 0; i < count_zero; i++) {
      buff_to_buff(res, "0", 1);
    }
    buff_to_buff(res, str, str_len(str));
    if (flags->alignment) {
      for (int i = 0; i < count_space; i++) {
        buff_to_buff(res, " ", 1);
      }
    }
  }
}

void id_to_str(char *res, long long int num, int num_sys, Flags *flags) {
  if (num < 0) {
    flags->minus = true;
  }
  convert_to_str(res, num, num_sys, flags);
}

void oxu_to_str(char *res, unsigned long long int num, int num_sys,
                Flags *flags) {
  convert_to_str(res, num, num_sys, flags);
}

void float_flags_work(char *res, char *str, char *prefix, Flags *flags) {
  if (res != S21_NULL && str != S21_NULL && prefix != S21_NULL) {
    int count_zero = 0;
    if (flags->zero) {
      count_zero = flags->width - str_len(str) - str_len(prefix);
    }
    int count_space =
        flags->width - str_len(str) - str_len(prefix) - count_zero;

    if (!flags->alignment) {
      for (int i = 0; i < count_space; i++) {
        buff_to_buff(res, " ", 1);
      }
    }
    buff_to_buff(res, prefix, str_len(prefix));
    for (int i = 0; i < count_zero; i++) {
      buff_to_buff(res, "0", 1);
    }
    buff_to_buff(res, str, str_len(str));
    if (flags->alignment) {
      for (int i = 0; i < count_space; i++) {
        buff_to_buff(res, " ", 1);
      }
    }
  }
}

void f_to_str(char *res, long double num, Flags flags) {
  if (res != S21_NULL) {
    Flags int_flags;
    zero_set_flag(&int_flags);
    char prefix[2] = "";
    char float_str[10000] = "";
    if (flags.plus && num >= 0) {
      buff_to_buff(prefix, "+", 1);
    } else if (num < 0) {
      num *= -1;
      buff_to_buff(prefix, "-", 1);
    } else if (flags.space)
      buff_to_buff(prefix, " ", 1);
    long long int_part = (long long)num;
    long double frac_part = num - int_part;
    id_to_str(float_str, int_part, 10, &int_flags);
    if (flags.accuracy > 0 || flags.sharp) {
      buff_to_buff(float_str, ".", 1);
      for (int i = 0; i < flags.accuracy; i++) {
        frac_part *= 10;
        int digit = (int)frac_part;
        id_to_str(float_str, digit, 10, &int_flags);
        frac_part -= digit;
      }
    }

    if (frac_part >= 0.5) {
      int pos = str_len(float_str) - 1;
      while (pos >= 0) {
        if (float_str[pos] != '.') {
          if (float_str[pos] < '9') {
            float_str[pos]++;
            break;
          } else {
            float_str[pos] = '0';
          }
        }
        pos--;
      }
    }
    float_flags_work(res, float_str, prefix, &flags);
  }
}

void eE_to_str(char *res, long double num, Flags flags) {
  if (res != S21_NULL) {
    char prefix[2] = "";
    char float_str[10000] = "";
    if (flags.plus && num >= 0) {
      buff_to_buff(prefix, "+", 1);
    } else if (num < 0) {
      num *= -1;
      buff_to_buff(prefix, "-", 1);
    } else {
      if (flags.space) buff_to_buff(prefix, " ", 1);
    }
    int exp = 0;
    if (num >= 1.0) {
      while (num >= 10.0) {
        num /= 10;
        exp++;
      }
    } else {
      while (num < 1.0 && num != 0) {
        num *= 10;
        exp--;
      }
    }
    Flags float_flags;
    zero_set_flag(&float_flags);
    if (flags.g_to_G) {
      flags.accuracy--;
    }
    float_flags.accuracy = flags.accuracy;
    float_flags.sharp = flags.sharp;
    f_to_str(float_str, num, float_flags);
    buff_to_buff(float_str, "e", 1);
    if (exp < 0) {
      buff_to_buff(float_str, "-", 1);
      exp *= -1;
    } else {
      buff_to_buff(float_str, "+", 1);
    }
    if (exp < 10) {
      buff_to_buff(float_str, "0", 1);
    }
    float_flags.accuracy = -1;
    id_to_str(float_str, exp, 10, &float_flags);
    float_flags_work(res, float_str, prefix, &flags);
  }
}

void gG_to_str(char *res, long double num, Flags *flags) {
  if (res != S21_NULL) {
    size_t len0 = str_len(res);
    eE_to_str(res, num, *flags);
    size_t len1 = check_float(res + len0);
    res[len0] = '\0';
    f_to_str(res, num, *flags);
    size_t len2 = check_float(res + len0);
    if (len1 < len2) {
      res[len0] = '\0';
      eE_to_str(res, num, *flags);
    }
  }
}

void s_to_str(char *res, char *str, Flags *flags) {
  if (res != S21_NULL && str != S21_NULL) {
    int count = (int)str_len(str);
    if (flags->accuracy >= 0)
      count = (int)str_len(str) < flags->accuracy ? (int)str_len(str)
                                                  : flags->accuracy;
    if ((int)flags->width > count && !flags->alignment) {
      for (size_t i = 0; i < flags->width - count; i++) {
        buff_to_buff(res, " ", 1);
      }
    }
    buff_to_buff(res, str, count);
    if ((int)flags->width > count && flags->alignment) {
      for (size_t i = 0; i < flags->width - count; i++)
        buff_to_buff(res, " ", 1);
    }
  }
}

void p_to_str(char *res, void *ptr, Flags *flags) {
  if (res != S21_NULL) {
    if (ptr == S21_NULL) {
      buff_to_buff(res, "0x0", 5);
    } else {
      flags->sharp = true;
      if (flags->plus) buff_to_buff(res, "+", 1);
      if (flags->accuracy) {
        flags->accuracy += 2;
      }
      oxu_to_str(res, (long long int)ptr, 16, flags);
    }
  }
}

void c_to_str(char **res, char c, Flags *flags) {
  if (*res != S21_NULL) {
    int width = flags->width > 1 ? flags->width : 1;
    if (!flags->alignment) {
      char fill_char = flags->zero ? '0' : ' ';
      for (int i = 1; i < width; i++) {
        buff_to_buff(*res, &fill_char, 1);
      }
    }
    char str[2] = {c, '\0'};
    buff_to_buff(*res, str, 1);
    if (flags->alignment) {
      for (int i = 1; i < width; i++) {
        buff_to_buff(*res, " ", 1);
      }
    }
    *res += str_len(*res);
  }
}

void convert_to_str(char *res, long long int num, int num_sys, Flags *flags) {
  if (res != NULL && num_sys != 0) {
    char str[50] = "";
    char prefix[3] = "";
    int i = 0;
    int u_flag = 0;
    int o_flag = 0;

    if (num_sys == 11) {
      num_sys = 10;
      u_flag++;
    }
    if (num == 0 && flags->accuracy != 0) {
      str[0] = '0';
      i++;
      o_flag++;
    }
    while (num != 0) {
      int rem_num = num_sys != 0 ? abs((int)(num % num_sys)) : 0;
      if (rem_num < 10) {
        str[i] = rem_num + '0';
      } else {
        str[i] = rem_num - 10 + 'a';
      }
      num /= num_sys;
      i++;
    }
    for (int j = 0; j < i / 2; j++) {
      char tmp = str[j];
      str[j] = str[i - j - 1];
      str[i - j - 1] = tmp;
    }
    if (num_sys == 8 || num_sys == 16) {
      if (flags->sharp && num_sys == 8 && o_flag == 0) {
        buff_to_buff(prefix, "0", 1);
        if (flags->accuracy > 0) flags->accuracy--;
      } else if (flags->sharp) {
        if (str[0] != '0') {
          if (flags->accuracy > 1) {
            flags->accuracy -= 2;
          }
          buff_to_buff(prefix, "0x", 2);
        }
      }
    } else if (flags->minus) {
      buff_to_buff(prefix, "-", 1);
    } else if (flags->plus) {
      buff_to_buff(prefix, "+", 1);
    } else if (flags->space && u_flag != 1) {
      buff_to_buff(prefix, " ", 1);
    }
    int_flags_work(res, str, prefix, flags);
  }
}

void sim_upp(char *str) {
  if (str != NULL) {
    size_t len = str_len(str);
    for (size_t i = 0; i < len; i++) {
      if (str[i] >= 'a' && str[i] <= 'z') {
        str[i] = str[i] - 32;
      }
    }
  }
}

void zero_set_flag(Flags *flags) {
  meaning_to_str(flags, 0, sizeof(Flags));
  flags->accuracy = -1;
}

size_t check_float(char *res) {
  int count = 0;
  if (res != S21_NULL) {
    size_t i = 0;
    while ((!ZERO_NINE_IF(res[i]) || res[i] == '0') && i < str_len(res)) {
      i++;
    }
    while ((ZERO_NINE_IF(res[i]) || res[i] == '.') && i < str_len(res)) {
      if (res[i] != '.') {
        count++;
      }
      i++;
    }
  }
  return count;
}

char *buff_to_buff(char *dest, const char *src, size_t n) {
  if (dest != S21_NULL && src != S21_NULL) {
    size_t size_dest = str_len(dest);
    size_t count = 0;
    while (count < n && src[count] != '\0') {
      dest[size_dest + count] = src[count];
      count++;
    }
    dest[size_dest + count] = '\0';
  }
  return dest;
}

size_t str_len(const char *src) {
  size_t i = 0;
  if (src != NULL) {
    while (src[i] != '\0') i++;
  }
  return i;
}

void *meaning_to_str(void *str, int c, size_t n) {
  if (str != S21_NULL) {
    unsigned char *ptr_str = (unsigned char *)str;
    for (size_t i = 0; i < n; i++) {
      ptr_str[i] = (unsigned char)c;
    }
  }
  return str;
}

char *sim_search(const char *str, int symbol) {
  int len = str_len(str) + 1;
  const char *res = S21_NULL;

  for (int x = 0; x < len && res == S21_NULL; x += 1) {
    if (str[x] == symbol) {
      res = (str + x);
    }
  }
  return (char *)res;
}
