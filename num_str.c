#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <setjmp.h>
#include <stdbool.h>
#include <math.h>
#include "chip_list.h"
#include "chip_map.h"
#include "chip_tuple.h"

enum { ValueError = 1, ZeroDivisionError = 2, EOFError = 3, Exception = 4 };

static jmp_buf		jmp_main;
static list_t		*check(long long number);
static list_t		*calc(long long number);
static char 		*get_hundred(int nm);
static list_t		*get_str_num(const list_t *list);
static long long	read_int64(void);

int main(void)
{
	list_t *result_calc = NULL;
	volatile long long number;
	const tuple_t *about = 
		tuple_vnew
		(
			3,
			(object_t) { .type = CONST_STRING_T, .cs = "num_str" },
			(object_t) { .type = CONST_STRING_T, .cs = "2.2.3.2" },
			(object_t) { .type = CONST_STRING_T, .cs = "Chip" }
		);
	printf(u8"Программа: %s релиз: %s автор: %s\n",
		tuple_value(about, 0)->cs,
		tuple_value(about, 1)->cs,
		tuple_value(about, 2)->cs);
	while (true)
	{
		switch (sigsetjmp(jmp_main, 1))
		{
			case 0:
				fputs(u8"Введите число: ", stdout);
				number = read_int64();
				list_t *result = check(number);
				result_calc = list_extend(result_calc, result);
				list_clear(&result);
				break;
			case ValueError:
				list_append(&result_calc,
					(object_t){.type = STRING_T, .s = u8"Это не число"});
				break;
			case EOFError:
				list_append
				(
					&result_calc,
					(object_t)
					{
						.type = STRING_T,
						.s = u8"Ошибка выполнения программы:"
								" EOF when reading a line"
					}
				);
				break;
			case Exception:
				list_append(&result_calc,
					(object_t) {.type = STRING_T, .s = strerror(errno)});
				break;
		}
		list_print(result_calc);
		tuple_clear(&about);
		list_clear(&result_calc);
	}
}

static list_t *check(long long number)
{
	list_t *list_number = NULL;
	list_t *list_name = list_vnew
	(
		4,
		(object_t) {.type = STRING_T, .s = ""},
		(object_t) {.type = STRING_T, .s = u8"тыс"},
		(object_t) {.type = STRING_T, .s = u8"млн"},
		(object_t) {.type = STRING_T, .s = u8"млрд"}
	);
	for (int i = list_len(list_name); i > -1; i--)
	{
		if (number / (long long) pow(1000, i) > 0 &&
			number / (long long) pow(1000, i) < 1000)
		{
			list_t *digits = calc(number / (long long) pow(1000, i));
			list_t *str_num = get_str_num(digits);
			list_number = list_extend(list_number, str_num);
			list_append(&list_number, *list_value(list_name, i));
			number = number % (long long) pow(1000, i);
			list_clear(&str_num);
			list_clear(&digits);
		}
	}
	list_clear(&list_name);
	return list_number;
}

static list_t *get_str_num(const list_t *list)
{
	map_t *hundreds = map_vnew(10,
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 0},
			.value = (object_t) {.type = STRING_T, .s = u8""}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 1},
			.value = (object_t) {.type = STRING_T, .s = u8"сто"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 2},
			.value = (object_t) {.type = STRING_T, .s = u8"двести"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 3},
			.value = (object_t) {.type = STRING_T, .s = u8"триста"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 4},
			.value = (object_t) {.type = STRING_T, .s = u8"четыреста"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 5},
			.value = (object_t) {.type = STRING_T, .s = u8"пятьсот"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 6},
			.value = (object_t) {.type = STRING_T, .s = u8"шестьсот"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 7},
			.value = (object_t) {.type = STRING_T, .s = u8"семьсот"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 8},
			.value = (object_t) {.type = STRING_T, .s = u8"восемьсот"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 9},
			.value = (object_t) {.type = STRING_T, .s = u8"девятьсот"}
		}
	);
	map_t *tens = map_vnew(9,
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 0},
			.value = (object_t) {.type = STRING_T, .s = u8""}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 2},
			.value = (object_t) {.type = STRING_T, .s = u8"двадцать"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 3},
			.value = (object_t) {.type = STRING_T, .s = u8"тридцать"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 4},
			.value = (object_t) {.type = STRING_T, .s = u8"сорок"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 5},
			.value = (object_t) {.type = STRING_T, .s = u8"пятьдесят"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 6},
			.value = (object_t) {.type = STRING_T, .s = u8"шестьдесят"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 7},
			.value = (object_t) {.type = STRING_T, .s = u8"семьдесят"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 8},
			.value = (object_t) {.type = STRING_T, .s = u8"восемьдесят"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 9},
			.value = (object_t) {.type = STRING_T, .s = u8"девяносто"}
		}
	);
	map_t *ten = map_vnew(10,
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 0},
			.value = (object_t) {.type = STRING_T, .s = u8"десять"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 1},
			.value = (object_t) {.type = STRING_T, .s = u8"одиннадцать"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 2},
			.value = (object_t) {.type = STRING_T, .s = u8"двенадцать"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 3},
			.value = (object_t) {.type = STRING_T, .s = u8"тринадцать"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 4},
			.value = (object_t) {.type = STRING_T, .s = u8"четырнадцать"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 5},
			.value = (object_t) {.type = STRING_T, .s = u8"пятнадцать"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 6},
			.value = (object_t) {.type = STRING_T, .s = u8"шестнадцать"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 7},
			.value = (object_t) {.type = STRING_T, .s = u8"семнадцать"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 8},
			.value = (object_t) {.type = STRING_T, .s = u8"восемнадцать"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 9},
			.value = (object_t) {.type = STRING_T, .s = u8"девятнадцать"}
		}
	);
	map_t *units = map_vnew(10,
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 0},
			.value = (object_t) {.type = STRING_T, .s = u8""}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 1},
			.value = (object_t) {.type = STRING_T, .s = u8"один"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 2},
			.value = (object_t) {.type = STRING_T, .s = u8"два"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 3},
			.value = (object_t) {.type = STRING_T, .s = u8"три"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 4},
			.value = (object_t) {.type = STRING_T, .s = u8"четыре"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 5},
			.value = (object_t) {.type = STRING_T, .s = u8"пять"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 6},
			.value = (object_t) {.type = STRING_T, .s = u8"шесть"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 7},
			.value = (object_t) {.type = STRING_T, .s = u8"семь"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 8},
			.value = (object_t) {.type = STRING_T, .s = u8"восемь"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 9},
			.value = (object_t) {.type = STRING_T, .s = u8"девять"}
		}
	);
	int d0 = list_value(list, 0)->i;
	int d1 = list_value(list, 1)->i;
	int d2 = list_value(list, 2)->i;
	object_t key0 = {.type = INT_T, .i = d0};
	object_t key1 = {.type = INT_T, .i = d1};
	object_t key2 = {.type = INT_T, .i = d2};
	list_t *str_list = NULL;
	if (d1 >= 2)
	{
		str_list = list_vnew
		(
			3,
			(object_t)
			{
				.type = STRING_T,
				.s = map_value(hundreds, key0)->s
			},
			(object_t)
			{
				.type = STRING_T,
				.s = map_value(tens, key1)->s
			},
			(object_t)
			{
				.type = STRING_T,
				.s = map_value(units, key2)->s
			}
		);
	}
	else if (d1 == 1)
	{
		str_list = list_vnew
		(
			3,
			(object_t)
			{
				.type = STRING_T,
				.s = map_value(hundreds, key0)->s
			},
			(object_t)
			{
				.type = STRING_T,
				.s = ""
			},
			(object_t)
			{
				.type = STRING_T,
				.s = map_value(ten, key2)->s
			}
		);
	}
	else if (d1 == 0)
	{
		str_list = list_vnew
		(
			3,
			(object_t)
			{
				.type = STRING_T,
				.s = map_value(hundreds, key0)->s
			},
			(object_t)
			{
				.type = STRING_T,
				.s = ""
			},
			(object_t)
			{
				.type = STRING_T,
				.s = map_value(units, key2)->s
			}
		);
	}
	map_clear(&units);
	map_clear(&ten);
	map_clear(&tens);
	map_clear(&hundreds);
	return str_list;
}

static list_t *calc(long long number)
{
	int d3 = number % 10;
	int d2 = number / 10 % 10;
	int d1 = number / 100 % 10;
	list_t *result = list_vnew
	(
		3,
		(object_t) {.type = INT_T, .i = d1},
		(object_t) {.type = INT_T, .i = d2},
		(object_t) {.type = INT_T, .i = d3}
	);
	return result;
}

static long long read_int64(void)
{
	char *end, buf[80], ch;
	if (!fgets(buf, sizeof buf, stdin))
		if (feof(stdin))
			siglongjmp(jmp_main, EOFError);
		else
			siglongjmp(jmp_main, Exception);
	errno = 0;
	long long a = strtoll(buf, &end, 10);
	if (end == buf || errno == ERANGE || sscanf(end, "%1s", &ch) == 1)
		siglongjmp(jmp_main, ValueError);
	return a;
}

static char *get_hundred(int nm)
{
	map_t *dict = map_vnew(9,
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 1},
			.value = (object_t) {.type = STRING_T, .s = u8"сто"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 2},
			.value = (object_t) {.type = STRING_T, .s = u8"двести"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 3},
			.value = (object_t) {.type = STRING_T, .s = u8"триста"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 4},
			.value = (object_t) {.type = STRING_T, .s = u8"четыреста"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 5},
			.value = (object_t) {.type = STRING_T, .s = u8"пятьсот"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 6},
			.value = (object_t) {.type = STRING_T, .s = u8"шестьсот"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 7},
			.value = (object_t) {.type = STRING_T, .s = u8"семьсот"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 8},
			.value = (object_t) {.type = STRING_T, .s = u8"восемьсот"}
		},
		(pair_t)
		{
			.key = (object_t) {.type = INT_T, .i = 9},
			.value = (object_t) {.type = STRING_T, .s = u8"девятьсот"}
		}
	);
	if (!dict)
		return strdup("map_vnew error");
	char *p = strdup(map_value(dict, (object_t){.type=INT_T, .i = nm})->s);
	map_clear(&dict);
	return p;
}
