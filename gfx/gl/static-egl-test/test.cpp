/* Exercises the MOZ_STATIC_EGL symbol-resolution path against the stub
 * libEGL.a: real GLLibraryLoader::LoadSymbols, weak core symbols, and the
 * eglGetProcAddress fallback, with no PR lookup on the success path. */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "StaticEGLSymbols.h"
#include "GLLibraryLoader.h"

using mozilla::gl::GLLibraryLoader;

static int g_prLookups = 0;

extern "C" {

PRFuncPtr PR_FindFunctionSymbol(PRLibrary* lib, const char* name)
{
    ++g_prLookups;
    (void)lib;
    (void)name;
    return nullptr;
}

PRFuncPtr PR_FindFunctionSymbolAndLibrary(const char* name, PRLibrary** lib)
{
    ++g_prLookups;
    (void)name;
    if (lib) {
        *lib = nullptr;
    }
    return nullptr;
}

PRLibrary* PR_LoadLibraryWithFlags(PRLibSpec libSpec, PRIntn flags)
{
    ++g_prLookups;
    (void)libSpec;
    (void)flags;
    return nullptr;
}

}

void printf_stderr(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
}

static PRFuncPtr
TestStaticLookup(const char* name)
{
    PRFuncPtr ptr;
    if (FindStaticEGLSymbol(name, &ptr)) {
        return ptr;
    }
    return reinterpret_cast<PRFuncPtr>(eglGetProcAddress(name));
}

static const char* const kSymbolNames[] = {
    "eglGetDisplay", "eglTerminate", "eglGetCurrentSurface",
    "eglGetCurrentContext", "eglMakeCurrent", "eglDestroyContext",
    "eglCreateContext", "eglDestroySurface", "eglCreateWindowSurface",
    "eglCreatePbufferSurface", "eglCreatePixmapSurface", "eglBindAPI",
    "eglInitialize", "eglChooseConfig", "eglGetError", "eglGetConfigs",
    "eglGetConfigAttrib", "eglWaitNative", "eglSwapBuffers",
    "eglCopyBuffers", "eglQueryString", "eglQueryContext",
    "eglBindTexImage", "eglReleaseTexImage", "eglQuerySurface",
    "eglGetProcAddress"
};

int
main(void)
{
    const int count = sizeof(kSymbolNames) / sizeof(kSymbolNames[0]);
    PRFuncPtr slots[26] = { 0 };
    GLLibraryLoader::SymLoadStruct early[27];

    for (int i = 0; i < count; ++i) {
        early[i].symPointer = &slots[i];
        early[i].symNames[0] = kSymbolNames[i];
        early[i].symNames[1] = nullptr;
    }
    early[count].symPointer = nullptr;

    if (!GLLibraryLoader::LoadSymbols(nullptr, early, TestStaticLookup,
                                      nullptr, false)) {
        fprintf(stderr, "FAIL: LoadSymbols reported failures\n");
        return 1;
    }

    for (int i = 0; i < count; ++i) {
        if (slots[i] == nullptr) {
            fprintf(stderr, "FAIL: %s unresolved\n", kSymbolNames[i]);
            return 1;
        }
    }

    if (slots[0] != reinterpret_cast<PRFuncPtr>(&eglGetDisplay)) {
        fprintf(stderr, "FAIL: eglGetDisplay did not bind to the static libEGL\n");
        return 1;
    }
    if (slots[count - 1] != reinterpret_cast<PRFuncPtr>(&eglGetProcAddress)) {
        fprintf(stderr, "FAIL: eglGetProcAddress did not bind to the static libEGL\n");
        return 1;
    }

    if (TestStaticLookup("eglTestMagic") == nullptr) {
        fprintf(stderr, "FAIL: eglGetProcAddress fallback not exercised\n");
        return 1;
    }

    if (g_prLookups != 0) {
        fprintf(stderr, "FAIL: NSPR lookup path used %d times\n", g_prLookups);
        return 1;
    }

    PRFuncPtr bogusSlot = reinterpret_cast<PRFuncPtr>(0x1);
    GLLibraryLoader::SymLoadStruct bogus =
        { &bogusSlot, { "eglNoSuchSymbol", nullptr } };
    if (GLLibraryLoader::LoadSymbols(nullptr, &bogus, TestStaticLookup,
                                     nullptr, false)) {
        fprintf(stderr, "FAIL: unknown symbol resolved\n");
        return 1;
    }
    if (bogusSlot != nullptr) {
        fprintf(stderr, "FAIL: unknown symbol slot not cleared\n");
        return 1;
    }

    printf("static-egl-test: OK (%d EGL symbols bound statically, 0 NSPR lookups)\n",
           count);
    return 0;
}
