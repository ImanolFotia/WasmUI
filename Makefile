#!/bin/sh

CXX:=clang++

LD_FLAGS:=--target=wasm32 --no-standard-libraries -Wl,--no-entry -fno-builtin -Wl,-z,stack-size=8388608 -Wl,--initial-heap=268435456 #8MB stack, 256MB heap
CXX_FLAGS:=-Wl,--export-all,--export=__heap_base -Wl,--allow-undefined -Wl,--export-table -O2 -pthread #-mbulk-memory

INCLUDE_DIRS:=-I./cpp -I/lib64/clang/18/include

SOURCE_FILES:=./cpp/string.cpp


all: hello-triangle hello-cube io

hello-triangle: 
	@mkdir -p wasm/$@
	$(CXX) $(LD_FLAGS) $(CXX_FLAGS) $(INCLUDE_DIRS) $(SOURCE_FILES) -o wasm/$@/$@.wasm samples/$@/main.cpp
	#@cp wasm/$@/$@.wasm  samples/$@/$@.wasm

hello-cube: 
	@mkdir -p wasm/$@
	$(CXX) $(LD_FLAGS) $(CXX_FLAGS) $(INCLUDE_DIRS) $(SOURCE_FILES) -o wasm/$@/$@.wasm samples/$@/main.cpp
	#@cp wasm/$@/$@.wasm  samples/$@/$@.wasm

io: 
	@mkdir -p wasm/$@
	$(CXX) $(LD_FLAGS) $(CXX_FLAGS) $(INCLUDE_DIRS) $(SOURCE_FILES) -o wasm/$@/$@.wasm samples/$@/main.cpp
	#@cp wasm/$@/$@.wasm  samples/$@/$@.wasm

