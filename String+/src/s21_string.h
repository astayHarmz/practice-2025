#ifndef S21_STRING_H
#define S21_STRING_H

#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef S21_NULL
#define S21_NULL ((void *)0)
#endif

#define SCAN_SPECS "cdiouxXpfeEgGsn%"
#define PRINTF_SPECS "cdifeEgGoxXupns"
#define DIGITS "0123456789abcdef"
#define ERROR 1
#define BUFF_SIZE 1024
#define BLOCK_SIZE 256
#define POS_INF 1.0 / 0.0
#define NEG_INF -1.0 / 0.0
typedef unsigned long s21_size_t;

typedef struct Flags {
  bool alignment;
  bool plus;
  bool space;
  bool sharp;
  bool zero;
  bool up;
  s21_size_t width;
  bool minus;
  int accuracy;
  char size;
  bool g_to_G;
} Flags;

typedef struct options {
  int width;
  int prec;
  int length;
  int spec;
  bool is_prec;
  bool f_plus;
  bool f_minus;
  bool f_space;
  bool f_hash;
  bool f_zero;
  char *ptr_str;
} options;

typedef struct state {
  int status;
  int count;
  int is_ignored;
  int width;
  int length;
  int spec;
  int shift;
  char *ptr;
} state;

#define FLAGSS(c) \
  ((c) == '-' || (c) == '+' || (c) == ' ' || (c) == '#' || (c) == '0')
#define ZERO_NINE_IF(c) ((c) >= '0' && (c) <= '9')
#define LENGTH(c) ((c) == 'h' || (c) == 'l' || (c) == 'L')
#define SPECIFICATOR(c)                                                  \
  ((c) == 'c' || (c) == 'd' || (c) == 'i' || (c) == 'e' || (c) == 'E' || \
   (c) == 'f' || (c) == 'g' || (c) == 'G' || (c) == 'o' || (c) == 's' || \
   (c) == 'u' || (c) == 'x' || (c) == 'X' || (c) == 'p' || (c) == 'n' || \
   (c) == '%')

#define INT_TYPE \
  (flags.size == 'l' ? va_arg(num, long long int) : va_arg(num, int))
#define UNT_TYPE                                           \
  (flags.size == 'l' ? va_arg(num, unsigned long long int) \
                     : va_arg(num, unsigned int))
#define FLOAT_TYPE \
  (flags.size == 'L' ? va_arg(num, long double) : va_arg(num, double))

// basic functions
void *s21_memchr(const void *str, int c, s21_size_t n);
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
void *s21_memcpy(void *dest, const void *src, s21_size_t n);
void *s21_memset(void *str, int c, s21_size_t n);
char *s21_strncat(char *dest, const char *src, s21_size_t n);
char *s21_strchr(const char *str, int c);
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
s21_size_t s21_strcspn(const char *str1, const char *str2);
char *s21_strerror(int errnum);
s21_size_t s21_strlen(const char *str);
char *s21_strpbrk(const char *str1, const char *str2);
char *s21_strrchr(const char *str, int c);
char *s21_strstr(const char *haystack, const char *needle);
char *s21_strtok(char *str, const char *delim);
int s21_sprintf(char *str, const char *flow, ...);
int s21_sscanf(const char *str, const char *format, ...);

// sprintf functions
void flags_in_work(char c, Flags *flags);
int int_read(const char *str, int *i);
void int_flags_work(char *res, char *str, char *prefix, Flags *flags);
void id_to_str(char *res, long long int num, int num_sys, Flags *flags);
void oxu_to_str(char *res, unsigned long long int num, int num_sys,
                Flags *flags);

void float_flags_work(char *res, char *str, char *prefix, Flags *flags);
void f_to_str(char *res, long double num, Flags flags);
void eE_to_str(char *res, long double num, Flags flags);
void gG_to_str(char *res, long double num, Flags *flags);

void s_to_str(char *res, char *str, Flags *flags);
void p_to_str(char *res, void *ptr, Flags *flags);
void c_to_str(char **res, char c, Flags *flags);

void convert_to_str(char *res, long long int num, int num_sys, Flags *flags);
void sim_upp(char *str);
void zero_set_flag(Flags *flags);
s21_size_t check_float(char *res);

char *buff_to_buff(char *dest, const char *src, s21_size_t n);
s21_size_t str_len(const char *src);
void *meaning_to_str(void *str, int c, s21_size_t n);
char *sim_search(const char *str, int symbol);

// scanf common funcs
void scan_format(char *str, char *format, va_list ptr_args, state *s_state);
void parse_options(char **format, state *s_state);
void scan_arg(char *str, va_list ptr_args, state *s_state);
void scan_char(char c, va_list ptr_args, state *s_state);
void scan_string(char *str, va_list ptr_args, state *s_state);
void scan_integer(char *str, va_list ptr_args, state *s_state);
void scan_float(char *str, va_list ptr_args, state *s_state);

bool s21_is_digit(int c);
bool s21_is_space(int c);
long long s21_atou(char *str, int n, state *s_state);
long s21_atox(char *str, int n, state *s_state);
long s21_atoo(char *str, int n, state *s_state);
long double s21_atof(char *str, int n, state *s_state);
int is_hexidecimal(char c);
int is_octodecimal(char c);
int hex_to_dec(char c);

// bonus functions
void *s21_to_upper(const char *str);
void *s21_to_lower(const char *str);
void *s21_insert(const char *src, const char *str, size_t start_index);
void *s21_trim(const char *src, const char *trim_chars);

#endif