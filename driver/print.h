#include <stdarg.h>

EFI_SYSTEM_TABLE* c_system_table = 0;

// There is a built in print fuction in edk2 however I was having comilation issues so I just vibecoded this

#define PRINT_BUFFER_CHARS 512

static void put_char(CHAR16* buf, UINTN* pos, CHAR16 c)
{
	if (*pos + 3 >= PRINT_BUFFER_CHARS)
		return;

	if (c == L'\n')
		buf[(*pos)++] = L'\r';

	buf[(*pos)++] = c;
}

static void put_string(CHAR16* buf, UINTN* pos, const CHAR16* s)
{
	if (!s)
		s = L"(null)";

	while (*s)
		put_char(buf, pos, *s++);
}

static void put_ascii(CHAR16* buf, UINTN* pos, const char* s)
{
	if (!s)
		s = "(null)";

	while (*s)
		put_char(buf, pos, (CHAR16)*s++);
}

static void put_unsigned(CHAR16* buf, UINTN* pos, UINT64 value, UINT32 base, BOOLEAN upper)
{
	const CHAR16* digits = upper ? L"0123456789ABCDEF" : L"0123456789abcdef";
	CHAR16 tmp[64];
	UINTN count = 0;

	do
	{
		tmp[count++] = digits[value % base];
		value /= base;
	} while (value != 0);

	while (count > 0)
		put_char(buf, pos, tmp[--count]);
}

static void put_signed(CHAR16* buf, UINTN* pos, INT64 value)
{
	UINT64 magnitude = (UINT64)value;

	if (value < 0)
	{
		put_char(buf, pos, L'-');
		magnitude = 0 - magnitude;
	}

	put_unsigned(buf, pos, magnitude, 10, FALSE);
}

void print(const CHAR16* format, ...)
{
	CHAR16 buf[PRINT_BUFFER_CHARS];
	UINTN pos = 0;

	va_list args;
	va_start(args, format);

	for (; *format; format++)
	{
		if (*format != L'%')
		{
			put_char(buf, &pos, *format);
			continue;
		}

		format++;

		// Length modifier: "ll" means 64-bit
		UINT32 l_count = 0;
		while (*format == L'l')
		{
			l_count++;
			format++;
		}
		BOOLEAN is_64 = l_count >= 2;

		switch (*format)
		{
		case L'd':
		case L'i':
			put_signed(buf, &pos, is_64 ? va_arg(args, INT64) : (INT64)va_arg(args, INT32));
			break;

		case L'u':
			put_unsigned(buf, &pos, is_64 ? va_arg(args, UINT64) : (UINT64)va_arg(args, UINT32), 10, FALSE);
			break;

		case L'x':
			put_unsigned(buf, &pos, is_64 ? va_arg(args, UINT64) : (UINT64)va_arg(args, UINT32), 16, FALSE);
			break;

		case L'X':
			put_unsigned(buf, &pos, is_64 ? va_arg(args, UINT64) : (UINT64)va_arg(args, UINT32), 16, TRUE);
			break;

		case L'p':
			put_string(buf, &pos, L"0x");
			put_unsigned(buf, &pos, (UINT64)va_arg(args, void*), 16, FALSE);
			break;

		case L'c':
			put_char(buf, &pos, (CHAR16)va_arg(args, int));
			break;

		case L's':
			put_string(buf, &pos, va_arg(args, const CHAR16*));
			break;

		case L'a':
			put_ascii(buf, &pos, va_arg(args, const char*));
			break;

		case L'%':
			put_char(buf, &pos, L'%');
			break;

		case L'\0':
			goto done; // format ended right after a '%'

		default:
			// Unknown specifier: print it literally
			put_char(buf, &pos, L'%');
			put_char(buf, &pos, *format);
			break;
		}
	}

done:
	va_end(args);

	buf[pos] = 0;
	c_system_table->ConOut->OutputString(c_system_table->ConOut, buf);
}