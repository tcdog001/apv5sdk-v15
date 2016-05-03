/*************************************************************************
Copyright (c) 2013-2015, Autelan Networks. All rights reserved.

This Software is the property of Autelan Networks. 
*************************************************************************/

/*
 * File version info: 
 *
 */
#ifndef __AUTEOS_IO_ON_STREAMS_H__
#define __AUTEOS_IO_ON_STREAMS_H__
/******************************************************************************/
/*
* section 1:include glibc header files
*/
#include "auteos_base.h"
#include "io_on_streams_glibc.h"
/******************************************************************************/
/*
* section 2:autelan api
*/
#if autelan_used_file64
extern FILE *   autelan_fopen64 (const char *filename, const char *opentype);
extern FILE *   autelan_freopen64 (const char *filename, const char *opentype, FILE *stream);
extern int      autelan_fseeko64 (FILE *stream, off64_t offset, int whence);
extern off64_t  autelan_ftello64 (FILE *stream);
extern int      autelan_fgetpos64 (FILE *stream, fpos64_t *position);
extern int      autelan_fsetpos64 (FILE *stream, const fpos64_t *position);
#endif

extern FILE *   autelan_fopen (const char *filename, const char *opentype);
extern FILE *   autelan_freopen (const char *filename, const char *opentype, FILE *stream);
extern int      autelan___freadable (FILE *stream);
extern int      autelan___fwritable (FILE *stream);
extern int      autelan___freading (FILE *stream);
extern int      autelan___fwriting (FILE *stream);
extern int      autelan_fclose (FILE *stream);
extern void     autelan_flockfile (FILE *stream);
extern int      autelan_ftrylockfile (FILE *stream);
extern void     autelan_funlockfile (FILE *stream);
extern int      autelan___fsetlocking (FILE *stream, int type);
extern int      autelan_fputc (int c, FILE *stream);
extern int      autelan_putc (int c, FILE *stream);
extern int      autelan_putchar (int c);
extern int      autelan_fputs (const char *s, FILE *stream);
extern int      autelan_puts (const char *s);
extern int      autelan_putw (int w, FILE *stream);
extern int      autelan_fgetc (FILE *stream);
extern int      autelan_getc (FILE *stream);
extern int      autelan_getchar (void);
extern int      autelan_getw (FILE *stream);
extern char *   autelan_fgets (char *s, int count, FILE *stream);
extern char *   autelan_gets (char *s);
extern int      autelan_ungetc (int c, FILE *stream);
extern size_t   autelan_fread (void *data, size_t size, size_t count, FILE *stream);
extern size_t   autelan_fwrite (const void *data, size_t size, size_t count, FILE *stream);
extern int      autelan_printf (const char *template, ...);
extern int      autelan_fprintf (FILE *stream, const char *template, ...);
extern int      autelan_sprintf (char *s, const char *template, ...);
extern int      autelan_snprintf (char *s, size_t size, const char *template, ...);
extern int      autelan_asprintf (char **ptr, const char *template, ...);
#define autelan_vasprintf   vasprintf
extern int      autelan_vprintf (const char *template, va_list ap);
extern int      autelan_vfprintf (FILE *stream, const char *template, va_list ap);
extern int      autelan_vsprintf (char *s, const char *template, va_list ap);
extern int      autelan_vsnprintf (char *s, size_t size, const char *template, va_list ap);
extern size_t   autelan_parse_printf_format (const char *template, size_t n, int *argtypes);
extern int      autelan_register_printf_function (int spec, printf_function handler_function, printf_arginfo_function arginfo_function);
extern int      autelan_scanf (const char *template, ...);
#define autelan_vscanf      vscanf
extern int      autelan_fscanf (FILE *stream, const char *template, ...);
#define autelan_vfscanf     vfscanf
extern int      autelan_sscanf (const char *s, const char *template, ...);
#define autelan_vsscanf     vsscanf
extern int      autelan_feof (FILE *stream);
extern int      autelan_ferror (FILE *stream);
extern void     autelan_clearerr (FILE *stream);
extern long int autelan_ftell (FILE *stream);
extern int      autelan_fseek (FILE *stream, long int offset, int whence);
extern void     autelan_rewind (FILE *stream);
extern int      autelan_fgetpos (FILE *stream, fpos_t *position);
extern int      autelan_fsetpos (FILE *stream, const fpos_t *position);
extern int      autelan_fflush (FILE *stream);
extern void     autelan__flushlbf (void);
extern void     autelan___fpurge (FILE *stream);
extern int      autelan_setvbuf (FILE *stream, char *buf, int mode, size_t size);
extern void     autelan_setbuf (FILE *stream, char *buf);
extern void     autelan_setbuffer (FILE *stream, char *buf, size_t size);
extern void     autelan_setlinebuf (FILE *stream);
extern int      autelan___flbf (FILE *stream);
extern size_t   autelan___fbufsize (FILE *stream);
extern size_t   autelan___fpending (FILE *stream);
#if autelan_used_wchar
extern wint_t   autelan_fgetwc (FILE *stream);
extern wint_t   autelan_getwc (FILE *stream);
extern wint_t   autelan_getwchar (void);
extern wint_t   autelan_ungetwc (wint_t wc, FILE *stream);

extern wint_t   autelan_fputwc (wchar_t wc, FILE *stream);
extern wint_t   autelan_putwc (wchar_t wc, FILE *stream);
extern wint_t   autelan_putwchar (wchar_t wc);
extern int      autelan_fputws (const wchar_t *ws, FILE *stream);
extern wchar_t *autelan_fgetws (wchar_t *ws, int count, FILE *stream);

extern int      autelan_wprintf (const wchar_t *template, ...);
#define autelan_vwprintf    vwprintf

extern int      autelan_fwprintf (FILE *stream, const wchar_t *template, ...);
#define autelan_vfwprintf   vfwprintf

extern int      autelan_swprintf (wchar_t *s, size_t size, const wchar_t *template, ...);
#define autelan_vswprintf   vswprintf

extern int      autelan_wscanf (const wchar_t *template, ...);
#define autelan_vswscanf    vswscanf

extern int      autelan_fwscanf (FILE *stream, const wchar_t *template, ...);
#define autelan_vfwscanf    vfwscanf

extern int      autelan_swscanf (const wchar_t *ws, const char *template, ...);
#define autelan_vwscanf     vwscanf

#endif

/*
* section 3:forbidden all glibc api
*/
#if autelan_used_file64
#define fopen64         autelan_replaced(fopen64)
#define freopen64       autelan_replaced(freopen64)
#define ftello64        autelan_replaced(ftello64)
#define fseeko64        autelan_replaced(fseeko64)
#define fgetpos64       autelan_replaced(fgetpos64)
#define fsetpos64       autelan_replaced(fsetpos64)
#else
#define fopen64         autelan_forbidden(fopen64)
#define freopen64       autelan_forbidden(freopen64)
#define ftello64        autelan_forbidden(ftello64)
#define fseeko64        autelan_forbidden(fseeko64)
#define fgetpos64       autelan_forbidden(fgetpos64)
#define fsetpos64       autelan_forbidden(fsetpos64)
#endif

#define fopen           autelan_replaced(fopen)
#define freopen         autelan_replaced(freopen)
#define __freadable     autelan_replaced(__freadable)
#define __fwritable     autelan_replaced(__fwritable)
#define __freading      autelan_replaced(__freading)
#define __fwriting      autelan_replaced(__fwriting)
#define fclose          autelan_replaced(fclose)
#define fcloseall       autelan_forbidden(fcloseall)
#define flockfile       autelan_replaced(flockfile)
#define ftrylockfile    autelan_replaced(ftrylockfile)
#define funlockfile     autelan_replaced(funlockfile)
#define __fsetlocking   autelan_replaced(__fsetlocking)
#define fputs           autelan_replaced(fputs)
#define fputs_unlocked  autelan_forbidden(fputs_unlocked)
#define puts            autelan_replaced(puts)
#define putw            autelan_replaced(putw)
#define getw            autelan_replaced(getw)
#define getline         autelan_forbidden(getline)
#define getdelim        autelan_forbidden(getdelim)
#define fgets           autelan_replaced(fgets)
#define fgets_unlocked  autelan_forbidden(fgets_unlocked)
#define gets            autelan_replaced(gets)
#define ungetc          autelan_replaced(ungetc)
#define fread           autelan_replaced(fread)
#define fread_unlocked  autelan_forbidden(fread_unlocked)
#define fwrite          autelan_replaced(fwrite)
#define fwrite_unlocked autelan_forbidden(fwrite_unlocked)
#define printf          autelan_replaced(printf)
#define fprintf         autelan_replaced(fprintf)
#define sprintf         autelan_replaced(sprintf)
#define snprintf        autelan_replaced(snprintf)
#define asprintf        autelan_replaced(asprintf)
#define obstack_printf  autelan_forbidden(obstack_printf)
#define vprintf         autelan_replaced(vprintf)
#define vfprintf        autelan_replaced(vfprintf)
#define vsprintf        autelan_replaced(vsprintf)
#define vsnprintf       autelan_replaced(vsnprintf)
#define obstack_vprintf autelan_forbidden(obstack_vprintf)
#define parse_printf_format         autelan_replaced(parse_printf_format)
#define register_printf_function    autelan_replaced(register_printf_function)
#define printf_size                 autelan_forbidden(printf_size)
#define printf_size_info            autelan_forbidden(printf_size_info)
#define scanf           autelan_replaced(scanf)
#define fscanf          autelan_replaced(fscanf)
#define sscanf          autelan_replaced(sscanf)
#define vfscanf         autelan_replaced(vfscanf)
#define ftell           autelan_replaced(ftell)
#define ftello          autelan_forbidden(ftello)
#define fseek           autelan_replaced(fseek)
#define fseeko          autelan_forbidden(fseeko)
#define rewind          autelan_replaced(rewind)
#define fgetpos         autelan_replaced(fgetpos)
#define fsetpos         autelan_replaced(fsetpos)
#define fflush          autelan_replaced(fflush)
#define fflush_unlocked autelan_forbidden(fflush_unlocked)
#define _flushlbf       autelan_replaced(_flushlbf)
#define __fpurge        autelan_replaced(__fpurge)
#define setvbuf         autelan_replaced(setvbuf)
#define setbuf          autelan_replaced(setbuf)
#define setbuffer       autelan_replaced(setbuffer)
#define setlinebuf      autelan_replaced(setlinebuf)
#define __flbf          autelan_replaced(__flbf)
#define __fbufsize      autelan_replaced(__fbufsize)
#define __fpending      autelan_replaced(__fpending)
#define fmemopen        autelan_forbidden(fmemopen)
#define open_memstream  autelan_forbidden(open_memstream)
#define fopencookie     autelan_forbidden(fopencookie)
#define fmtmsg          autelan_forbidden(fmtmsg)
#define addseverity     autelan_forbidden(addseverity)

#if autelan_used_wchar

#define fgetwc          autelan_replaced(fgetwc)
#define getwc           autelan_replaced(getwc)
#define getwchar        autelan_replaced(getwchar)
#define ungetwc         autelan_replaced(ungetwc)
#define fputwc          autelan_replaced(fputwc)
#define putwc           autelan_replaced(putwc)
#define putwchar        autelan_replaced(putwchar)
#define fputws          autelan_replaced(fputws)
#define fgetws          autelan_replaced(fgetws)
#define wprintf         autelan_replaced(wprintf)
#define fwprintf        autelan_replaced(fwprintf)
#define swprintf        autelan_replaced(swprintf)
#define wscanf          autelan_replaced(wscanf)
#define fwscanf         autelan_replaced(fwscanf)
#define swscanf         autelan_replaced(swscanf)

#else

#define fgetwc          autelan_forbidden(fgetwc)
#define getwc           autelan_forbidden(getwc)
#define getwchar        autelan_forbidden(getwchar)
#define ungetwc         autelan_forbidden(ungetwc)
#define fputwc          autelan_forbidden(fputwc)
#define putwc           autelan_forbidden(putwc)
#define putwchar        autelan_forbidden(putwchar)
#define fputws          autelan_forbidden(fputws)
#define fgetws          autelan_forbidden(fgetws)
#define wprintf         autelan_forbidden(wprintf)
#define fwprintf        autelan_forbidden(fwprintf)
#define swprintf        autelan_forbidden(swprintf)
#define wscanf          autelan_forbidden(wscanf)
#define fwscanf         autelan_forbidden(fwscanf)
#define swscanf         autelan_forbidden(swscanf)

#endif

#if 0
#define getc            autelan_replaced(getc)
#define getc_unlocked   autelan_forbidden(getc_unlocked)
#define putc            autelan_replaced(putc)
#define putc_unlocked   autelan_forbidden(putc_unlocked)
#define getchar         autelan_replaced(getchar)
#define getchar_unlocked    autelan_forbidden(getchar_unlocked)
#define putchar         autelan_replaced(putchar)
#define putchar_unlocked    autelan_forbidden(putchar_unlocked)
#define fgetc           autelan_replaced(fgetc)
#define fgetc_unlocked  autelan_forbidden(fgetc_unlocked)
#define fputc           autelan_replaced(fputc)
#define fputc_unlocked  autelan_forbidden(fputc_unlocked)
#define feof            autelan_replaced(feof)
#define feof_unlocked   autelan_forbidden(feof_unlocked)
#define ferror          autelan_replaced(ferror)
#define ferror_unlocked autelan_forbidden(ferror_unlocked)
#define clearerr        autelan_replaced(clearerr)
#define clearerr_unlocked   autelan_forbidden(clearerr_unlocked)
#endif

/******************************************************************************/
#endif /* __AUTEOS_IO_ON_STREAMS_H__ */
