#ifndef ARGPARSE_H
#define ARGPARSE_H

#include <stddef.h>

typedef struct {
	char short_name,
	    *long_name;
	int code;
} ArgparseArgCfg;

typedef struct {
	ArgparseArgCfg *cfg;
	size_t idx,
	       argc;
} ArgparseBuf;

#ifdef ARGPARSE_IMPL
#endif

#endif