#ifndef CHIP_MAP_H
#define CHIP_MAP_H

#include "object.h"

typedef struct {
	object_t key;
	object_t value;
} pair_t;

typedef struct chip_map
{
	pair_t data;
	struct chip_map *next;
} map_t;

map_t	*map_new(pair_t obj);
map_t	*map_vnew(int count, ...);
const object_t *map_value(const map_t *head, object_t key);
map_t	*map_append(map_t **head, pair_t obj);
void	map_print(const map_t *head);
void	map_clear(map_t **head);

#endif
