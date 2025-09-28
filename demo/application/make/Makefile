project = test

PREFIX ?=
CC  = $(PREFIX)gcc
CXX = $(PREFIX)g++
ASM = $(PREFIX)gcc
CP  = $(PREFIX)objcopy
SZ  = $(PREFIX)size

out_dir ?= .
src_dir += . src

c_src   := $(wildcard $(foreach f, $(src_dir), $(f)/*.c))
cxx_src := $(wildcard $(foreach f, $(src_dir), $(f)/*.cpp))
asm_src := $(wildcard $(foreach f, $(src_dir), $(f)/*.s))

jobs := $(patsubst %.c, %.o, $(c_src))
jobs += $(patsubst %.cpp, %.o, $(cxx_src))
jobs += $(patsubst %.s, %.o, $(asm_src))

deps := $(wildcard $(foreach f, $(jobs), $(out_dir)/$(f).d))
prepare_path := $(sort $(dir $(foreach f, $(jobs), $(out_dir)/$(f).d)))

CFLAGS = \
	-Wall \
	-Werror	\
	-Iinc

CXXFLAGS = \
	-Wall \
	-Werror	\
	-Iinc

ASMFLAGS = \
	-x assembler-with-cpp \
	-Wall \
	-Werror	\
	-Iinc

LDFLAGS =

ifneq ($(cxx_src), )
LDFLAGS += \
	-lstdc++
endif

all: build_prepare $(project)
	@echo build success ......

$(project): $(foreach f, $(jobs), $(out_dir)/$(f))
	$(CC) $^ $(LDFLAGS) -o $@

-include $(deps)

$(out_dir)/%.o : %.c
	$(CC) -c $< $(CFLAGS) -o $@ -MD -MF $@.d

$(out_dir)/%.o : %.cpp
	$(CXX) -c $< $(CXXFLAGS) -o $@ -MD -MF $@.d

$(out_dir)/%.o : %.s
	$(ASM) -c $< $(ASMFLAGS) -o $@ -MD -MF $@.d

.PHONY : clean
ifeq ($(out_dir), .)
clean:
	rm -rf $(foreach f, $(jobs), $(out_dir)/$(f)) $(deps) $(project)
else
clean:
	rm -rf $(out_dir) $(project)
endif

.PHONY : build_prepare
build_prepare:
	@/bin/mkdir -p $(prepare_path)

