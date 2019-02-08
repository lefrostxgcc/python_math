#ifndef CHIP_OBJECT_H
#define CHIP_OBJECT_H

enum { INT_T, DOUBLE_T, CHAR_T,	STRING_T, CONST_STRING_T };

typedef struct
{
	union
	{
		int i;
		double d;
		char c;
		char *s;
		const char *cs;
	};
	char type;
} object_t;

void object_print(object_t obj);

#endif
