This project is meant to be the building blocks to create WebGPU projects using C++ for the web.
It's barebones, it's only dependency being your C++ compiler (Clang), it doesn't make use of the C++ STL, nor does it have a memory allocator (malloc, free, new, delete), the only way to manage your memory is using the provided memory arena implementation to manipulate and write to the pre-allocated heap, but it should suffice for 99% of small games and renders.
WebGPU features are implemented as I create more examples, so it could take a while (if ever) to complete the entire specification.
Keep in mind this is not a library.
Don't make a pull request that introduces a dependency.

## Dependencies
* Clang

## Build

```make all```

## License
MIT License
Copyright (c) 2024 Imanol Fotia

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.