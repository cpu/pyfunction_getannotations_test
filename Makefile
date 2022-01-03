CMD=main
CC=gcc

ifeq ($(pythonVersion),)
	PYTHON_VERSION := python3.10
else
	PYTHON_VERSION := $(pythonVersion)
endif

PYTHON_CFLAGS:=$(shell $(PYTHON_VERSION)-config --cflags)
PYTHON_LDFLAGS:=$(shell $(PYTHON_VERSION)-config --ldflags)
CFLAGS=-Wall -O0 -g $(PYTHON_CFLAGS)
LDFLAGS=-lm $(PYTHON_LDFLAGS) -l$(PYTHON_VERSION)

.PHONY: all
all: $(CMD)

.PHONY: clean
clean:
	$(RM) *~ *.o $(CMD)

$(CMD): $(CMD).o
	$(CC) $(CFLAGS) $(CMD).o -o $(CMD) $(LDFLAGS)
