#include <stdio.h>
#include "object.h"

void object_print(object_t obj)
{
	switch (obj.type)
	{
		case INT_T: printf("%d", obj.i); break;
		case DOUBLE_T: printf("%.1f", obj.d); break;
		case CHAR_T: printf("'%c'", obj.c); break;
		case STRING_T: printf("'%s'", obj.s); break;
		case CONST_STRING_T: printf("'%s'", obj.cs); break;
	}
}
