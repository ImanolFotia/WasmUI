#!/bin/sh

CXX:=clang++

LD_FLAGS:=--target=wasm32 --no-standard-libraries -Wl,--no-entry -fno-builtin -Wl,-z,stack-size=8388608 -Wl,--initial-heap=268435456 #8MB stack, 256MB heap
CXX_FLAGS:=-Wl,--export-all,--export=__heap_base -Wl,--allow-undefined -Wl,--export-table -O2 -mbulk-memory

INCLUDE_DIRS:=-I./cpp

all: hello-triangle hello-cube

hello-triangle: 
	@mkdir -p wasm/$@
	$(CXX) $(LD_FLAGS) $(CXX_FLAGS) $(INCLUDE_DIRS) -o wasm/$@/$@.wasm samples/$@/main.cpp
	@cp wasm/$@/$@.wasm  samples/$@/$@.wasm

hello-cube: 
	@mkdir -p wasm/$@
	$(CXX) $(LD_FLAGS) $(CXX_FLAGS) $(INCLUDE_DIRS) -o wasm/$@/$@.wasm samples/$@/main.cpp
	@cp wasm/$@/$@.wasm  samples/$@/$@.wasm

