# argparse.h
`argparse.h` is a single-header, ANSI C argument parsing library,
with support for long-form (GNU style) and short form (Unix style)
arguments. It is implemented in less than 200 lines of code, and does
not depend on dynamic allocation or non-standard features.

## Usage
To use `argparse.h`, you need to do *two* things:

* Include `argparse.h` in all the files that you want to use it in.
* Include `argparse.h` with the macro `ARGPARSE_IMPL` defined
in exactly **one** file.

You should then define your options, in an array like this:

```c
ArgparseArgCfg arg_cfg[] = {
		{'o', "output", 1},
		{'f', "file",   2}
	};
```

The third value is the code of the option. This will be used to signal
to you which option was found. It must be a natural number, as negative ones
are used for special purposes.

For more information, check `argparse.h` and the below example.

## Special option codes
These are used to signal special states. Macros for them may be defined in the future:

| **Number** | **Meaning**                          |
|------------|--------------------------------------|
| -1         | Positional argument                  |
| -2         | Reached the end of the argument list |
| -3         | The argument is empty (length is 0)  |
| -4         | Internal failure. Should not happen  |
| -5         | Unkown option (not in the config)    |

(Yes, I'm the god of API design.)

## Example
```c
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
```