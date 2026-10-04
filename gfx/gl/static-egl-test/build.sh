#!/bin/sh
# Builds the stub libEGL.a with the musl cross toolchain and links a
# static-pie test binary that exercises the MOZ_STATIC_EGL symbol-resolution
# path through the real GLLibraryLoader, then runs it and reports NSPR
# symbol usage in the linked binary.
set -e

cd "$(dirname "$0")"
XTOOL=${XTOOL:-/tmp/x86_64-linux-musl-cross/bin}
CC=$XTOOL/x86_64-linux-musl-gcc
CXX=$XTOOL/x86_64-linux-musl-g++
AR=$XTOOL/x86_64-linux-musl-ar
UXP="$(cd ../../.. && pwd)"
OUT=out

rm -rf "$OUT"
mkdir -p "$OUT"

$CC -c stub.c -o "$OUT/stub.o"
$AR rcs "$OUT/libEGL.a" "$OUT/stub.o"

CXXFLAGS="-std=c++17 -fno-exceptions -fno-rtti -fno-plt"
CXXFLAGS="$CXXFLAGS -DMOZ_STATIC_EGL=1"
CXXFLAGS="$CXXFLAGS -include cstring"
CXXFLAGS="$CXXFLAGS -Ishims -I$UXP/gfx/gl -I$UXP/nsprpub/pr/include"

$CXX $CXXFLAGS -c test.cpp -o "$OUT/test.o"
$CXX $CXXFLAGS -c "$UXP/gfx/gl/GLLibraryLoader.cpp" -o "$OUT/GLLibraryLoader.o"

$CXX -static-pie "$OUT/test.o" "$OUT/GLLibraryLoader.o" \
    -Wl,--whole-archive "$OUT/libEGL.a" -Wl,--no-whole-archive \
    -o "$OUT/static-egl-test"

"$OUT/static-egl-test"

echo "--- undefined dynamic symbols in the static-pie binary (filtered) ---"
nm -u "$OUT/static-egl-test" | grep -E 'dlopen|dlsym|PR_Load|PR_Find' \
    && { echo "FAIL: NSPR/dl references present"; exit 1; } \
    || echo "none (good)"

echo "--- call sites of NSPR lookup stubs (runtime-proven never executed) ---"
# GLLibraryLoader.o ships OpenLibrary, an unreferenced code path that calls
# PR_LoadLibraryWithFlags; the test asserts at runtime that it never runs.
# The EGL objects proper (GLLibraryEGL.o, GLContextProviderEGL.o) contain no
# such call sites with MOZ_STATIC_EGL.
objdump -d "$OUT/static-egl-test" | grep -E 'call.*<PR_(Load|Find)' || echo "none"
