SRCS_DIRS := $(shell find ./src/ -maxdepth 3 -type d)
TEST_SRCS_DIRS := $(shell find ./test/ -maxdepth 3 -type d)
ALL_SRCS_DIRS := $(shell find ./src/ -maxdepth 3 -type d)
ALL_SRCS_DIRS += $(shell find ./test/ -maxdepth 3 -type d)

SRCS := $(foreach dir,$(SRCS_DIRS),$(wildcard $(dir)/*.cpp))
TEST_SRCS := $(foreach dir,$(SRCS_DIRS),$(wildcard $(dir)/*.cpp))
ALL_SRCS := $(foreach dir,$(ALL_SRCS_DIRS),$(wildcard $(dir)/*.cpp))

MAKEROOT = $(shell pwd)
OBJ_DIR = $(MAKEROOT)/obj
DEP_DIR = $(MAKEROOT)/dep

OBJS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRCS)))
TEST_OBJS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(TEST_SRCS)))
ALL_OBJS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(ALL_SRCS)))
DEPS = $(patsubst %.o, $(DEP_DIR)/%.d, $(notdir $(ALL_OBJS)))

CC = g++
INCLUDES = -Iinclude \
		   -Itest \
		   -I.

LIBS = -lcurl -lpthread
CCFLAGS = -g -Wall -O0 -fPIC
OUTPUT = nacos-cli.out
OUTLIB1 = libnacos-cli.so
OUTLIB2 = libnacos-cli.a

vpath %.cpp $(ALL_SRCS_DIRS)

all :$(DEPS) $(OUTPUT) $(OUTLIB1) $(OUTLIB2)

$(OUTPUT) : $(ALL_OBJS)
	$(info Linking $@ ...)
	@$(CC) $^ -o $@ $(INCLUDES) $(LIBS) $(CCFLAGS)

$(OUTLIB1) : $(OBJS)
	$(info Linking Dynamic $@ ...)
	@$(CC) $^ -o $@ $(INCLUDES) $(LIBS) $(CCFLAGS) -shared

$(OUTLIB2) : $(OBJS)
	$(info Linking Static $@ ...)
	ar cru $(OUTLIB2) $(OBJS)
	ranlib $(OUTLIB2)

$(ALL_OBJS) : $(OBJ_DIR)/%.o : %.cpp
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
