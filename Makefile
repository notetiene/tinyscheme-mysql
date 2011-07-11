# vim: tabstop=8 noexpandtab

MY_CONFIG = mysql_config

DESTDIR =
PREFIX  = /usr/local
MANDIR  = $(PREFIX)/share/man

CC      = gcc
CFLAGS  = -Wall -ansi -pedantic  -ggdb -g
CFLAGS += -Wno-long-long
CFLAGS += $(shell $(MY_CONFIG) --include)
CFLAGS += -DUSE_INTERFACE=1 \
          -DSTANDALONE=0 \
          -DUSE_MATH=1 \
          -DOSX=1 \
          -DUSE_DL=1 \
          -DUSE_ERROR_HOOK=1 \
          -DUSE_ASCII=1
LFLAGS  = 
LFLAGS += $(shell $(MY_CONFIG) --libs)

INC     = 
SRC     = 

OBJ     = $(SRC:.c=.o)
BIN     = 
LIB     = 

default: all

all:	tsx_mysql.so

clean:
	rm -f $(OBJ) tsx_mysql.so

tsx_mysql.so: tsx_mysql.c 
	$(CC) $(CFLAGS) -shared -Wall -fPIC  -o $@ $(DEBUG) $< $(LFLAGS)

help:
	groff -man -Tascii tsx_mysql.1 | less

.PHONY: default clean test all install install-man uninstall 

