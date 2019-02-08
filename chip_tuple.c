#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "chip_tuple.h"

static tuple_t	*tuple_new(object_t obj);
static tuple_t	*tuple_append(tuple_t **head, object_t obj);

static tuple_t *tuple_new(object_t obj)
{
	tuple_t *p = (tuple_t *) calloc(1, sizeof (tuple_t));
	if (!p)
		return NULL;
	p->data = obj;
	p->next = NULL;
	if (obj.type == CONST_STRING_T && obj.cs)
	{
		size_t len = strlen(obj.cs);
		char *str = malloc(len + 1);
		if (!str)
		{
			free(p);
			return NULL;
		}
		memcpy(str, obj.cs, len);
		str[len] = '\0';
		p->data.cs = str;
	}
	if (obj.type == STRING_T && obj.s)
	{
		size_t len = strlen(obj.s);
		char *str = malloc(len + 1);
		if (!str)
		{
			free(p);
			return NULL;
		}
		memcpy(str, obj.s, len);
		str[len] = '\0';
		p->data.cs = str;
	}
	return p;
}

const tuple_t	*tuple_vnew(int count, ...)
{
	tuple_t *head = NULL;
	va_list ap;
	va_start(ap, count);
	while (count-- > 0)
		if (tuple_append(&head, va_arg(ap, object_t)) == NULL)
		{
			tuple_clear((const tuple_t **) &head);
			break;
		}
	va_end(ap);
	return head;
}

static tuple_t *tuple_append(tuple_t **head, object_t obj)
{
	if (!head)
		return NULL;
	tuple_t *p = tuple_new(obj);
	if (!p)
		return NULL;
	if (!*head)
		*head = p;
	else
	{
		tuple_t *h = *head;
		while (h->next)
			h = h->next;
		h->next = p;
	}
	return p;
}

void tuple_clear(const tuple_t **head)
{
	if (!head)
		return;
	const tuple_t *p = *head;
	while (p)
	{
		tuple_t *next = p->next;
		if (p->data.type == STRING_T)
			free(p->data.s);
		else if (p->data.type == CONST_STRING_T)
			free((void *) p->data.cs);
		free((void*) p);
		p = next;
	}
	*head = NULL;
}

const object_t *tuple_value(const tuple_t *head, int index)
{
	if (!head || index < 0)
		return NULL;
	int i;
	for (i = 0; i < index && head; i++)
		head = head->next;
	if (i == index && head)
		return &head->data;
	return NULL;
}

void tuple_print(const tuple_t *head)
{
	putchar('(');
	while (head)
	{
		object_print(head->data);
		if (head->next)
		{
			putchar(',');
			putchar(' ');
		}
		head = head->next;
	}
	putchar(')');
	putchar('\n');
}
