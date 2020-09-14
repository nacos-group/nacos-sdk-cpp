SRCS_DIRS := $(shell find ./src/ -maxdepth 3 -type d)
SRCS_DIRS += $(shell find ./test/ -maxdepth 3 -type d)
SRCS := $(foreach dir,$(SRCS_DIRS),$(wildcard $(dir)/*.cpp))

MAKEROOT = $(shell pwd)
OBJ_DIR = $(MAKEROOT)/obj
DEP_DIR = $(MAKEROOT)/dep

OBJS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRCS)))
DEPS = $(patsubst %.o, $(DEP_DIR)/%.d, $(notdir $(OBJS)))

CC = g++
INCLUDES = -Iinclude \
		   -Itest \
		   -I.

LIBS = -lcurl -lpthread
CCFLAGS = -g -Wall -O0 -fPIC
OUTPUT = nacos-cli.out
OUTLIB = nacos-cli.so

vpath %.cpp $(SRCS_DIRS)

all :$(DEPS) $(OUTPUT) $(OUTLIB)

$(OUTPUT) : $(OBJS)
	$(info Linking $@ ...)
	@$(CC) $^ -o $@ $(INCLUDES) $(LIBS) $(CCFLAGS)

$(OUTLIB) : $(OBJS)
	$(info Linking $@ ...)
	@$(CC) $^ -o $@ $(INCLUDES) $(LIBS) $(CCFLAGS) -shared

$(OBJS) : $(OBJ_DIR)/%.o : %.cpp
	$(info Building $@ ...)
	@mkdir -p "$(OBJ_DIR)"
	@$(CC) -c $< -o $@ $(CCFLAGS) $(INCLUDES)

-include $(DEPS)
$(DEPS) : $(DEP_DIR)/%.d : %.cpp
	$(info Creating $< Dependencies file ...)
	@mkdir -p "$(DEP_DIR)"
	@$(CC) $(CCFLAGS) $(INCLUDES) -MM -MT $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $<)) -MF $@ $<


testcase : all
	SRCS = $(SRCS:testcase/*.cpp)

clean:
	rm -rf *.out
	rm -rf *.so
	rm -rf $(OBJ_DIR)
	rm -rf $(DEP_DIR)

.PHONY:clean
