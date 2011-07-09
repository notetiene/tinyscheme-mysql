# vim: tabstop=8 noexpandtab

MY_CONFIG = mysql_config

DESTDIR =
PREFIX  = /usr/local
MANDIR  = $(PREFIX)/share/man

CC      = gcc
CFLAGS  = -Wall -ansi -pedantic 
CFLAGS += -Wno-long-long
CFLAGS += $(shell $(MY_CONFIG) --include)
LFLAGS  = 
LFLAGS += $(shell $(MY_CONFIG) --libs)
#LFLAGS += -lrt

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
	$(CC) $(CFLAGS) -shared -Wall -fPIC -DUSE_DL=1  -o $@ $(DEBUG) $< $(LFLAGS)

.PHONY: default clean test all install install-man uninstall 

