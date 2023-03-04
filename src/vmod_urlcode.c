#include <stdlib.h>

#include <cache/cache.h>

#include <vcc_if.h>

static char hexchars[] = "0123456789ABCDEF";
#define visalpha(c) \
	((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
#define visalnum(c) \
	((c >= '0' && c <= '9') || visalpha(c))

VCL_STRING
vmod_encode(VRT_CTX, VCL_STRANDS s)
{
	char *b, *e;
	unsigned u;

	CHECK_OBJ_NOTNULL(ctx, VRT_CTX_MAGIC);
	CHECK_OBJ_NOTNULL(ctx->ws, WS_MAGIC);
	u = WS_ReserveAll(ctx->ws);
	e = b = ctx->ws->f;
	e += u;

    for(int i=0; i < s->n; i++)
	{
        const char *str = s->p[i];
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

VCL_STRING
vmod_decode(VRT_CTX, VCL_STRANDS s)
{
	char *b, *e;
	int h, l;
	unsigned u;
	int percent = 0;
    int i = 0;

	CHECK_OBJ_NOTNULL(ctx, VRT_CTX_MAGIC);
	CHECK_OBJ_NOTNULL(ctx->ws, WS_MAGIC);
	u = WS_ReserveAll(ctx->ws);
	e = b = ctx->ws->f;
	e += u;

	while (b < e && i < s->n)
	{
        const char *str = s->p[i];

        while(*str)
        {
            if (percent == 0)
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
        i++;
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
