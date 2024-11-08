CXX := g++
CXXWARNINGS := -Wall -Wextra -Werror -Wno-deprecated-copy
CXXOPT := -O2
CXXSTD := -std=c++20
INCLUDES := -I Core/include

PYTHIA8 := /Users/joachimcarlokristianhansen/PYTHIA/pythia8311
FASTJET := /Users/joachimcarlokristianhansen/alisw/sw/osx_arm64/fastjet/latest
PYINC := -I $(PYTHIA8)/include -I $(FASTJET)/include
INCLUDES += $(PYINC)
CXXFLAGS := $(CXXWARNINGS) $(CXXSTD) $(CXXOPT) $(INCLUDES)
LDFLAGS := -L$(PYTHIA8)/lib -lpythia8 -L$(FASTJET)/lib -lfastjet
LDFLAGS += -Wl,-rpath,$(PYTHIA8)/lib

CROOTFLAGS := $(shell root-config --cflags)
LROOTFLAGS := $(shell root-config --glibs)
CXXFLAGS += $(CROOTFLAGS)
LDFLAGS += $(LROOTFLAGS)

SRC_DIR := Core/src
SOURCES := Tasks/main.cxx $(wildcard $(SRC_DIR)/*.cxx)
OBJECTS := $(patsubst %.cxx, %.o, $(SOURCES)) CoreDict.o

TARGET_EXE := main
TARGET_LIB := libPythiaEvent.so
.PHONY: all clean

all: $(TARGET_EXE) $(TARGET_LIB)

$(TARGET_EXE): $(OBJECTS) $(TARGET_LIB)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET_LIB): $(SRC_DIR)/PythiaEvent.o CoreDict.o
	$(CXX) $(CXXFLAGS) -shared -fPIC -o $@ $(SRC_DIR)/PythiaEvent.o CoreDict.o $(LDFLAGS)
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cxx
	$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@

CoreDict.cxx: Core/include/PythiaEvent.h Core/include/CoreLinkDef.h
	rootcling -f CoreDict.cxx -c Core/include/PythiaEvent.h Core/include/CoreLinkDef.h

CoreDict.o: CoreDict.cxx
	$(CXX) $(CXXFLAGS) -fPIC -c CoreDict.cxx -o CoreDict.o

clean:
	rm -v $(SRC_DIR)/*.o Tasks/*.o $(TARGET_EXE) $(TARGET_LIB) CoreDict.cxx CoreDict_rdict.pcm CoreDict.o