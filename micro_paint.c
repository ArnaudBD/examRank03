#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_drawing
{
	int width;
	int height;
	char bg;
	char *d;
} t_d;

typedef struct s_rectangle
{
	char type;
	float x;
	float y;
	float width;
	float height;
	char c;
} t_rect;

int ft_strlen(char *s)
{
	int i;

	i = 0;
	while(s && s[i])
		i++;
	return (i);
}

void print_d(t_d d)
{
	int i;

	i = 0;
	while(d.d[i])
	{
		write(1, &d.d[i], 1);
		if ((i || d.width == 1) && (i + 1) % d.width == 0)
			write(1, "\n", 1);
		i++;
	}
}

int	check_r(t_rect r)
{
	if (r.width <= 0 || r.height <= 0 || (r.type != 'r' && r.type != 'R'))
		return (0);
	return (1);
}

int where(float x, float y, t_rect r)
{
	if (x < r.x || x >= (r.x + r.width) || y < r.y || y >= (r.y + r.height))
		return (0); //outside
	if ((x - r.x) < 1.00000000 || ((r.x + r.width) - x) <= 1.00000000 || (y - r.y) < 1.00000000 || ((r.y + r.height) - y) <= 1.00000000)
		return (1); //line
	return (2); //inside
}

void modify_d(int x, int y, t_d *d, t_rect r)
{
	int place;
	
	place = where((float)x, (float)y, r);
	if (place == 1 || (place == 2 && r.type == 'R'))
		d->d[(y * d->width) + x] = r.c;
}

int main(int ac, char *av[])
{
	int x;
	int y;
	int ret;
	t_d d;
	t_rect r;
	FILE *file;
	char *err_op = "Error: Operation file corrupted\n";
	char *err_arg = "Error: argument\n";
	if (ac != 2)
	{
		write(1, err_arg, ft_strlen(err_arg));
		return (1);
	}
	file = fopen(av[1], "r");
	if (!file)
	{
		write(1, err_op, ft_strlen(err_op));
		return (1);
	}
	ret = fscanf(file, "%d %d %c\n", &d.width, &d.height, &d.bg);
	if (ret != 3 || d.width > 300 || d.width <= 0 || d.height > 300 || d.height <= 0)
	{
		fclose(file);
		write(1, err_op, ft_strlen(err_op));
		return (1);	
	}
	d.d = malloc(d.width * d.height + 1);
	if (!d.d)
	{
		fclose(file);
		write(1, err_op, ft_strlen(err_op));
		return (1);	
	}
	memset(d.d, d.bg, d.width * d.height);
	d.d[d.width * d.height] = 0;
	ret = fscanf(file, "%c %f %f %f %f %c\n", &r.type, &r.x, &r.y, &r.width, &r.height, &r.c);
	while (ret == 6)
	{
		if (!check_r(r))
		{
			free(d.d);
			fclose(file);
			write(1, err_op, ft_strlen(err_op));
			return (1);	
		}
		y = 0;
		while (y < d.height)
		{
			x = 0;
			while (x < d.width)
			{
				modify_d(x, y, &d, r);
				x++;
			}
			y++;
		}
		ret = fscanf(file, "%c %f %f %f %f %c\n", &r.type, &r.x, &r.y, &r.width, &r.height, &r.c);
	}
	if (ret != -1)
	{
		free(d.d);
		fclose(file);
		write(1, err_op, ft_strlen(err_op));
		return (1);	
	}
	print_d(d);
	fclose(file);
	free(d.d);
	return (0);
}
