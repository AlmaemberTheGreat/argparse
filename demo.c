/*
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

#include <stdio.h>

#define ARGPARSE_IMPL
#include "argparse.h"

int main(int argc, char *argv[]) {
	/* Define demo options */
	ArgparseArgCfg arg_cfg[] = {
		{'o', "output", 1},
		{'f', "file",   2}
	};
	ArgparseBuf arg_buf;
	int arg_code;
	char *option_value;
	argparse_init_buf(&arg_buf, arg_cfg,
	                  sizeof(arg_cfg)/sizeof(ArgparseArgCfg),
	                  argc, argv);


	while ((arg_code = argparse_next(&arg_buf, &option_value)) != -2) {
		printf("Option: %d\n Value: %s\n\n", arg_code, option_value != NULL ? option_value : "<none>");
	}

	return 0;
}
