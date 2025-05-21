#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

struct Flags {
  int i;
  int v;
  int c;
  int l;
  int n;
  int e;
  int o;
  int h;
  int s;
  char **files;
  char *pattern;
  int number_of_files;
};

struct Flags args_parsing(int argc, char **argv);
void parse_double_flag(const char *flag, struct Flags *flags);
void print_char(unsigned char c, struct Flags flags);
void read_file(FILE *file, const char *filename, struct Flags flags);
void free_flags(struct Flags *flags);

int main(int argc, char **argv) {
  struct Flags flags = args_parsing(argc, argv);
  FILE *file;
  for (int i = 0; i < flags.number_of_files; i++) {
    file = fopen(flags.files[i], "r");
    if (file == NULL) {
      if (!flags.s)
        printf("grep: %s: No such file or directory\n", flags.files[i]);
    } else {
      read_file(file, flags.files[i], flags);
      fclose(file);
    }
  }
  free_flags(&flags);
  return 0;
}

void parse_double_flag(const char *flag, struct Flags *flags) {
  if (strlen(flag) == 3) {
    switch (flag[1]) {
      case 'i':
        flags->i = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'o':
        flags->o = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
    }
    switch (flag[2]) {
      case 'i':
        flags->i = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'o':
        flags->o = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
    }
  }
}

struct Flags args_parsing(int argc, char **argv) {
  struct Flags flags = {0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, 0};
  flags.files = malloc(argc * sizeof(char *));
  int pattern_set = 0;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (strlen(argv[i]) == 3) {
        parse_double_flag(argv[i], &flags);
      } else if (strcmp(argv[i], "-e") == 0) {
        flags.e = 1;
        if (i + 1 < argc) {
          flags.pattern = argv[++i];
          pattern_set = 1;
        }
      } else if (strcmp(argv[i], "-i") == 0)
        flags.i = 1;
      else if (strcmp(argv[i], "-v") == 0)
        flags.v = 1;
      else if (strcmp(argv[i], "-c") == 0)
        flags.c = 1;
      else if (strcmp(argv[i], "-l") == 0)
        flags.l = 1;
      else if (strcmp(argv[i], "-n") == 0)
        flags.n = 1;
      else if (strcmp(argv[i], "-h") == 0)
        flags.h = 1;
      else if (strcmp(argv[i], "-o") == 0)
        flags.o = 1;
      else if (strcmp(argv[i], "-s") == 0)
        flags.s = 1;
      else if (!flags.s)
        printf("grep: illegal option -- %s\n", argv[i]);
    } else {
      if (!pattern_set && !flags.pattern) {
        flags.pattern = argv[i];
        pattern_set = 1;
      } else {
        flags.files[flags.number_of_files++] = argv[i];
      }
    }
  }

  return flags;
}

void read_file(FILE *file, const char *filename, struct Flags flags) {
  char line[MAX_LINE];
  int line_num = 1;
  int match_count = 0;
  regex_t regex;

  if (flags.i)
    regcomp(&regex, flags.pattern, REG_EXTENDED | REG_ICASE);
  else
    regcomp(&regex, flags.pattern, REG_EXTENDED);

  while (fgets(line, MAX_LINE, file) && !(flags.v && flags.o)) {
    int found = regexec(&regex, line, 0, NULL, 0);
    int matched = flags.v ? (found != 0) : (found == 0);
    int current_line = line_num;
    int matches_in_line = 0;

    if (matched) {
      match_count++;
      if (flags.l) {
        printf("%s\n", filename);
        break;
      }
      if (flags.c) continue;

      if (!flags.h && filename && flags.number_of_files > 1) {
        printf("%s:", filename);
      }

      if (flags.n) {
        printf("%d:", line_num);
      }

      if (flags.o && found == 0) {
        regmatch_t match;
        char *ptr = line;
        while (regexec(&regex, ptr, 1, &match, 0) == 0) {
          if (flags.n) printf("%d:", current_line);
          if (!flags.h && flags.number_of_files > 1) printf("%s:", filename);
          printf("%.*s\n", (int)(match.rm_eo - match.rm_so), ptr + match.rm_so);
          ptr += match.rm_eo;
          matches_in_line++;
        }
      } else {
        printf("%s", line);
        if (strchr(line, '\n') == NULL) {
          printf("\n");
        }
      }
    }
    line_num++;
  }

  if (flags.c) {
    if (!flags.h && filename && flags.number_of_files > 1)
      printf("%s:", filename);
    printf("%d\n", match_count);
  }

  regfree(&regex);
}

void free_flags(struct Flags *flags) { free(flags->files); }
