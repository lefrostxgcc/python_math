#ifndef CHIP_LIST_H
#define CHIP_LIST_H

#include "object.h"

typedef struct chip_list
{
	object_t data;
	struct chip_list *next;
} list_t;

list_t	*list_new(object_t obj);
list_t	*list_vnew(int count, ...);
list_t	*list_append(list_t **head, object_t obj);
list_t	*list_list(const char *s);
list_t	*list_extend(list_t *p, const list_t *s);
list_t	*list_insert(list_t **head, int index, object_t obj);
list_t	*list_remove(list_t **head, object_t obj);
void	list_print(const list_t *head);
const object_t *list_value(const list_t *head, int index);
int		list_count(const list_t *head, object_t obj);
void	list_clear(list_t **head);

#endif
