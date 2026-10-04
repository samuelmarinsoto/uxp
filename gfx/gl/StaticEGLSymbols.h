/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef STATICEGLSYMBOLS_H_
#define STATICEGLSYMBOLS_H_

#ifdef MOZ_STATIC_EGL

#include <string.h>

#include "GLLibraryLoader.h"

// A static build cannot dlopen libEGL (musl's static dlopen is a stub), so
// the final binary links a static libEGL and EGL symbols bind at link time.
// Core EGL entry points are declared weak: they resolve to the linked
// implementation when one exists and to null otherwise, so a missing libEGL
// surfaces at EnsureInitialized time instead of failing the link.

#define MOZ_STATIC_EGL_WEAK(name) extern "C" void egl##name() __attribute__((weak))

MOZ_STATIC_EGL_WEAK(GetDisplay);
MOZ_STATIC_EGL_WEAK(Terminate);
MOZ_STATIC_EGL_WEAK(GetCurrentSurface);
MOZ_STATIC_EGL_WEAK(GetCurrentContext);
MOZ_STATIC_EGL_WEAK(MakeCurrent);
MOZ_STATIC_EGL_WEAK(DestroyContext);
MOZ_STATIC_EGL_WEAK(CreateContext);
MOZ_STATIC_EGL_WEAK(DestroySurface);
MOZ_STATIC_EGL_WEAK(CreateWindowSurface);
MOZ_STATIC_EGL_WEAK(CreatePbufferSurface);
MOZ_STATIC_EGL_WEAK(CreatePixmapSurface);
MOZ_STATIC_EGL_WEAK(BindAPI);
MOZ_STATIC_EGL_WEAK(Initialize);
MOZ_STATIC_EGL_WEAK(ChooseConfig);
MOZ_STATIC_EGL_WEAK(GetError);
MOZ_STATIC_EGL_WEAK(GetConfigs);
MOZ_STATIC_EGL_WEAK(GetConfigAttrib);
MOZ_STATIC_EGL_WEAK(WaitNative);
MOZ_STATIC_EGL_WEAK(SwapBuffers);
MOZ_STATIC_EGL_WEAK(CopyBuffers);
MOZ_STATIC_EGL_WEAK(QueryString);
MOZ_STATIC_EGL_WEAK(QueryContext);
MOZ_STATIC_EGL_WEAK(BindTexImage);
MOZ_STATIC_EGL_WEAK(ReleaseTexImage);
MOZ_STATIC_EGL_WEAK(QuerySurface);

// eglGetProcAddress needs a real prototype because it is also called
// directly; EGL 1.5 requires it to serve core entry points too, which
// covers everything not listed above (all extensions).
extern "C" void (*eglGetProcAddress(const char* procname))() __attribute__((weak));

struct StaticEGLSymbol
{
    const char* name;
    PRFuncPtr ptr;
};

static const StaticEGLSymbol sStaticEGLSymbols[] = {
#define MOZ_STATIC_EGL_SYM(name) { "egl" #name, reinterpret_cast<PRFuncPtr>(&egl##name) }
    MOZ_STATIC_EGL_SYM(GetDisplay),
    MOZ_STATIC_EGL_SYM(Terminate),
    MOZ_STATIC_EGL_SYM(GetCurrentSurface),
    MOZ_STATIC_EGL_SYM(GetCurrentContext),
    MOZ_STATIC_EGL_SYM(MakeCurrent),
    MOZ_STATIC_EGL_SYM(DestroyContext),
    MOZ_STATIC_EGL_SYM(CreateContext),
    MOZ_STATIC_EGL_SYM(DestroySurface),
    MOZ_STATIC_EGL_SYM(CreateWindowSurface),
    MOZ_STATIC_EGL_SYM(CreatePbufferSurface),
    MOZ_STATIC_EGL_SYM(CreatePixmapSurface),
    MOZ_STATIC_EGL_SYM(BindAPI),
    MOZ_STATIC_EGL_SYM(Initialize),
    MOZ_STATIC_EGL_SYM(ChooseConfig),
    MOZ_STATIC_EGL_SYM(GetError),
    MOZ_STATIC_EGL_SYM(GetConfigs),
    MOZ_STATIC_EGL_SYM(GetConfigAttrib),
    MOZ_STATIC_EGL_SYM(WaitNative),
    MOZ_STATIC_EGL_SYM(SwapBuffers),
    MOZ_STATIC_EGL_SYM(CopyBuffers),
    MOZ_STATIC_EGL_SYM(QueryString),
    MOZ_STATIC_EGL_SYM(QueryContext),
    MOZ_STATIC_EGL_SYM(BindTexImage),
    MOZ_STATIC_EGL_SYM(ReleaseTexImage),
    MOZ_STATIC_EGL_SYM(QuerySurface),
    MOZ_STATIC_EGL_SYM(GetProcAddress)
};

static inline bool
FindStaticEGLSymbol(const char* name, PRFuncPtr* out)
{
    for (size_t i = 0; i < sizeof(sStaticEGLSymbols) / sizeof(sStaticEGLSymbols[0]); ++i) {
        if (strcmp(sStaticEGLSymbols[i].name, name) == 0) {
            *out = sStaticEGLSymbols[i].ptr;
            return true;
        }
    }
    return false;
}

#endif // MOZ_STATIC_EGL

#endif // STATICEGLSYMBOLS_H_
