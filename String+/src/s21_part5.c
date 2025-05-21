#include "s21_string.h"

void *s21_to_upper(const char *str) {
  char *result = S21_NULL;
  if (str) {
    s21_size_t len = s21_strlen(str);
    result = malloc(len + 1);
    if (result) {
      for (s21_size_t i = 0; i < len; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
          result[i] = str[i] - 32;
        } else {
          result[i] = str[i];
        }
      }
      result[len] = '\0';
    }
  }

  return result;
}

void *s21_to_lower(const char *str) {
  char *result = S21_NULL;
  if (str) {
    s21_size_t len = s21_strlen(str);
    result = malloc(len + 1);
    if (result) {
      for (s21_size_t i = 0; i < len; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
          result[i] = str[i] + 32;
        } else {
          result[i] = str[i];
        }
      }
      result[len] = '\0';
    }
  }

  return result;
}

void *s21_insert(const char *src, const char *str, size_t start_index) {
  char *result = S21_NULL;

  s21_size_t src_len = s21_strlen(src);
  s21_size_t len = s21_strlen(str);
  if (src && str) {
    if (start_index <= src_len) {
      result = malloc(src_len + len + 1);
      if (result) {
        for (s21_size_t i = 0; i < start_index; i++) {
          result[i] = src[i];
        }
        for (s21_size_t i = 0; i < len; i++) {
          result[start_index + i] = str[i];
        }
        for (s21_size_t i = start_index; i < src_len; i++) {
          result[len + i] = src[i];
        }
        result[src_len + len] = '\0';
      }
    }
  }
  return result;
}

void *s21_trim(const char *src, const char *trim_chars) {
  char *result = S21_NULL;
  if (src) {
    s21_size_t len = s21_strlen(src);
    result = malloc(len + 1);
    if (result) {
      s21_size_t tr_len = s21_strlen(trim_chars);
      s21_size_t cur = 0;
      int match = 1;
      while (match) {
        match = 0;
        for (s21_size_t i = 0; i < tr_len; i++) {
          if (trim_chars[i] == src[cur]) {
            cur++;
            match = 1;
            break;
          }
        }
      }
      for (s21_size_t i = 0; i < len - cur; i++) {
        result[i] = src[cur + i];
      }
      result[len - cur] = '\0';
      s21_size_t len_res = s21_strlen(result);
      cur = len_res - 1;
      match = 1;
      while (match && len_res > 0) {
        match = 0;
        for (s21_size_t i = 0; i < tr_len; i++) {
          if (trim_chars[i] == result[cur]) {
            result[cur] = '\0';
            cur--;
            match = 1;
            break;
          }
        }
      }
    }
  }
  return result;
}