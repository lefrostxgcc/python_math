#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "chip_list.h"

list_t *list_new(object_t obj)
{
	list_t *p = (list_t *) calloc(1, sizeof (list_t));
	if (!p)
		return NULL;
	p->data = obj;
	p->next = NULL;
	if (obj.type == STRING_T && obj.s)
	{
		size_t len = strlen(obj.s);
		p->data.s = (char *) malloc(len + 1);
		if (!p->data.s)
		{
			free(p);
			return NULL;
		}
		memcpy(p->data.s, obj.s, len);
		p->data.s[len] = '\0';
	}
	return p;
}

list_t	*list_vnew(int count, ...)
{
	list_t *head = NULL;
	va_list ap;
	va_start(ap, count);
	while (count-- > 0)
		if (list_append(&head, va_arg(ap, object_t)) == NULL)
		{
			list_clear(&head);
			break;
		}
	va_end(ap);
	return head;
}

list_t *list_append(list_t **head, object_t obj)
{
	if (!head)
		return NULL;
	list_t *p = list_new(obj);
	if (!p)
		return NULL;
	if (!*head)
		*head = p;
	else
	{
		list_t *h = *head;
		while (h->next)
			h = h->next;
		h->next = p;
	}
	return p;
}

list_t *list_list(const char *s)
{
	list_t *head = NULL;
	while (*s)
	{
		list_t *p = list_append(&head, (object_t){.type = CHAR_T, .c = *s});
		if (!p)
		{
			list_clear(&head);
			break;
		}
		s++;
	}
	return head;
}

list_t *list_extend(list_t *p, const list_t *s)
{
	if (!p && !s)
		return NULL;
	if (!s)
		return p;
	while (s)
	{
		list_t *t = list_append(&p, s->data);
		if (!t)
			break;
		s = s->next;
	}
	return p;
}

list_t *list_insert(list_t **head, int index, object_t obj)
{
	if (!head)
		return NULL;
	list_t *p = list_new(obj);
	if (!p)
		return NULL;
	if (!*head)
		*head = p;
	else
	{
		list_t *h = *head;
		list_t *prev = h;
		while (h && index-- > 0)
		{
			prev = h;
			h = h->next;
		}
		if (h == *head)
		{
			p->next = h;
			*head = p;
		}
		else
		{
			p->next = h;
			prev->next = p;
		}
	}
	return p;
}

int list_count(const list_t *head, object_t obj)
{
	if (!head)
		return 0;
	int count = 0;
	while (head)
	{
		if (obj.type == STRING_T && head->data.type == STRING_T)
		{
			if (strcmp(head->data.s, obj.s) == 0)
				count++;
		}
		else if (memcmp(&head->data, &obj, sizeof (object_t)) == 0)
			count++;
		head = head->next;
	}
	return count;
}

int list_len(const list_t *head)
{
	if (!head)
		return 0;
	int len = 0;
	while (head)
	{
		head = head->next;
		len++;
	}
	return len;
}

list_t *list_remove(list_t **head, object_t obj)
{
	if (!head || !*head)
		return NULL;
	list_t *h = *head;
	list_t *prev = h;
	while (h)
	{
		if (obj.type == STRING_T && h->data.type == STRING_T)
		{
			if (strcmp(h->data.s, obj.s) == 0)
				break;
		}
		else if (memcmp(&h->data, &obj, sizeof (object_t)) == 0)
			break;
		prev = h;
		h = h->next;
	}
	if (h == NULL)
		return NULL;
	if (h == *head)
		*head = h->next;
	else
		prev->next = h->next;
	if (h->data.type == STRING_T)
		free(h->data.s);
	free(h);
	return prev->next;
}

void list_clear(list_t **head)
{
	if (!head)
		return;
	list_t *p = *head;
	while (p)
	{
		list_t *next = p->next;
		if (p->data.type == STRING_T)
			free(p->data.s);
		p->next = NULL;
		free(p);
		p = next;
	}
	*head = NULL;
}

const object_t *list_value(const list_t *head, int index)
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

void list_print(const list_t *head)
{
	putchar('[');
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
	putchar(']');
	putchar('\n');
}
