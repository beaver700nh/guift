# guift
GUI for TFT: An Arduino library to build GUIs on TFT LCD displays compatible with MCUFRIEND_kbv.

## AVR CMake build

This repository now includes an AVR-oriented CMake setup that:

- Scans `src/` with `CONFIGURE_DEPENDS` so source additions/removals are picked up.
- Builds one ELF output.
- Builds each configured library directory as a separate static library target.
- Links all static libraries into the final ELF.

### Configure

```bash
cmake -S . -B build \
  -DCMAKE_TOOLCHAIN_FILE=cmake/avr-toolchain.cmake \
  -DAVR_TOOLCHAIN_BIN_DIR=/path/to/avr/bin \
  -DAVR_MCU=atmega328p \
  -DAVR_F_CPU=16000000UL \
  -DAVR_LIBRARY_SOURCE_DIRS="/abs/path/lib1;/abs/path/lib2" \
  -DAVR_CUSTOM_INCLUDE_DIRS="/abs/path/include;/abs/path/other/include" \
  -DAVR_COMMON_COMPILE_OPTIONS="-ffunction-sections;-fdata-sections" \
  -DAVR_C_COMPILE_OPTIONS="" \
  -DAVR_CXX_COMPILE_OPTIONS="-fno-exceptions;-fno-rtti" \
  -DAVR_LINK_OPTIONS="-Wl,--gc-sections"
```

Then build:

```bash
cmake --build build
```

### Key variables

- Tool locations (`cmake/avr-toolchain.cmake`):
  - `AVR_TOOLCHAIN_BIN_DIR`
  - `AVR_C_COMPILER`, `AVR_CXX_COMPILER`, `AVR_ASM_COMPILER`
  - `AVR_AR`, `AVR_RANLIB`, `AVR_OBJCOPY`, `AVR_SIZE`
- Build configuration (`CMakeLists.txt`):
  - `AVR_TARGET_NAME`, `AVR_SRC_DIR`, `AVR_MCU`, `AVR_F_CPU`
  - `AVR_CUSTOM_INCLUDE_DIRS`
  - `AVR_LIBRARY_SOURCE_DIRS`, `AVR_LIBRARY_INCLUDE_DIRS`, `AVR_EXTRA_LIB_TARGETS`
  - `AVR_COMMON_COMPILE_OPTIONS`, `AVR_C_COMPILE_OPTIONS`, `AVR_CXX_COMPILE_OPTIONS`, `AVR_LINK_OPTIONS`
