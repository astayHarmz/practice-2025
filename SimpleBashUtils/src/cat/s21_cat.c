#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Flags {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  char **files;
  int number_of_files;
};

struct Flags args_parsing(int argc, char **argv);
void print_char(unsigned char c, struct Flags flags);
void read_file(FILE *file, struct Flags flags, int *line_num, int empty_line,
               char *prev_char);
void free_flags(struct Flags *flags);

int main(int argc, char **argv) {
  struct Flags flags = args_parsing(argc, argv);
  FILE *file;
  int line_num = 1;
  int empty_line = 0;
  char prev_char = '\n';

  for (int i = 0; i < flags.number_of_files; i++) {
    file = fopen(flags.files[i], "r");
    if (file == NULL) {
      printf("cat: %s: No such file or directory\n", flags.files[i]);
    } else {
      read_file(file, flags, &line_num, empty_line, &prev_char);
      fclose(file);
    }
  }
  free_flags(&flags);
  return 0;
}

struct Flags args_parsing(int argc, char **argv) {
  struct Flags flags = {0, 0, 0, 0, 0, 0, NULL, 0};
  flags.files = malloc(argc * sizeof(char *));

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (strcmp(argv[i], "-b") == 0 ||
          strcmp(argv[i], "--number-nonblank") == 0)
        flags.b = 1;
      else if (strcmp(argv[i], "-e") == 0)
        flags.e = flags.v = 1;
      else if (strcmp(argv[i], "-E") == 0)
        flags.e = 1;
      else if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--number") == 0)
        flags.n = 1;
      else if (strcmp(argv[i], "-s") == 0 ||
               strcmp(argv[i], "--squeeze-blank") == 0)
        flags.s = 1;
      else if (strcmp(argv[i], "-t") == 0)
        flags.t = flags.v = 1;
      else if (strcmp(argv[i], "-T") == 0)
        flags.t = 1;
      else if (strcmp(argv[i], "-v") == 0)
        flags.v = 1;
      else
        printf("cat: illegal option -- %s\n", argv[i]);
    } else {
      flags.files[flags.number_of_files++] = argv[i];
    }
  }

  if (flags.b) flags.n = 0;

  return flags;
}

void read_file(FILE *file, struct Flags flags, int *line_num, int empty_line,
               char *prev_char) {
  char c;
  while ((c = fgetc(file)) != EOF) {
    if (flags.s && c == '\n' && *prev_char == '\n') {
      empty_line++;
      if (empty_line > 1) continue;
    } else {
      empty_line = 0;
    }

    if (*prev_char == '\n') {
      if ((flags.b && c != '\n') || (flags.n && !flags.b)) {
        printf("%6d\t", (*line_num)++);
      }
    }

    print_char(c, flags);
    *prev_char = c;
  }
}

void print_char(unsigned char c, struct Flags flags) {
  if (flags.v) {
    if (c == '\t' && flags.t) {
      printf("^I");
      return;
    } else if (c == '\n' && flags.e) {
      printf("$\n");
      return;
    } else if (c < 32 && c != '\n' && c != '\t') {
      printf("^%c", c + 64);
      return;
    } else if (c == 127) {
      printf("^?");
      return;
    } else if (c > 127 && c < 160) {
      printf("M-^%c", c - 64);
      return;
    }
  }

  putchar(c);
}

void free_flags(struct Flags *flags) { free(flags->files); }