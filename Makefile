#!/bin/sh

CXX:=clang++

LD_FLAGS:=--target=wasm32 --no-standard-libraries -Wl,--no-entry -fno-builtin
CXX_FLAGS:=-Wl,--export-all,--export=__heap_base -Wl,--allow-undefined -O2

all: test

test:
	$(CXX) $(LD_FLAGS) $(CXX_FLAGS) -o $@.wasm $@.cpp

