/* Stub libEGL for the static-binding proof: exports the core EGL entry
 * points gecko binds at link time and fails on every call. */
#include <string.h>
#include <stddef.h>

void *eglGetDisplay(void *display_id) { (void)display_id; return 0; }
unsigned int eglTerminate(void *dpy) { (void)dpy; return 0; }
void *eglGetCurrentSurface(int draw) { (void)draw; return 0; }
void *eglGetCurrentContext(void) { return 0; }
unsigned int eglMakeCurrent(void *dpy, void *draw, void *read, void *ctx)
    { (void)dpy; (void)draw; (void)read; (void)ctx; return 0; }
unsigned int eglDestroyContext(void *dpy, void *ctx) { (void)dpy; (void)ctx; return 0; }
void *eglCreateContext(void *dpy, void *config, void *share, const int *attribs)
    { (void)dpy; (void)config; (void)share; (void)attribs; return 0; }
unsigned int eglDestroySurface(void *dpy, void *surface) { (void)dpy; (void)surface; return 0; }
void *eglCreateWindowSurface(void *dpy, void *config, unsigned long win, const int *attribs)
    { (void)dpy; (void)config; (void)win; (void)attribs; return 0; }
void *eglCreatePbufferSurface(void *dpy, void *config, const int *attribs)
    { (void)dpy; (void)config; (void)attribs; return 0; }
void *eglCreatePixmapSurface(void *dpy, void *config, unsigned long pixmap, const int *attribs)
    { (void)dpy; (void)config; (void)pixmap; (void)attribs; return 0; }
unsigned int eglBindAPI(unsigned int api) { (void)api; return 0; }
unsigned int eglInitialize(void *dpy, int *major, int *minor)
    { (void)dpy; (void)major; (void)minor; return 0; }
unsigned int eglChooseConfig(void *dpy, const int *attribs, void **configs,
                             int config_size, int *num_config)
    { (void)dpy; (void)attribs; (void)configs; (void)config_size; (void)num_config; return 0; }
int eglGetError(void) { return 0x300E; }
unsigned int eglGetConfigs(void *dpy, void **configs, int config_size, int *num_config)
    { (void)dpy; (void)configs; (void)config_size; (void)num_config; return 0; }
unsigned int eglGetConfigAttrib(void *dpy, void *config, int attribute, int *value)
    { (void)dpy; (void)config; (void)attribute; (void)value; return 0; }
unsigned int eglWaitNative(int engine) { (void)engine; return 0; }
unsigned int eglSwapBuffers(void *dpy, void *surface) { (void)dpy; (void)surface; return 0; }
unsigned int eglCopyBuffers(void *dpy, void *surface, unsigned long target)
    { (void)dpy; (void)surface; (void)target; return 0; }
const char *eglQueryString(void *dpy, int name) { (void)dpy; (void)name; return 0; }
unsigned int eglQueryContext(void *dpy, void *ctx, int attribute, int *value)
    { (void)dpy; (void)ctx; (void)attribute; (void)value; return 0; }
unsigned int eglBindTexImage(void *dpy, void *surface, int buffer)
    { (void)dpy; (void)surface; (void)buffer; return 0; }
unsigned int eglReleaseTexImage(void *dpy, void *surface, int buffer)
    { (void)dpy; (void)surface; (void)buffer; return 0; }
unsigned int eglQuerySurface(void *dpy, void *surface, int attribute, int *value)
    { (void)dpy; (void)surface; (void)attribute; (void)value; return 0; }

static void sentinel_proc(void) {}

void (*eglGetProcAddress(const char *procname))(void)
{
    if (procname && strcmp(procname, "eglTestMagic") == 0) {
        return sentinel_proc;
    }
    return 0;
}
