FLAGS=-Wall -Werror -Wextra

all: s21_cat s21_grep

s21_cat:
	gcc cat/s21_cat.c ${FLAGS} -o cat/s21_cat

s21_grep:
	gcc grep/s21_grep.c ${FLAGS} -o grep/s21_grep

s21_cat_test:
	bash ./cat/tests.bash

s21_grep_test:
	bash ./grep/tests.bash