#include <stdlib.h>

#include <cache/cache.h>

#include <vcc_if.h>

static char hexchars[] = "0123456789ABCDEF";
#define visalpha(c) \
	((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
#define visalnum(c) \
	((c >= '0' && c <= '9') || visalpha(c))

const char *
vmod_encode(const struct vrt_ctx *ctx, const char *str, ...)
{
	char *b, *e;
	unsigned u;
	va_list ap;

	CHECK_OBJ_NOTNULL(ctx, VRT_CTX_MAGIC);
	CHECK_OBJ_NOTNULL(ctx->ws, WS_MAGIC);
	u = WS_Reserve(ctx->ws, 0);
	e = b = ctx->ws->f;
	e += u;
	va_start(ap, str);
	while (b < e && str != vrt_magic_string_end)
	{
		while (b < e && str && *str)
		{
			if (visalnum((int)*str) || *str == '-' || *str == '.' || *str == '_' || *str == '~')
			{ /* RFC3986 2.3 */
				*b++ = *str++;
			}
			else if (b + 4 >= e)
			{				 /* % hex hex NULL */
				b = e; /* not enough space */
			}
			else
			{
				*b++ = '%';
				unsigned char foo = *str;
				*b++ = hexchars[foo >> 4];
				*b++ = hexchars[*str & 15];
				str++;
			}
		}
		str = va_arg(ap, const char *);
	}
	if (b < e)
		*b = '\0';
	b++;
	if (b > e)
	{
		WS_Release(ctx->ws, 0);
		return (NULL);
	}
	else
	{
		e = b;
		b = ctx->ws->f;
		WS_Release(ctx->ws, e - b);
		return (b);
	}
}

static inline int
vmod_hex_to_int(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	else if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	else if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	return (-1);
}

const char *
vmod_decode(const struct vrt_ctx *ctx, const char *str, ...)
{
	char *b, *e;
	int h, l;
	unsigned u;
	va_list ap;
	int percent = 0;

	CHECK_OBJ_NOTNULL(ctx, VRT_CTX_MAGIC);
	CHECK_OBJ_NOTNULL(ctx->ws, WS_MAGIC);
	u = WS_Reserve(ctx->ws, 0);
	e = b = ctx->ws->f;
	e += u;
	va_start(ap, str);
	while (b < e && str != vrt_magic_string_end)
	{
		if (str == NULL || *str == '\0')
		{
			str = va_arg(ap, const char *);
		}
		else if (percent == 0)
		{
			switch (*str)
			{
			case '%':
				percent = 1;
				str++;
				break;
			case '+':
				*b++ = ' ';
				str++;
				break;
			default:
				*b++ = *str++;
				break;
			}
		}
		else if (percent == 1)
		{
			h = vmod_hex_to_int(*str++);
			if (h < 0)
				b = e;
			percent = 2;
		}
		else if (percent == 2)
		{
			l = vmod_hex_to_int(*str++);
			if (l < 0)
				b = e;
			*b++ = (char)((h << 4) | l);
			percent = 0;
		}
	}
	if (b < e)
		*b = '\0';
	b++;
	if (b > e)
	{
		WS_Release(ctx->ws, 0);
		return (NULL);
	}
	else
	{
		e = b;
		b = ctx->ws->f;
		WS_Release(ctx->ws, e - b);
		return (b);
	}
}
