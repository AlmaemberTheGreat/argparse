/*
argparse.h -- single-header, ANSI C argument parsing library,
with support for long-form (GNU style) and short form (Unix style)
arguments.

Copyright (C) 2021 almaember

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

almaember
almaember@almaember.com
*/

#ifndef ARGPARSE_H
#define ARGPARSE_H

#include <stddef.h>
#include <string.h>

typedef struct {
	char short_name,
	    *long_name;
	int code;
} ArgparseArgCfg;

typedef struct {
	ArgparseArgCfg *cfgs;
	size_t no_of_cfgs,
	       idx,
	       argc;
	char **argv;
	int parse_args;
} ArgparseBuf;

/*
 * Initialize an argparse buffer
 * WARNING: All pointer arguments must be non-NULL.
 * The function doesn't check for the pointers' validity at runtime!
 * 
 * WARNING: Pointer arguments must point to writable memory.
 * 
 * WARNING: The function does not copy the value of cfgs. Make sure
 * that the buffer is only used while the pointer is valid!
 * 
 * WARNING: The negative codes in argument configs have special meaning,
 * and they are all reserved for such use. Please only use numbers that
 * are greater than or equal to 0. This is not checked at runtime.
 */
void argparse_init_buf(ArgparseBuf *buf,
                       ArgparseArgCfg cfgs[],
                       size_t no_of_cfgs,
                       size_t argc,
                       char *argv[]);

/*
 * Get the next argument
 * WARNING: All pointer arguments must be non-NULL.
 * The function doesn't check for the pointers' validity at runtime!
 * 
 * WARNING: Pointer arguments must point to writable memory.
 * 
 * RETURNS: the code of the option, or -1 if the argument is positional,
 * or -2 if there are no more arguments left, -3 if the argument is empty.
 * If it returns -4, then something /horrible/ has happened.
 * Report it to me immediately.
 * 
 * It also returns -5 if it's an unknown argument (not in the config list).
 * The arg_val_out will also be set to the FULL VALUE of the current argument.
 * 
 * *arg_val_out will contain the address of the option value,
 * or if there's none, NULL.
*/
int argparse_next(ArgparseBuf *buf,
                  char **arg_val_out);

#ifdef ARGPARSE_IMPL

void argparse_init_buf(ArgparseBuf *buf,
                       ArgparseArgCfg cfgs[],
                       size_t no_of_cfgs,
                       size_t argc,
                       char *argv[]) {
	buf->cfgs = cfgs;
	buf->no_of_cfgs = no_of_cfgs;
	buf->argc = argc;
	buf->argv = argv;

	buf->idx = 1;
	buf->parse_args = 1;
}

int argparse_next(ArgparseBuf *buf,
                  char **arg_val_out) {
	char *current_arg;
	size_t arg_len;

	if (buf->argc <= buf->idx) {
		/* We are at the end of the arg list */
		return -2;
	}

	current_arg = buf->argv[buf->idx++];
	arg_len = strlen(current_arg);

	if (arg_len == 0) {
		/* argument is empty */
		return -3;
	}

	if (arg_len > 1 && current_arg[0] == '-' && buf->parse_args) {
		/* it's an option */
		if (current_arg[1] == '-') {
			/* long form option */
			if (arg_len > 2) {
				size_t i;
				char *opt = current_arg + 2;

				*arg_val_out = NULL;
				for (i = 0; i < arg_len - 2; ++i) {
					if (opt[i] == '=') {
						opt[i] = '\0';
						*arg_val_out = opt + i + 1;
						break;
					}
				}
				
				for (i = 0; i < buf->no_of_cfgs; ++i) {
					if (strcmp(opt, buf->cfgs[i].long_name) == 0) {
						return buf->cfgs[i].code;
					}
				}

				/* the option wasn't found */
				*arg_val_out = current_arg;
				return -5;
			} else {
				/* received '--' as an argument, stop argument parsing */
				buf->parse_args = 0;
				*arg_val_out = NULL;
				return -3;
			}
		} else {
			char arg_opt;
			size_t i;

			/* short form option */
			if (arg_len > 2) {
				/* we have a value */
				*arg_val_out = current_arg + 2;
			} else {
				/* we have no value */
				*arg_val_out = NULL;
			}

			arg_opt = current_arg[1];
			for (i = 0; i < buf->no_of_cfgs; ++i) {
				if (buf->cfgs[i].short_name == arg_opt) {
					return buf->cfgs[i].code;
				}
			}
			return -5;
		}
	} else {
		/* it's a positional argument */
		*arg_val_out = current_arg;
		return -1;
	}

	return -4;
}

#endif

#endif
