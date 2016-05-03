/*************************************************************************
Copyright (c) 2013-2015, Autelan Networks. All rights reserved.

This Software is the property of Autelan Networks. 
*************************************************************************/
#include "io_on_streams_glibc.h"


#if autelan_used_file64

FILE * autelan_fopen64 (const char *filename, const char *opentype)
{
    return fopen64(filename, opentype);
}


FILE * autelan_freopen64 (const char *filename, const char *opentype, FILE *stream)
{
    return freopen64(filename, opentype, stream);
}

off64_t autelan_ftello64 (FILE *stream)
{
    return ftello64(stream);
}

int autelan_fseeko64 (FILE *stream, off64_t offset, int whence)
{
    return fseeko64(stream, offset, whence);
}

int autelan_fgetpos64 (FILE *stream, fpos64_t *position)
{
    return fgetpos64(stream, position);
}

int autelan_fsetpos64 (FILE *stream, const fpos64_t *position)
{
    return fsetpos64(stream, position);
}

#endif

FILE * autelan_fopen (const char *filename, const char *opentype)
{
    return fopen(filename, opentype);
}

FILE * autelan_freopen (const char *filename, const char *opentype, FILE *stream)
{
    return freopen(filename, opentype, stream);
}
int autelan___freadable (FILE *stream)
{
    return __freadable(stream);
}

int autelan___fwritable (FILE *stream)
{
    return __fwritable(stream);
}

int autelan___freading (FILE *stream)
{
    return __freading(stream);
}

int autelan___fwriting (FILE *stream)
{
    return __fwriting(stream);
}

int autelan_fclose (FILE *stream)
{
    return fclose(stream);
}

void autelan_flockfile (FILE *stream)
{
    flockfile(stream);
}

int autelan_ftrylockfile (FILE *stream)
{
    return ftrylockfile(stream);
}

void autelan_funlockfile (FILE *stream)
{
    return funlockfile(stream);
}

int autelan___fsetlocking (FILE *stream, int type)
{
    return __fsetlocking(stream, type);
}

int autelan_fputc (int c, FILE *stream)
{
    return fputc(c, stream);
}

int autelan_putc (int c, FILE *stream)
{
    return putc(c, stream);
}

int autelan_putchar (int c)
{
    return putchar(c);
}

int autelan_fputs (const char *s, FILE *stream)
{
    return fputs(s, stream);
}

int autelan_puts (const char *s)
{
    return puts(s);
}

int autelan_putw (int w, FILE *stream)
{
    return putw(w, stream);
}

int autelan_fgetc (FILE *stream)
{
    return fgetc(stream);
}

int autelan_getc (FILE *stream)
{
    return getc(stream);
}

int autelan_getchar (void)
{
    return getchar();
}

int autelan_getw (FILE *stream)
{
    return getw(stream);
}

char * autelan_fgets (char *s, int count, FILE *stream)
{
    return fgets(s, count, stream);
}

char * autelan_gets (char *s)
{
    return gets(s);
}

int autelan_ungetc (int c, FILE *stream)
{
    return ungetc(c, stream);
}

size_t autelan_fread (void *data, size_t size, size_t count, FILE *stream)
{
    return fread(data, size, count, stream);
}

size_t autelan_fwrite (const void *data, size_t size, size_t count, FILE *stream)
{
    return fwrite(data, size, count, stream);
}

int autelan_printf (const char *template, ...)
{
    va_list args;
    int ret;
    
    va_start(args, template);
    ret = vprintf(template, args);
    va_end(args);

    return ret;
}

int autelan_fprintf (FILE *stream, const char *template, ...)
{
    va_list args;
    int ret;
    
    va_start(args, template);
    ret = vfprintf(stream, template, args);
    va_end(args);

    return ret;
}

int autelan_sprintf (char *s, const char *template, ...)
{
    va_list args;
    int ret;
    
    va_start(args, template);
    ret = vsprintf(s, template, args);
    va_end(args);
    
    return ret;
}

int autelan_snprintf (char *s, size_t size, const char *template, ...)
{
    va_list args;
    int ret;
    
    va_start(args, template);
    ret = vsnprintf(s, size, template, args);
    va_end(args);
    
    return ret;
}

int autelan_vprintf (const char *template, va_list ap)
{
    return vprintf(template, ap);
}

int autelan_vfprintf (FILE *stream, const char *template, va_list ap)
{
    return vfprintf(stream, template, ap);
}

int autelan_vsprintf (char *s, const char *template, va_list ap)
{
    return vsprintf(s, template, ap);
}

int autelan_vsnprintf (char *s, size_t size, const char *template, va_list ap)
{
    return vsnprintf(s, size, template, ap);
}

size_t autelan_parse_printf_format (const char *template, size_t n, int *argtypes)
{
    return parse_printf_format(template, n, argtypes);
}

int autelan_register_printf_function (int spec, printf_function handler_function, printf_arginfo_function arginfo_function)
{
    return register_printf_function(spec, handler_function, arginfo_function);
}

int autelan_fscanf (FILE *stream, const char *template, ...)
{
    va_list args;
    int ret;

    va_start(args, template);
    ret = vfscanf(stream, template, args);
    va_end(args);

    return ret;
}

int autelan_sscanf (const char *s, const char *template, ...)
{
    va_list args;
    int ret;

    va_start(args, template);
    ret = vsscanf(s, template, args);
    va_end(args);

    return ret;
}

int autelan_feof (FILE *stream)
{
    return feof(stream);
}

int autelan_ferror (FILE *stream)
{
    return ferror(stream);
}

void autelan_clearerr (FILE *stream)
{
    clearerr(stream);
}

long int autelan_ftell (FILE *stream)
{
    return ftell(stream);
}

int autelan_fseek (FILE *stream, long int offset, int whence)
{
    return fseek(stream, offset, whence);
}


void autelan_rewind (FILE *stream)
{
    rewind(stream);
}

int autelan_fgetpos (FILE *stream, fpos_t *position)
{
    return fgetpos(stream, position);
}

int autelan_fsetpos (FILE *stream, const fpos_t *position)
{
    return fsetpos(stream, position);
}

int autelan_fflush (FILE *stream)
{
    return fflush(stream);
}

void autelan__flushlbf (void)
{
    _flushlbf();
}

void autelan___fpurge (FILE *stream)
{
    __fpurge(stream);
}

int autelan_setvbuf (FILE *stream, char *buf, int mode, size_t size)
{
    return setvbuf(stream, buf, mode, size);
}

void autelan_setbuf (FILE *stream, char *buf)
{
    setbuf(stream, buf);
}

void autelan_setbuffer (FILE *stream, char *buf, size_t size)
{
    setbuffer(stream, buf, size);
}

void autelan_setlinebuf (FILE *stream)
{
    setlinebuf(stream);
}

int autelan___flbf (FILE *stream)
{
    return __flbf(stream);
}

size_t autelan___fbufsize (FILE *stream)
{
    return __fbufsize(stream);
}

size_t autelan___fpending (FILE *stream)
{
    return __fpending(stream);
}

#if autelan_used_wchar

wint_t autelan_fgetwc (FILE *stream)
{
    return fgetwc(stream);
}

wint_t autelan_getwc (FILE *stream)
{
    return getwc(stream);
}

wint_t autelan_getwchar (void)
{
    return getwchar();
}

wint_t autelan_ungetwc (wint_t wc, FILE *stream)
{
    return ungetwc(wc, stream);
}

wint_t autelan_fputwc (wchar_t wc, FILE *stream)
{
    return fputwc(wc, stream);
}

wint_t autelan_putwc (wchar_t wc, FILE *stream)
{
    return putwc(wc, stream);
}

wint_t autelan_putwchar (wchar_t wc)
{
    return putwchar(wc);
}

int autelan_fputws (const wchar_t *ws, FILE *stream)
{
    return fputws(ws, stream);
}

wchar_t *autelan_fgetws (wchar_t *ws, int count, FILE *stream)
{
    return fgetws(ws, count, stream);
}

int autelan_wprintf (const wchar_t *template, ...)
{
    va_list args;
    int ret;
    
    va_start(args, template);
    ret = vwprintf(template, args);
    va_end(args);

    return ret;
}

int autelan_fwprintf (FILE *stream, const wchar_t *template, ...)
{
    va_list args;
    int ret;
    
    va_start(args, template);
    ret = vfwprintf(stream, template, args);
    va_end(args);

    return ret;
}

int autelan_swprintf (wchar_t *s, size_t size, const wchar_t *template, ...)
{
    va_list args;
    int ret;
    
    va_start(args, template);
    ret = vswprintf(s, size, template, args);
    va_end(args);

    return ret;
}

int autelan_wscanf (const wchar_t *template, ...)
{
    va_list args;
    int ret;
    
    va_start(args, template);
    ret = vwscanf(template, args);
    va_end(args);

    return ret;
}

int autelan_fwscanf (FILE *stream, const wchar_t *template, ...)
{
    va_list args;
    int ret;
    
    va_start(args, template);
    ret = vfwscanf(stream, template, args);
    va_end(args);

    return ret;
}

int autelan_swscanf (const wchar_t *ws, const char *template, ...)
{
    va_list args;
    int ret;
    
    va_start(args, template);
    ret = vswscanf(ws, template, args);
    va_end(args);
    
    return ret;
}

#endif
