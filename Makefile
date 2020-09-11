SRCS_DIRS := $(shell find ./src/ -maxdepth 3 -type d)
SRCS_DIRS += $(shell find ./test/ -maxdepth 3 -type d)
SRCS := $(foreach dir,$(SRCS_DIRS),$(wildcard $(dir)/*.cpp))

MAKEROOT := $(shell pwd)
OBJ_DIR= $(MAKEROOT)/obj
$(shell mkdir -p "$(OBJ_DIR)")

OBJS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRCS)))

CC = g++
INCLUDES = -Iinclude \
		   -Itest \
		   -I.
LIBS = -lcurl -lpthread
CCFLAGS = -g -Wall -O0
OUTPUT = nacos-cli.out

vpath %.cpp $(SRCS_DIRS)

DEP:=$(OBJS:%.o=%.d)

all : $(OUTPUT)

$(OUTPUT) : $(OBJS)
	$(CC) $^ -o $@ $(INCLUDES) $(LIBS) $(CCFLAGS)

-include $(DEP)

$(OBJS) : $(OBJ_DIR)/%.o : %.cpp
	$(CC) -c $< -o $@ $(CCFLAGS) $(INCLUDES)
	$(CC) $(CFLAGS) $(INCLUDES) -MM -MT $@ -MF $(patsubst %.o, %.d, $@) $<

testcase : all
	SRCS = $(SRCS:testcase/*.cpp)

clean:
	rm -rf *.out
	rm -rf obj

.PHONY:clean
