/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhyokki <jhyokki@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 13:41:27 by jhyokki           #+#    #+#             */
/*   Updated: 2025/02/25 11:43:37 by jhyokki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../libft/include/libft.h"
#include "../include/pipex.h"

char	*get_cmd_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;

	if (ft_strchr(cmd, '/') && access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		path = try_path(cmd, paths[i]);
		if (path)
		{
			free_array_of_strings(paths);
			return (path);
		}
		i++;
	}
	free_array_of_strings(paths);
	return (NULL);
}

char	*try_path(char *cmd, char *dir)
{
	char	*full_path;
	char	*path;

	full_path = ft_strjoin(dir, "/");
	if (!full_path)
		return (NULL);
	path = ft_strjoin(full_path, cmd);
	free(full_path);
	if (path && access(path, X_OK) == 0)
		return (path);
	free(path);
	return (NULL);
}

void	execute_cmd1(int fd[2], char **argv, char **envp)
{
	int		fd_in;
	char	**cmd_args;

	fd_in = open(argv[1], O_RDONLY);
	if (fd_in < 0)
	{
		perror("Input file error");
		exit(EXIT_FAILURE);
	}
	dup2(fd_in, STDIN_FILENO);
	close(fd_in);
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	cmd_args = ft_split(argv[2], ' ');
	if (!cmd_args)
	{
		perror("Command split error");
		exit(EXIT_FAILURE);
	}
	execute_command(cmd_args, envp);
}

void	execute_cmd2(int fd[2], char **argv, char **envp)
{
	int		fd_out;
	char	**cmd_args;

	fd_out = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out < 0)
	{
		perror("Output file error");
		exit(EXIT_FAILURE);
	}
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	dup2(fd_out, STDOUT_FILENO);
	close(fd_out);
	cmd_args = ft_split(argv[3], ' ');
	if (!cmd_args)
	{
		perror("Command split error");
		exit(EXIT_FAILURE);
	}
	execute_command(cmd_args, envp);
}

void	execute_command(char **cmd_args, char **envp)
{
	char	*cmd_path;

	cmd_path = get_cmd_path(cmd_args[0], envp);
	if (!cmd_path)
	{
		free_array_of_strings(cmd_args);
		perror("command not found");
		exit(127);
	}
	execve(cmd_path, cmd_args, envp);
	free(cmd_path);
	free_array_of_strings(cmd_args);
	perror("execve failed");
	exit(EXIT_FAILURE);
}
