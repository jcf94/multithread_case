# ***********************************************
# MYID : Chen Fan
# LANG : Makefile
# PROG : 
# ***********************************************

CC = g++
CFLAGS +=

LD = g++
LDFLAGS +=

NAME=$(wildcard *.cpp)
TARGET=$(patsubst %.cpp, %, $(NAME))
SRCS=$(wildcard src/*.cpp)
OBJS=$(patsubst %.cpp, %.o, $(SRCS))

CLEAN-O = rm -f $(OBJS)

release: CFLAGS += -O3
release: LDFLAGS += -O3
release: all

debug: CFLAGS += -g3
debug: LDFLAGS += -g3
debug: all

all: $(TARGET)
	$(CLEAN-O)
	@echo "=------------------------------="
	@echo "|     Target Make Success      |"
	@echo "=------------------------------="

$(TARGET): $(OBJS) $(NAME)
	$(LD) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean
clean:
	rm -rf $(TARGET)
	$(CLEAN-O)
	@echo "=-----------------------------="
	@echo "|     Target Clean Success    |"
	@echo "=-----------------------------="

show:
	@echo NAME: $(NAME)
	@echo TARGET: $(TARGET)
	@echo SRCS: $(SRCS)
	@echo OBJS: $(OBJS)
