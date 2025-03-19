/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhyokki <jhyokki@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 18:01:30 by jhyokki           #+#    #+#             */
/*   Updated: 2025/03/19 14:38:53 by jhyokki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../libft/include/libft.h"
#include "../include/pipex.h"

int	main(int argc, char **argv, char **envp)
{
	int	fd[2];

	if (argc != 5)
	{
		perror("Invalid number of arguments, expected 5");
		exit(EXIT_FAILURE);
	}
	check_file(argv[1], R_OK);
	check_command(argv[2], envp);
	check_command(argv[3], envp);
	if (pipe(fd) == -1)
	{
		perror("Failed to create pipe");
		exit(EXIT_FAILURE);
	}
	return (execute_pipeline(fd, argv, envp));
}

void	check_file(char *file, const int permission)
{
	if (access(file, permission) == -1)
	{
		perror("File error");
		exit(EXIT_FAILURE);
	}
}

void	check_command(char *cmd, char **envp)
{
	char	**cmd_split;
	char	*cmd_path;

	cmd_split = ft_split(cmd, ' ');
	cmd_path = get_cmd_path(cmd_split[0], envp);
	if (!cmd_path || !cmd_split || !cmd_split)
	{
		free(cmd_path);
		free_array_of_strings(cmd_split);
		perror("command not found");
		exit(127);
	}
	free(cmd_path);
	free_array_of_strings(cmd_split);
}

int	execute_pipeline(int fd[2], char **argv, char **envp)
{
	pid_t	pid1;
	pid_t	pid2;
	int		status1;
	int		status2;

	pid1 = fork();
	if (pid1 < 0)
		exit(EXIT_FAILURE);
	if (pid1 == 0)
		child_process(fd, argv, envp);
	pid2 = fork();
	if (pid2 < 0)
		exit(EXIT_FAILURE);
	if (pid2 == 0)
		parent_process(fd, argv, envp);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if ((status1 & 0xFF) == 0 && ((status1 >> 8) & 0xFF) == 127)
		return (127);
	if ((status2 & 0xFF) == 0)
		return ((status2 >> 8) & 0xFF);
	return (EXIT_FAILURE);
}
