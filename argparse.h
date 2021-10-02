#ifndef ARGPARSE_H
#define ARGPARSE_H

#include <stddef.h>

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
 * WARNING: The function does not copy the value of cfgs. Make sure
 * that the buffer is only used while the pointer is valid!
 */
void argparse_init_buf(ArgparseBuf *buf,
                       ArgparseArgCfg cfgs[],
                       size_t no_of_cfgs,
                       size_t argc,
                       char *argv[]);

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

#endif

#endif
