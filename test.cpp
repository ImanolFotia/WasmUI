#include "string.hpp"

extern "C" void print(JsString);

export auto sum(int a, int b) -> int { return a + b; }

export auto test() -> void { print("Calling console.log from C++!"); }

export auto hello() -> JsCString {
  return {(size_t) "Hello, world from C++!", 22};
}
