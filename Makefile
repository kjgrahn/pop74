# Makefile
#
# Copyright (c) 2014 J�rgen Grahn
# All rights reserved.

SHELL=/bin/bash
INSTALLBASE=/usr/local

all: pop74
all: pop74.1
all: tests

libpop.a: album.o
libpop.a: child.o
libpop.a: find.o
libpop.a: basename.o
	$(AR) -r $@ $^

pop74: pop74.o libpop.a
	$(CXX) $(CXXFLAGS) -o $@ $< -L. -lpop

CFLAGS=-W -Wall -pedantic -ansi -g -Os
CXXFLAGS=-W -Wall -pedantic -std=c++98 -g -Os

.PHONY: check checkv
check: tests
	./tests
checkv: tests
	valgrind -q ./tests -v

test.cc: libtest.a
	testicle -o$@ $^

tests: test.o libpop.a libtest.a
	$(CXX) -o $@ test.o -L. -ltest -lpop

test/%.o: CPPFLAGS+=-I.

libtest.a: test/test_path.o
	$(AR) -r $@ $^

.PHONY: install
install: all
	install -m555 pop74 $(INSTALLBASE)/bin/
	install -m644 pop74.1 $(INSTALLBASE)/man/man1/

.PHONY: tags TAGS
tags: TAGS
TAGS:
	etags *.{h,cc}

.PHONY: depend
depend:
	makedepend -- -I. -- -Y -I. *.{c,cc} test/*.cc

.PHONY: clean
clean:
	$(RM) pop74 tests
	$(RM) test.cc
	$(RM) *.o test/*.o lib*.a
	$(RM) version.c
	$(RM) Makefile.bak

love:
	@echo "not war?"

# DO NOT DELETE

album.o: album.h
basename.o: basename.h
find.o: find.h album.h basename.h
pop74.o: find.h album.h
test/test_path.o: basename.h
