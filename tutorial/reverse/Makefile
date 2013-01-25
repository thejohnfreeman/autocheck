SRCDIR := .
SOURCES := \
	reverse.cpp

OBJDIR := build
OBJECTS := $(SOURCES:%.cpp=$(OBJDIR)/$(SRCDIR)/%.o)

INSTALLDIR := bin
INSTALLDIR := $(abspath $(INSTALLDIR))

EXE := reverse
EXE := $(INSTALLDIR)/$(EXE)

INCPATHS := $(HOME)/include

LIBPATHS :=
LIBS :=


## default target

.PHONY : all

all : $(EXE)


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

LXXFLAGS += -stdlib=libc++ 
LXXFLAGS += $(foreach LIBPATH, $(LIBPATHS), -L$(LIBPATH))
LXXFLAGS += $(foreach LIB, $(LIBS), -l$(LIB))


## components

$(OBJECTS) : $(OBJDIR)/$(SRCDIR)/%.o : $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(call dep_cxx,$@,$<)
	$(CXX) $(CXXFLAGS) -o $@ -c $<


## target

$(EXE) : $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(LXXFLAGS) -o $@ $^


## testing

.PHONY : test

test : all
	$(abspath $(EXE))


## cleaning

.PHONY : clean

clean :
	rm -rf $(OBJDIR)
	rm -rf $(INSTALLDIR)

