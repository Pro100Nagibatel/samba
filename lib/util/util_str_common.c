/*
   Unix SMB/CIFS implementation.
   Samba utility functions

   Copyright (C) Andrew Tridgell 1992-2001
   Copyright (C) Simo Sorce      2001-2002
   Copyright (C) Martin Pool     2003
   Copyright (C) James Peach	 2005

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "includes.h"

/**
Do a case-insensitive, whitespace-ignoring string compare.
**/
_PUBLIC_ int strwicmp(const char *psz1, const char *psz2)
{
	/* if BOTH strings are NULL, return TRUE, if ONE is NULL return */
	/* appropriate value. */
	if (psz1 == psz2)
		return (0);
	else if (psz1 == NULL)
		return (-1);
	else if (psz2 == NULL)
		return (1);

	/* sync the strings on first non-whitespace */
	while (1) {
		while (isspace((int)*psz1))
			psz1++;
		while (isspace((int)*psz2))
			psz2++;
		if (toupper_ascii((unsigned char)*psz1) != toupper_ascii((unsigned char)*psz2)
		    || *psz1 == '\0'
		    || *psz2 == '\0')
			break;
		psz1++;
		psz2++;
	}
	return (*psz1 - *psz2);
}

_PUBLIC_ size_t ucs2_align(const void *base_ptr, const void *p, int flags)
{
	if (flags & (STR_NOALIGN|STR_ASCII))
		return 0;
	return PTR_DIFF(p, base_ptr) & 1;
}

/**
 String replace.
 NOTE: oldc and newc must be 7 bit characters
**/
void string_replace( char *s, char oldc, char newc )
{
	char *p;

	/* this is quite a common operation, so we want it to be
	   fast. We optimise for the ascii case, knowing that all our
	   supported multi-byte character sets are ascii-compatible
	   (ie. they match for the first 128 chars) */

	for (p = s; *p; p++) {
		if (*p & 0x80) /* mb string - slow path. */
			break;
		if (*p == oldc) {
			*p = newc;
		}
	}

	if (!*p)
		return;

	/* Slow (mb) path. */
#ifdef BROKEN_UNICODE_COMPOSE_CHARACTERS
	/* With compose characters we must restart from the beginning. JRA. */
	p = s;
#endif

	while (*p) {
		size_t c_size;
		next_codepoint(p, &c_size);

		if (c_size == 1) {
			if (*p == oldc) {
				*p = newc;
			}
		}
		p += c_size;
	}
}