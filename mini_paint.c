#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct s_drawing
{
	int width;
	int height;
	char bg;
	char *d;
} t_d;

typedef struct s_circle
{
	char type;
	float x;
	float y;
	float radius;
	char c;
} t_circle;

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

int	check_c(t_circle c)
{
	if (c.radius <= 0 || (c.type != 'c' && c.type != 'C'))
		return (0);
	return (1);
}

int where(float x, float y, t_circle c)
{
	float dist;

	dist = sqrtf(powf(x - c.x, 2) + powf(y - c.y, 2));
	if (dist > c.radius)
		return (0); //outside
	if (c.radius -dist < 1.00000000)
		return (1); //line
	return (2); //inside
}

void modify_d(int x, int y, t_d *d, t_circle c)
{
	int place;
	
	place = where((float)x, (float)y, c);
	if (place == 1 || (place == 2 && c.type == 'C'))
		d->d[(y * d->width) + x] = c.c;
}

int main(int ac, char *av[])
{
	int x;
	int y;
	int ret;
	t_d d;
	t_circle c;
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
	ret = fscanf(file, "%c %f %f %f %c\n", &c.type, &c.x, &c.y, &c.radius, &c.c);
	while (ret == 5)
	{
		if (!check_c(c))
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
				modify_d(x, y, &d, c);
				x++;
			}
			y++;
		}
		ret = fscanf(file, "%c %f %f %f %c\n", &c.type, &c.x, &c.y, &c.radius, &c.c);
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
