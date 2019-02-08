#include <stdio.h>

#include "chip_list.h"

int main(void)
{
	puts(">>> s = []");
	list_t *s = NULL;
	puts(">>> s");
	list_print(s);
	puts(">>> s.append(1)");
	list_append(&s, (object_t){.type = INT_T, .i = 1});
	puts(">>> s.append(2)");
	list_append(&s, (object_t){.type = INT_T, .i = 2});
	puts(">>> s");
	list_print(s);
	puts(">>> s.append('hello')");
	list_append(&s, (object_t){.type = STRING_T, .s = "hello"});
	puts(">>> s");
	list_print(s);
	puts(">>> s.append(2.3)");
	list_append(&s, (object_t){.type = DOUBLE_T, .d = 2.3});
	list_print(s);
	puts(">>> p = list('python')");
	list_t *p = list_list("python");
	puts(">>> p");
	list_print(p);
	puts(">>> p.extend(s)");
	list_extend(p, s);
	puts(">>> p");
	list_print(p);
	puts(">>> s.remove('hello')");
	list_remove(&s, (object_t){.type = STRING_T, .s = "hello"});
	puts(">>> s");
	list_print(s);
	puts(">>> s.clear()");
	list_clear(&s);
	puts(">>> s");
	list_print(s);
	puts(">>> p.insert(1, 'p')");
	list_insert(&p, 1, (object_t){.type = CHAR_T, .c = 'p'});
	puts(">>> p");
	list_print(p);
	puts(">>> p.insert(4, 'p')");
	list_insert(&p, 4, (object_t){.type = CHAR_T, .c = 'p'});
	puts(">>> p");
	list_print(p);
	puts(">>> p.count('p')");
	printf("%d\n", list_count(p, (object_t){.type = CHAR_T, .c = 'p'}));
	puts(">>> p.count(2)");
	printf("%d\n", list_count(p, (object_t){.type = INT_T, .i = 2}));
	puts(">>> p.count(20000)");
	printf("%d\n", list_count(p, (object_t){.type = INT_T, .i = 20000}));
	list_clear(&s);
	list_clear(&p);
}
