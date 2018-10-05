# ***********************************************
# MYID	: Chen Fan
# LANG	: Makefile
# PROG	: ThreadPool
# ***********************************************

CC=g++
CFLAGS+=-std=c++14

LD=g++
LDFLAGS+=-lpthread

SRCS=$(wildcard *.cpp */*.cpp)
OBJS=$(patsubst %.cpp, %.o, $(SRCS))
TARGET=$(patsubst %.cpp, %, $(SRCS))

all: main

$(TARGET):$(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJS)
	@echo "=-----------------------------="
	@echo "|     Target Clean Success    |"
	@echo "=-----------------------------="
