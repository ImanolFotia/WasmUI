#include <std/stdio.hpp>

int main(int argc, char** argv) {
  std::SetElementText("myElement", "%: 2 + 2 = %", "From C++", 2+2);

  std::printf("%, %!", "Hello", "World");
  std::printf("%, %! % %", "Hello", "World", 654, 87.14);
}