/*-------------------------------------------------------------------------
 *
 * unicode.h
 *	  unicode and wide chars routines
 *
 * Portions Copyright (c) 2017-2018 Pavel Stehule
 *
 * IDENTIFICATION
 *	  src/unicode.h
 *
 *-------------------------------------------------------------------------
 */

#ifndef PSPG_UNICODE_H
#define PSPG_UNICODE_H

#include <stdbool.h>
#include <stdlib.h>

extern size_t utf8len(char *s);
extern size_t utf8len_start_stop(const char *start, const char *stop);
extern int utf8charlen(char ch);
extern int utf_dsplen(const char *s);
extern int utf_string_dsplen(const char *s, size_t max_bytes);
extern int readline_utf_string_dsplen(const char *s, size_t max_bytes, size_t offset);
extern const char *utf8_nstrstr(const char *haystack, const char *needle);
extern const char *utf8_nstrstr_ignore_lower_case(const char *haystack, const char *needle);
extern bool utf8_isupper(const char *s);
extern unsigned char *unicode_to_utf8(wchar_t c, unsigned char *utf8string, int *size);
extern int utf8_tofold(const char *s);

#endif
