# GIFLIB - CMake Build Instructions

This is a CMake build configuration for GIFLIB that supports MSVC and other compilers.

## Building with CMake

### Prerequisites
- CMake 3.15 or higher
- A C compiler (MSVC, GCC, Clang, etc.)

### Build Steps

1. Create a build directory:
```bash
mkdir build
cd build
```

2. Configure the project:
```bash
cmake ..
```

Available CMake options:
- `BUILD_STATIC_LIBS=ON/OFF` - Build static libraries (default: ON)
- `BUILD_UTILS=ON/OFF` - Build utility programs (default: ON)

3. Build the project:
```bash
cmake --build . --config Release
```

4. Install (optional):
```bash
cmake --install . --config Release
```

### Using GIFLIB in Your CMake Project

After installing GIFLIB, you can use it in your CMake project:

```cmake
find_package(GIFLIB REQUIRED)
target_link_libraries(your_target PRIVATE GIFLIB::gif)
```

## Project Structure

- `lib/` - Core GIF library
- `util/` - Utility programs
  - gif2rgb - Convert GIF to RGB format
  - gifbuild - Build a GIF from text data
  - gifbg - Generate test-pattern GIFs
  - gifclrmp - Modify GIF colormap
  - gifcolor - Generate color test patterns
  - gifecho - Generate GIF from text
  - giffilter - Filter a GIF through stdin/stdout
  - giffix - Attempt to fix corrupt GIFs
  - gifhisto - Generate color histogram
  - gifinto - Save stdin to a GIF
  - gifsponge - Filter a GIF through stdin/stdout
  - giftext - Print GIF info
  - giftool - GIF manipulation tool
  - gifwedge - Create a test-pattern GIF 