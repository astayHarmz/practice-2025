ifndef VERBOSE
.SILENT:
endif

NAME = s21_string.a
SOURCEFILES := $(wildcard s21*.c)
OBJECTFILES := $(patsubst %.c,%.o,$(SOURCEFILES))
CC = gcc
FLAGS = -Wall -Werror -Wextra 
LDFLAGS = `pkg-config --cflags --libs check` -lm -lcheck
GCOVFLAGS := -fprofile-arcs -ftest-coverage
UNAME = $(shell uname)

ifeq ($(UNAME),Linux)
	LDFLAGS = -lcheck -pthread -lrt -lm -lsubunit
	OPEN=xdg-open
endif
ifeq ($(UNAME),Darwin)
	LDFLAGS += `pkg-config --cflags --libs check`
	OPEN=open
endif

all: $(NAME)

$(NAME): $(OBJECTFILES)
	ar rcs $(NAME) $(OBJECTFILES)
	rm -rf *.o
	ranlib $(NAME)

test: $(NAME) unittests $(OBJECTFILES)
	$(CC) $(FLAGS) unittests.c $(NAME) $(LDFLAGS) -o unittests.out
	rm -rf *.o
	./unittests.out

gcov_report: $(NAME) unittests
	$(CC) $(GCOVFLAGS) $(SOURCEFILES) unittests.c $(LDFLAGS) --coverage -o report.out 
	./report.out
	gcov -b -l -p -c s21_*.gcno
	gcovr -o gcov_report.html --html --html-details --exclude 'unittests.c'
	open gcov_report.html

%.o: %.c
	$(CC) $(FLAGS) -c $^ -o $@

check: all unittests_nofork
	cppcheck --enable=all --suppress=missingIncludeSystem *.h *.c
	cp ../materials/linters/.clang-format .
	find . -name '*.h' -o -name '*.c' ! -name 'unittests.c' -exec clang-format -n {} +
	rm -rf .clang-format
ifeq ($(UNAME),Linux)
	$(CC) unittests.c s21_string.a $(LDFLAGS) 
	touch valgrind_res.txt
	valgrind --leak-check=full --log-file="valgrind_res.txt" --track-origins=yes --show-reachable=yes ./a.out
	grep "ERROR" valgrind_res.txt
endif

clean:
	rm -rf *.o *.a *.out *.gcov *.gcda *.gcno *.info report unittests.c unit_tests/_unittests.check valgrind_res.txt *.html *.css *.h.gch

include makefiles/check.mk