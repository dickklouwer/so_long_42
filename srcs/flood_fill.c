/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   flood_fill.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: tklouwer <tklouwer@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/30 09:01:37 by tklouwer      #+#    #+#                 */
/*   Updated: 2022/11/30 15:00:33 by tklouwer      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

int	flood_fill(int x, int y, t_map *map)
{
	if ((x < 0) || (x >= map->width) || (y < 0) || (y >= map->height))
		return (1);
	if (map->map[y][x] == 'P' || map->map[y][x] == 'E' || map->map[y][x] == '0'
		|| map->map[y][x] == 'C')
		return (0);
	if ((x + 1 < map->width && map->map[y][x + 1] == ' ')
		|| (x - 1 >= 0 && map->map[y][x - 1] == ' ') || (y + 1
		< map->height && map->map[y + 1][x] == ' ') || (y - 1
		>= 0 && map->map[y - 1][x] == ' '))
		error("Map not surrounded by walls.");
	if (x == 0 || y == 0 || y == map->height - 1
		|| x == map->width - 1)
		error("Map not surrounded by walls.");
	if (flood_fill(x - 1, y, map) == 1)
		return (1);
	if (flood_fill(x, y - 1, map) == 1)
		return (1);
	if (flood_fill(x + 1, y, map) == 1)
		return (1);
	if (flood_fill(x, y + 1, map) == 1)
		return (1);
	return (0);
}
