#include <errno.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>

#include "adapter-register.h"

/*
 * redirection for glibc's stdio pointers, which is defined as:
 *   struct _IO_FILE_plus _IO_2_1_stdin_;
 *   struct _IO_FILE_plus _IO_2_1_stdout_;
 *   struct _IO_FILE_plus _IO_2_1_stderr_;
 *   FILE *stdin = (FILE *) &_IO_2_1_stdin_;
 *   FILE *stdout = (FILE *) &_IO_2_1_stdout_;
 *   FILE *stderr = (FILE *) &_IO_2_1_stderr_;
 *   the size of _IO_FILE_plus is 224.
 *   Those pointers are not getted by fopen, so translate those
 * pointers to valid bionic ones.
 */

// the data only is placeholder
static const char adapt__IO_2_1_stdin_[224] = {0};
static const char adapt__IO_2_1_stdout_[sizeof(adapt__IO_2_1_stdin_)] = {0};
static const char adapt__IO_2_1_stderr_[sizeof(adapt__IO_2_1_stdin_)] = {0};
static FILE *adapt_stdio_handle(FILE *fp) {
  const char *c_fp = (const char *)fp;
  if (c_fp == adapt__IO_2_1_stdin_) {
    return stdin;
  } else if (c_fp == adapt__IO_2_1_stdout_) {
    return stdout;
  } else if (c_fp == adapt__IO_2_1_stderr_) {
    return stderr;
  }
  return fp;
}

static FILE *adapt_fopen(const char *pathname, const char *mode) {
  FILE *fp = fopen(pathname, mode);
  adapter_log("fopen filename %s, mode %s, fp %p", pathname, mode, fp);
  return fp;
}

static FILE *adapt_fdopen(int fd, const char *mode) {
  FILE *fp = fdopen(fd, mode);
  adapter_log("fdopen fd %d, mode %s, fp %p", fd, mode, fp);
  return fp;
}

static FILE *adapt_popen(const char *command, const char *type) {
  FILE *fp = popen(command, type);
  adapter_log("fopen command %s, type %s, fp %p", command, type, fp);
  return fp;
}

static void adapt_clearerr(FILE *fp) { clearerr(adapt_stdio_handle(fp)); }

static int adapt_fclose(FILE *fp) {
  adapter_log("fclose fp %p", fp);
  return fclose(adapt_stdio_handle(fp));
}

static int adapt_feof(FILE *fp) { return feof(adapt_stdio_handle(fp)); }

static int adapt_ferror(FILE *fp) { return ferror(adapt_stdio_handle(fp)); }

static int adapt_fflush(FILE *fp) {
  adapter_log("fflush fp %p", fp);
  if (fileno(adapt_stdio_handle(fp)) < 0) {
    return 0;
  }
  return fflush(adapt_stdio_handle(fp));
}

static int adapt_fgetc(FILE *fp) { return fgetc(adapt_stdio_handle(fp)); }

static int adapt_fgetpos(FILE *fp, fpos_t *pos) {
  adapter_log("fgetpos fp %p pos %p", fp, pos);
  return fgetpos(adapt_stdio_handle(fp), pos);
}

static int adapt_fgetpos64(FILE *fp, fpos64_t *pos) {
  adapter_log("fgetpos64 fp %p pos %p", fp, pos);
  return fgetpos64(adapt_stdio_handle(fp), pos);
}

static char *adapt_fgets(char *s, int n, FILE *fp) {
  adapter_log("fgets s %s n %d fp %p", s, n, fp);
  return fgets(s, n, adapt_stdio_handle(fp));
}

static int adapt_fprintf(FILE *fp, const char *fmt, ...) {
  int ret = 0;
  va_list args;
  va_start(args, fmt);
  ret = vfprintf(adapt_stdio_handle(fp), fmt, args);
  va_end(args);
  return ret;
}

static int adapt_fputc(int c, FILE *fp) {
  return fputc(c, adapt_stdio_handle(fp));
}

static int adapt_fputs(const char *s, FILE *fp) {
  adapter_log("fputs s '%s' fp %p", s, fp);
  return fputs(s, adapt_stdio_handle(fp));
}

static size_t adapt_fread(void *ptr, size_t size, size_t nmemb, FILE *fp) {
  adapter_log("fread ptr %p size %zu nmemb %zu fp %p", ptr, size, nmemb, fp);
  return fread(ptr, size, nmemb, adapt_stdio_handle(fp));
}

static FILE *adapt_freopen(const char *filename, const char *mode, FILE *fp) {
  adapter_log("freopen filename '%s' mode '%s' fp %p", filename, mode, fp);
  return freopen(filename, mode, adapt_stdio_handle(fp));
}

static FILE *adapt_freopen64(const char *filename, const char *mode, FILE *fp) {
  adapter_log("freopen64 filename '%s' mode '%s' fp %p", filename, mode, fp);
  return freopen64(filename, mode, adapt_stdio_handle(fp));
}

static int adapt_fscanf(FILE *fp, const char *fmt, ...) {
  int ret = 0;
  adapter_log("fscanf fp %p fmt '%s'", fp, fmt);
  va_list args;
  va_start(args, fmt);
  ret = vfscanf(adapt_stdio_handle(fp), fmt, args);
  va_end(args);
  return ret;
}

static int adapt_fseek(FILE *fp, long offset, int whence) {
  adapter_log("fseek fp %p offset %ld whence %d", fp, offset, whence);
  return fseek(adapt_stdio_handle(fp), offset, whence);
}

static int adapt_fseeko(FILE *fp, off_t offset, int whence) {
  adapter_log("fseeko fp %p offset %ld whence %d", fp, offset, whence);
  return fseeko(adapt_stdio_handle(fp), offset, whence);
}

static int adapt_fseeko64(FILE *fp, off64_t offset, int whence) {
  adapter_log("fseeko64 fp %p offset %ld whence %d", fp, offset, whence);
  return fseeko64(adapt_stdio_handle(fp), offset, whence);
}

static int adapt_fsetpos(FILE *fp, const fpos_t *pos) {
  adapter_log("fsetpos fp %p pos %p", fp, pos);
  return fsetpos(adapt_stdio_handle(fp), pos);
}

static int adapt_fsetpos64(FILE *fp, const fpos64_t *pos) {
  adapter_log("fsetpos64 fp %p pos %p", fp, pos);
  return fsetpos64(adapt_stdio_handle(fp), pos);
}

static long adapt_ftell(FILE *fp) { return ftell(adapt_stdio_handle(fp)); }

static off_t adapt_ftello(FILE *fp) { return ftello(adapt_stdio_handle(fp)); }

static off_t adapt_ftello64(FILE *fp) {
  return ftello64(adapt_stdio_handle(fp));
}

static size_t adapt_fwrite(const void *ptr, size_t size, size_t nmemb,
                           FILE *fp) {
  adapter_log("fwrite ptr %p size %zu nmemb %zu fp %p", ptr, size, nmemb, fp);
  return fwrite(ptr, size, nmemb, adapt_stdio_handle(fp));
}

static int adapt_getc(FILE *fp) { return getc(adapt_stdio_handle(fp)); }

static ssize_t adapt_getdelim(char **lineptr, size_t *n, int delimiter,
                              FILE *fp) {
  return getdelim(lineptr, n, delimiter, adapt_stdio_handle(fp));
}

static ssize_t adapt_getline(char **lineptr, size_t *n, FILE *fp) {
  return getline(lineptr, n, adapt_stdio_handle(fp));
}

static int adapt_putc(int c, FILE *fp) {
  return putc(c, adapt_stdio_handle(fp));
}

static void adapt_rewind(FILE *fp) {
  adapter_log("rewind fp %p", fp);
  rewind(adapt_stdio_handle(fp));
}

static void adapt_setbuf(FILE *fp, char *buf) {
  setbuf(adapt_stdio_handle(fp), buf);
}

static int adapt_setvbuf(FILE *fp, char *buf, int mode, size_t size) {
  return setvbuf(adapt_stdio_handle(fp), buf, mode, size);
}

static int adapt_ungetc(int c, FILE *fp) {
  return ungetc(c, adapt_stdio_handle(fp));
}

static int adapt_vfprintf(FILE *fp, const char *fmt, va_list arg) {
  adapter_log("vfprintf fp %p fmt '%s'", fp, fmt);
  return vfprintf(adapt_stdio_handle(fp), fmt, arg);
}

static int adapt_vfscanf(FILE *fp, const char *fmt, va_list arg) {
  adapter_log("vfscanf fp %p fmt '%s'", fp, fmt);
  return vfscanf(adapt_stdio_handle(fp), fmt, arg);
}

static int adapt_fileno(FILE *fp) { return fileno(adapt_stdio_handle(fp)); }

static int adapt_pclose(FILE *fp) {
  adapter_log("pclose fp %p", fp);
  return pclose(adapt_stdio_handle(fp));
}

static void adapt_flockfile(FILE *fp) {
  adapter_log("flockfile fp %p", fp);
  flockfile(adapt_stdio_handle(fp));
}

static int adapt_ftrylockfile(FILE *fp) {
  adapter_log("ftrylockfile fp %p", fp);
  return ftrylockfile(adapt_stdio_handle(fp));
}

static void adapt_funlockfile(FILE *fp) {
  adapter_log("funlockfile fp %p", fp);
  funlockfile(adapt_stdio_handle(fp));
}

static void adapt_clearerr_unlocked(FILE *fp) {
  clearerr_unlocked(adapt_stdio_handle(fp));
}

static int adapt_feof_unlocked(FILE *fp) {
  return feof_unlocked(adapt_stdio_handle(fp));
}

static int adapt_ferror_unlocked(FILE *fp) {
  return ferror_unlocked(adapt_stdio_handle(fp));
}

static int adapt_getc_unlocked(FILE *fp) {
  return getc_unlocked(adapt_stdio_handle(fp));
}

static int adapt_putc_unlocked(int c, FILE *fp) {
  return putc_unlocked(c, adapt_stdio_handle(fp));
}

static char *adapt_fgetln(FILE *fp, size_t *len) {
  return fgetln(adapt_stdio_handle(fp), len);
}

static void adapt___fpurge(FILE *fp) { __fpurge(adapt_stdio_handle(fp)); }

static int adapt_getw(FILE *fp) {
  adapter_log("Not support getw, fp %p", fp);
  errno = ENOTSUP;
  return EOF;
}

static int adapt_putw(int w, FILE *fp) {
  (void)w;
  adapter_log("Not support putw,  fp %p", fp);
  errno = ENOTSUP;
  return EOF;
}

static void adapt_setbuffer(FILE *fp, char *buf, int size) {
  setbuffer(adapt_stdio_handle(fp), buf, size);
}

static void adapt_setlinebuf(FILE *fp) { setlinebuf(adapt_stdio_handle(fp)); }

#if defined(_GNU_SOURCE)

static int adapt_fflush_unlocked(FILE *fp) {
  adapter_log("fflush_unlocked fp %p", fp);
  if (fileno_unlocked(adapt_stdio_handle(fp)) < 0) {
    return 0;
  }
  return fflush_unlocked(adapt_stdio_handle(fp));
}

static int adapt_fileno_unlocked(FILE *fp) {
  return fileno_unlocked(adapt_stdio_handle(fp));
}

static int adapt_fgetc_unlocked(FILE *fp) {
  return fgetc_unlocked(adapt_stdio_handle(fp));
}

static char *adapt_fgets_unlocked(char *s, int n, FILE *fp) {
  return fgets_unlocked(s, n, adapt_stdio_handle(fp));
}

static int adapt_fputc_unlocked(int c, FILE *fp) {
  return fputc_unlocked(c, adapt_stdio_handle(fp));
}

static int adapt_fputs_unlocked(const char *s, FILE *fp) {
  return fputs_unlocked(s, adapt_stdio_handle(fp));
}

static size_t adapt_fread_unlocked(void *ptr, size_t size, size_t nmemb,
                                   FILE *fp) {
  return fread_unlocked(ptr, size, nmemb, adapt_stdio_handle(fp));
}

static size_t adapt_fwrite_unlocked(const void *ptr, size_t size, size_t nmemb,
                                    FILE *fp) {
  return fwrite_unlocked(ptr, size, nmemb, adapt_stdio_handle(fp));
}
#endif

static struct glibc_adapter_t stdio_adapters[] = {
    ADAPT_INDIRECT(_IO_2_1_stdin_),
    ADAPT_INDIRECT(_IO_2_1_stdout_),
    ADAPT_INDIRECT(_IO_2_1_stderr_),

    ADAPT_INDIRECT(fopen),
    ADAPT_INDIRECT(fdopen),
    ADAPT_INDIRECT(popen),
    ADAPT_DIRECT(puts),
    ADAPT_DIRECT(printf),
    ADAPT_DIRECT(sprintf),
    ADAPT_DIRECT(asprintf),
    ADAPT_DIRECT(vasprintf),
    ADAPT_DIRECT(snprintf),
    ADAPT_DIRECT(vsprintf),
    ADAPT_DIRECT(vsnprintf),
    ADAPT_INDIRECT(clearerr),
    ADAPT_INDIRECT(fclose),
    ADAPT_INDIRECT(feof),
    ADAPT_INDIRECT(ferror),
    ADAPT_INDIRECT(fflush),
    ADAPT_INDIRECT(fgetc),
    ADAPT_INDIRECT(fgetpos),
    ADAPT_INDIRECT(fgetpos64),
    ADAPT_INDIRECT(fgets),
    ADAPT_INDIRECT(fprintf),
    ADAPT_INDIRECT(fputc),
    ADAPT_INDIRECT(fputs),
    ADAPT_INDIRECT(fread),
    ADAPT_INDIRECT(freopen),
    ADAPT_INDIRECT(freopen64),
    ADAPT_INDIRECT(fscanf),
    ADAPT_INDIRECT(fseek),
    ADAPT_INDIRECT(fseeko),
    ADAPT_INDIRECT(fseeko64),
    ADAPT_INDIRECT(fsetpos),
    ADAPT_INDIRECT(fsetpos64),
    ADAPT_INDIRECT(ftell),
    ADAPT_INDIRECT(ftello),
    ADAPT_INDIRECT(ftello64),
    ADAPT_INDIRECT(fwrite),
    ADAPT_INDIRECT(getc),
    ADAPT_INDIRECT(getdelim),
    ADAPT_INDIRECT(getline),
    ADAPT_INDIRECT(putc),
    ADAPT_INDIRECT(rewind),
    ADAPT_INDIRECT(setbuf),
    ADAPT_INDIRECT(setvbuf),
    ADAPT_INDIRECT(ungetc),
    ADAPT_INDIRECT(vfprintf),
    ADAPT_INDIRECT(vfscanf),
    ADAPT_INDIRECT(fileno),
    ADAPT_INDIRECT(pclose),
    ADAPT_INDIRECT(flockfile),
    ADAPT_INDIRECT(ftrylockfile),
    ADAPT_INDIRECT(funlockfile),
    ADAPT_INDIRECT(clearerr_unlocked),
    ADAPT_INDIRECT(feof_unlocked),
    ADAPT_INDIRECT(ferror_unlocked),
    ADAPT_INDIRECT(fgetln),
    ADAPT_INDIRECT(getc_unlocked),
    ADAPT_INDIRECT(putc_unlocked),
    ADAPT_INDIRECT(__fpurge),
    ADAPT_INDIRECT(getw),
    ADAPT_INDIRECT(putw),
    ADAPT_INDIRECT(setbuffer),
    ADAPT_INDIRECT(setlinebuf),

#if defined(_GNU_SOURCE)
    ADAPT_INDIRECT(fflush_unlocked),
    ADAPT_INDIRECT(fgetc_unlocked),
    ADAPT_INDIRECT(fgets_unlocked),
    ADAPT_INDIRECT(fputc_unlocked),
    ADAPT_INDIRECT(fputs_unlocked),
    ADAPT_INDIRECT(fread_unlocked),
    ADAPT_INDIRECT(fwrite_unlocked),
    ADAPT_INDIRECT(fileno_unlocked),
#endif
};

void register_adapters_stdio() { REGISTER_ADAPTERS_BY_CLASS(stdio); }
