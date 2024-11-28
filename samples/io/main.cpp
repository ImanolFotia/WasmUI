#include <arena.hpp>
#include <string.hpp>

extern "C" FILE fopen(JsString);
extern "C" void fread(FILE, char*, size_t);

extern "C" int wasm_main() {
    FILE file = fopen("../../shaders/hello-triangle/fragment.wgsl");
    char buffer[256];
    for(int i = 0; i < 100; i++) {
    //memset(buffer, 0, 256);
        fread(file, buffer, 256);
    }
        puts(buffer);
    return 0;
}