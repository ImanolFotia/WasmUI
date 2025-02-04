#!/bin/sh

CXX:=clang++

LD_FLAGS:=--target=wasm32 --no-standard-libraries -Wl,--no-entry -fno-builtin -Wl,-z,stack-size=8388608 -Wl,--initial-heap=268435456 #8MB stack, 256MB heap
CXX_FLAGS:=-Wl,--export-all,--export=__heap_base -Wl,--allow-undefined -Wl,--export-table -O2 -pthread --std=c++2c #-mbulk-memory

INCLUDE_DIRS:=-I./cpp -I./cpp/std -I/lib64/clang/18/include

SOURCE_FILES:=./cpp/std/string.cpp

.PHONY: all
all: hello-world hello-triangle hello-cube textured-cube io aoe2-random

hello-triangle: $(SOURCE_FILES)
	@echo "Compiling project:" $@
	@mkdir -p wasm/$@
	@$(CXX) $(LD_FLAGS) $(CXX_FLAGS) $(INCLUDE_DIRS) $(SOURCE_FILES) -o wasm/$@/$@.wasm samples/$@/main.cpp
	@cp wasm/$@/$@.wasm  samples/$@/$@.wasm

hello-cube: $(SOURCE_FILES)
	@echo "Compiling project:" $@
	@mkdir -p wasm/$@
	@$(CXX) $(LD_FLAGS) $(CXX_FLAGS) $(INCLUDE_DIRS) $(SOURCE_FILES) -o wasm/$@/$@.wasm samples/$@/main.cpp
	@cp wasm/$@/$@.wasm  samples/$@/$@.wasm

textured-cube: $(SOURCE_FILES)
	@echo "Compiling project:" $@
	@mkdir -p wasm/$@
	@$(CXX) $(LD_FLAGS) $(CXX_FLAGS) $(INCLUDE_DIRS) $(SOURCE_FILES) -o wasm/$@/$@.wasm samples/$@/main.cpp
	@cp wasm/$@/$@.wasm  samples/$@/$@.wasm

io: $(SOURCE_FILES)
	@echo "Compiling project:" $@
	@mkdir -p wasm/$@
	@$(CXX) $(LD_FLAGS) $(CXX_FLAGS) $(INCLUDE_DIRS) $(SOURCE_FILES) -o wasm/$@/$@.wasm samples/$@/main.cpp
	@cp wasm/$@/$@.wasm  samples/$@/$@.wasm

hello-world: $(SOURCE_FILES)
	@echo "Compiling project:" $@
	@mkdir -p wasm/$@
	@$(CXX) $(LD_FLAGS) $(CXX_FLAGS) $(INCLUDE_DIRS) $(SOURCE_FILES) -o wasm/$@/$@.wasm samples/$@/main.cpp
	@cp wasm/$@/$@.wasm  samples/$@/$@.wasm

aoe2-random: $(SOURCE_FILES)
	@echo "Compiling project:" $@
	@mkdir -p wasm/$@
	@$(CXX) $(LD_FLAGS) $(CXX_FLAGS) $(INCLUDE_DIRS) $(SOURCE_FILES) -o wasm/$@/$@.wasm samples/$@/main.cpp
	@cp wasm/$@/$@.wasm  samples/$@/$@.wasm

