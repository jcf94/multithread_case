# ***********************************************
# MYID	: Chen Fan
# LANG	: Makefile
# PROG	: ThreadPool
# ***********************************************

CC=g++
#FLAG=-g

SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp, %.o, $(SRCS))
TARGET=$(patsubst %.cpp, %, $(SRCS))

all: main

$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(FLAG)

%.o: %.cpp
	$(CC) -o $@ -c $< $(FLAG)

.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJS)
	@echo "=-----------------------------="
	@echo "|     Target Clean Success    |"
	@echo "=-----------------------------="
