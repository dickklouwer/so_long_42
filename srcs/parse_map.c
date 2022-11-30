/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_map.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tklouwer <tklouwer@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/15 15:55:24 by tklouwer      #+#    #+#                 */
/*   Updated: 2022/11/30 14:53:10 by tklouwer      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

/* 
** Checks if the map is surrounded with walls.
** If not, the function exits with an error message.
*/
static int	st_wall_check(t_map *map, char *line, int y)
{
	int	i;

	i = 0;
	if (y == 0 || y == map->height - 1)
	{
		while (line[i])
		{
			if (line[i] != '1')
				error("INVALID BORDERS");
			i++;
		}
	}
	else if (line[0] != '1' || line[map->width - 1] != '1')
		error("INVALID BORDERS");
	return (EXIT_SUCCESS);
}

/* 
** Scans the map and stores the data.
** If the data is not right, the function exits with an error message.
*/
static int	st_map_data(t_game *game, t_map *map, char *line, int y)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if ((line[i] != 'P') && (line[i] != 'E') && (line[i] != 'C')
			&& (line[i] != '0') && (line[i] != '1'))
			error("INVALID MAP TILE");
		if (line[i] == 'E')
			map->exit++;
		if (line[i] == 'P')
		{
			game->player.x = i;
			game->player.y = y;
			map->start++;
		}
		if (line[i] == 'C')
			map->coll++;
		i++;
	}
	if (i != map->width)
		error("INVALID WIDTH");
	return (EXIT_SUCCESS);
}

/*  
** Checks the mandatory data in the map.
** If the data is not right, the function exits with an error message.
*/
int	st_check_data(t_map *map)
{
	if (map->exit != 1 || map->start != 1)
		error("NO START/END");
	if (map->coll < 1)
		error("NO POKÉ BALLS");
	if (map->width > 41)
		error("WINDOW WIDTH EXCEEDED");
	return (EXIT_SUCCESS);
}

/* 
** Reads the map for the first time to store the Height & Width.
*/
static int	st_pre_check(t_map *map, char *file)
{
	int		i;
	int		fd;
	char	*line;

	i = 0;
	fd = open(file, O_RDONLY);
	if (fd < 0)
		error("COULDN'T READ MAP");
	while (get_next_line(fd, &line) == 1)
	{
		free(line);
		i++;
	}
	free(line);
	map->height = i + 1;
	close(fd);
	return (EXIT_SUCCESS);
}

/* 
** Parses the map with help of the above functions.
** If something is invalid, the program will exit.
*/
int	parse_map(t_game *game, char *file)
{
	int	fd;
	int	i;

	i = 0;
	if (st_pre_check(game->map, file))
		return (EXIT_FAILURE);
	game->map->map = (char **)ft_calloc(game->map->height + 1, sizeof(char *));
	if (!game->map->map)
		return (EXIT_FAILURE);
	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (EXIT_FAILURE);
	get_next_line(fd, &game->map->map[i]);
	game->map->width = ft_strlen(game->map->map[i]);
	while (i < game->map->height)
	{
		st_wall_check(game->map, game->map->map[i], i);
		st_map_data(game, game->map, game->map->map[i], i);
		i++;
		get_next_line(fd, &game->map->map[i]);
	}
	if (flood_fill(game->player.x, game->player.y, game->map))
		exit(1);
	close(fd);
	return (st_check_data(game->map));
}
