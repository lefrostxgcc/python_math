#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "chip_map.h"

map_t *map_new(pair_t obj)
{
	map_t *p = (map_t *) calloc(1, sizeof (map_t));
	if (!p)
		return NULL;
	p->data = obj;
	p->next = NULL;
	if (obj.key.type == STRING_T && obj.key.s)
	{
		size_t len = strlen(obj.key.s);
		p->data.key.s = (char *) malloc(len + 1);
		if (!p->data.key.s)
		{
			free(p);
			return NULL;
		}
		memcpy(p->data.key.s, obj.key.s, len);
		p->data.key.s[len] = '\0';
	}

	if (obj.value.type == STRING_T && obj.value.s)
	{
		size_t len = strlen(obj.value.s);
		p->data.value.s = (char *) malloc(len + 1);
		if (!p->data.value.s)
		{
			free(p);
			return NULL;
		}
		memcpy(p->data.value.s, obj.value.s, len);
		p->data.value.s[len] = '\0';
	}
	return p;
}

map_t	*map_vnew(int count, ...)
{
	map_t *head = NULL;
	va_list ap;
	va_start(ap, count);
	while (count-- > 0)
		if (map_append(&head, va_arg(ap, pair_t)) == NULL)
		{
			map_clear(&head);
			break;
		}
	va_end(ap);
	return head;
}

map_t *map_append(map_t **head, pair_t obj)
{
	if (!head)
		return NULL;
	map_t *p = map_new(obj);
	if (!p)
		return NULL;
	if (!*head)
		*head = p;
	else
	{
		map_t *h = *head;
		while (h->next)
			h = h->next;
		h->next = p;
	}
	return p;
}

void map_clear(map_t **head)
{
	if (!head)
		return;
	map_t *p = *head;
	while (p)
	{
		map_t *next = p->next;
		if (p->data.key.type == STRING_T)
			free(p->data.key.s);
		if (p->data.value.type == STRING_T)
			free(p->data.value.s);
		p->next = NULL;
		free(p);
		p = next;
	}
	*head = NULL;
}

const object_t *map_value(const map_t *head, object_t key)
{
	if (!head)
		return NULL;
	while (head)
	{
		if (key.type == STRING_T && head->data.key.type == STRING_T)
		{
			if (strcmp(head->data.key.s, key.s) == 0)
				return &head->data.value;
		}
		else if (memcmp(&head->data.key, &key, sizeof (object_t)) == 0)
			return &head->data.value;
		head = head->next;
	}
	return NULL;
}

void map_print(map_t *head)
{
	putchar('{');
	while (head)
	{
		object_print(head->data.key);
		putchar(':');
		putchar(' ');
		object_print(head->data.value);
		if (head->next)
		{
			putchar(',');
			putchar(' ');
		}
		head = head->next;
	}
	putchar('}');
	putchar('\n');
}
