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

	buf->idx = 0;
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

	if (arg_len > 1 && current_arg[0] == '-') {
		/* it's an option */
		if (current_arg[1] == '-') {
			/* long form option */
			if (arg_len > 2) {
				size_t i;
				char *opt = current_arg + 2;
				
				for (i = 0; i < buf->no_of_cfgs; ++i) {
					if (strcmp(opt, buf->cfgs[i].long_name) == 0) {
						return buf->cfgs[i].code;
					}
				}

				/* the option wasn't found */
				*arg_val_out = current_arg;
				return -5;
			}
		} else {
			/* short form option */
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
