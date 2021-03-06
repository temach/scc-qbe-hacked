# scc version
VERSION = 0.1

## Customize below to fit your system
ARCHS = z80 i386-sysv amd64-sysv qbe
# default architecure used in nested makefiles
ARCH = qbe
DRIVER = posix

# Can be c89 or c99
STD = c99

# paths
PREFIX    = $(HOME)
MANPREFIX = $(PREFIX)/share/man

# scc expects to be built by a C99 compiler
# if your system is not at least POSIX 2004 compatible, adjust CC
# CC = c99
# AR = ar

# for Plan9 add -D_SUSV2_SOURCE
SCC_CFLAGS = -DARCH=\"$(ARCH)\" \
             $(CSTDINC) \
             -DPREFIX=\"$(PREFIX)\" \
             -DDEBUG \
             $(CFLAGS) \
             -Wall -g -O0 -static

SCC_LDFLAGS = $(LDFLAGS)

.c.o:
	$(CC) $(SCC_CFLAGS) -o $@ -c $<

.c:
	$(CC) $(SCC_CFLAGS) $(SCC_LDFLAGS) -o $@ $<
