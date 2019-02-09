#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

char *obj_str(object_t obj)
{
	char buf[32];

	switch (obj.type)
	{
		case INT_T: snprintf(buf, sizeof buf, "%d", obj.i); break;
		case DOUBLE_T: snprintf(buf, sizeof buf, "%f", obj.d); break;
		case CHAR_T: snprintf(buf, sizeof buf, "%c", obj.c); break;
		case STRING_T: return strdup(obj.s);
		case CONST_STRING_T: return strdup(obj.cs);
	}
	return strdup(buf);
}
