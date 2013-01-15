SRCDIR := lib
SOURCES := \

OBJDIR := build
OBJECTS := $(SOURCES:%.c=$(OBJDIR)/$(SRCDIR)/%.o)

TESTDIR := test
TESTS := \
	is_one_of \
	largest \
	value \
	apply \
	generator \
	arbitrary \
	check \

INSTALLDIR := bin
INSTALLDIR := $(abspath $(INSTALLDIR))

INCPATHS := $(HOME)/include ./include
LIBPATHS := $(HOME)/lib


## compiler

CXX := clang++

FLAGS         += $(foreach INCPATH, $(INCPATHS), -I$(INCPATH))
RELEASE_FLAGS := -O2 -DNDEBUG
DEBUG_FLAGS   := -O0 -g3

CXXFLAGS         += $(FLAGS)
CXXFLAGS         += -std=c++11
CXXFLAGS         += -stdlib=libc++ -U__STRICT_ANSI__
RELEASE_CXXFLAGS := $(RELEASE_FLAGS)
DEBUG_CXXFLAGS   := $(DEBUG_FLAGS)

LXXFLAGS += $(foreach LIBPATH, $(LIBPATHS), -L$(LIBPATH)) -stdlib=libc++ 


## default target

.PHONY : all debug release

all : debug

debug : CXXFLAGS += $(DEBUG_CXXFLAGS)
debug : test

release : CXXFLAGS += $(RELEASE_CXXFLAGS)
release : test


## dependencies

include $(shell find $(OBJDIR) -name '*.make' 2>/dev/null)

# $1 = target ($@)
# $2 = source ($<)
# $3 = extra flags
dep_cxx = \
	DEP=$1.make; \
	  $(CXX) $(CXXFLAGS) $3 -o $$DEP -MM $2; \
	  cat $$DEP | sed -e 's![^[:space:]]*[[:space:]]*:!$1 :!' \
	    -e 's!/usr[^[:space:]]*!!g' \
	    -e '/^[:space:]*\\$$/ d' \
	    > $$DEP.tmp && mv $$DEP.tmp $$DEP


## library

$(OBJECTS) : $(OBJDIR)/$(SRCDIR)/%.o : $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(call dep_cxx,$@,$<)
	$(CXX) $(CXXFLAGS) -o $@ -c $<


## tests

TESTOBJECTS  := $(TESTS:%=$(OBJDIR)/$(TESTDIR)/%.o)

.PHONY : test verbose

verbose : $(INSTALLDIR)/test
	GTEST_OUTPUT=verbose $(INSTALLDIR)/test

test : $(INSTALLDIR)/test
	$(INSTALLDIR)/test 2>/dev/null

$(INSTALLDIR)/test : LXXFLAGS += -lgtest -lgtest_main
$(INSTALLDIR)/test : $(TESTOBJECTS) $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CXX) $(LXXFLAGS) -o $@ $^

$(TESTOBJECTS) : $(OBJDIR)/$(TESTDIR)/%.o : $(TESTDIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(call dep_cxx,$@,$<,$(TESTCXXFLAGS))
	$(CXX) $(CXXFLAGS) -o $@ -c $<


## debug

.PHONY : gdb

GDB := ggdb -q

gdb : $(INSTALLDIR)/test
	$(GDB) $(INSTALLDIR)/test


## cleaning

.PHONY : clean

clean :
	rm -rf $(OBJDIR)
	rm -rf $(INSTALLDIR)

