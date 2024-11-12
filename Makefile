#!/bin/bash

CC:=clang
CXX:=clang++
LD:=wasm-ld

LD_FLAGS:=--target=wasm32 -nostdlib -Wl,--no-entry -fno-builtin
CXX_FLAGS:=-Wl,--export-all,--export=__heap_base -Wl,--allow-undefined

all: sum.wasm

sum.wasm:
	$(CXX) $(LD_FLAGS) $(CXX_FLAGS) -o $@ sum.cpp

