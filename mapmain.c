#include <stdio.h>
#include "chip_map.h"

int main(void)
{
	map_t *dict = map_vnew(2,
	(pair_t){(object_t){.type=INT_T, .i = 1}, (object_t){.type=INT_T, .i = 10}},
	(pair_t){(object_t){.type=INT_T, .i = 2}, (object_t){.type=INT_T, .i = 20}});
	object_t *p = map_value(dict, (object_t){.type=INT_T, .i = 1});
	if (p);
		printf("%d\n", p->i);
	map_print(dict);
	map_clear(&dict);
}
