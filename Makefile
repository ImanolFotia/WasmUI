#!/bin/sh

CXX:=clang++

LD_FLAGS:=--target=wasm32 --no-standard-libraries -Wl,--no-entry -fno-builtin
CXX_FLAGS:=-Wl,--export-all,--export=__heap_base -Wl,--allow-undefined -Wl,--export-table -O2 -Wall -Werror

INCLUDE_DIRS:=-I./cpp

all: hello-triangle

hello-triangle: 
	@mkdir -p wasm/$@
	$(CXX) $(LD_FLAGS) $(CXX_FLAGS) $(INCLUDE_DIRS) -o wasm/$@/$@.wasm samples/$@/main.cpp
	@cp wasm/$@/$@.wasm  samples/$@/$@.wasm

