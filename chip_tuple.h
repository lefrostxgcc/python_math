#ifndef CHIP_TUPLE_H
#define CHIP_TUPLE_H

#include "object.h"

typedef struct chip_tuple
{
	object_t data;
	struct chip_tuple *next;
} tuple_t;

tuple_t			*tuple_vnew(int count, ...);
void			tuple_print(tuple_t *head);
const object_t	*tuple_value(const tuple_t *head, int index);
void			tuple_clear(tuple_t **head);

#endif
