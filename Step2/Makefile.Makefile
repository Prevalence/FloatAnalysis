CXX = clang-3.4
LLVM_PATH=/usr/lib
LLVM_CONFIG = $(LLVM_PATH)/llvm-config
LLVM_CXXFLAGS += $(shell $(LLVM_CONFIG) --cxxflags)
LLVM_LDFLAGS := $(shell $(LLVM_CONFIG) --ldflags) 
LLVM_LIBS = $(shell $(LLVM_CONFIG) --libs --system-libs)
CLANG_LIBS = \
  -lclangFrontendTool -lclangFrontend -lclangDriver \
  -lclangSerialization -lclangCodeGen -lclangParse \
  -lclangSema -lclangStaticAnalyzerFrontend \
  -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore \
  -lclangAnalysis -lclangARCMigrate -lclangRewrite \
  -lclangEdit -lclangTooling -lclangASTMatchers -lclangAST -lclangLex -lclangBasic

TARGETS=transform traverse matcher
BINFILES=$(addprefix bin/,$(TARGETS))

.PHONY: all $(TARGETS) clean

all: $(TARGETS)

$(TARGETS): %: bin/%

$(BINFILES): bin/%: src/%.cpp
	$(CXX) $^ $(LLVM_CXXFLAGS) $(CLANG_LIBS) $(LLVM_LIBS) $(LLVM_LDFLAGS) -o $@
	
clean:
	rm -f $(BINFILES)
