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
GPLv3 License
Copyright (C) 2026  Imanol Fotia

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.