#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <setjmp.h>

enum { ValueError = 1, ZeroDivisionError = 2, EOFError = 3, Exception = 4 };

static jmp_buf		jmp_main;
static long double	calc_c(void);
static long long	read_int64(void);

int main(void)
{
	switch (setjmp(jmp_main))
	{
		case 0:
			printf("%.1Lf\n", calc_c());
			break;
		case ValueError:
			puts(u8"Это не число! Выходим из программы");
			break;
		case ZeroDivisionError:
			puts(u8"На ноль делить нельзя! Выход");
			break;
		case EOFError:
			puts(u8"\nОшибка: EOF when reading a line");
			break;
		case Exception:
			perror(u8"\nОшибка");
			break;
	}
}

static long double calc_c(void)
{
	fputs(u8"Число 1: ", stdout);
	long long a = read_int64();
	fputs(u8"Число 2: ", stdout);
	long long b = read_int64();
	if (b == 0)
		longjmp(jmp_main, ZeroDivisionError);
	return (long double) a / b + 12;
}

static long long read_int64(void)
{
	char *end, buf[80], ch;
	if (!fgets(buf, sizeof buf, stdin))
		if (feof(stdin))
			longjmp(jmp_main, EOFError);
		else
			longjmp(jmp_main, Exception);
	errno = 0;
	long long a = strtoll(buf, &end, 10);
	if (end == buf || errno == ERANGE || sscanf(end, "%1s", &ch) == 1)
		longjmp(jmp_main, ValueError);
	return a;
}
