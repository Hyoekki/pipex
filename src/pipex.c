/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhyokki <jhyokki@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 18:01:30 by jhyokki           #+#    #+#             */
/*   Updated: 2025/02/24 14:43:53 by jhyokki          ###   ########.fr       */
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
	check_files(argv[1], argv[4]);
	check_commands(argv[2], argv[3], envp);
	if (pipe(fd) == -1)
	{
		perror("Failed to create pipe");
		exit(EXIT_FAILURE);
	}
	return (handle_processes(fd, argv, envp));
}

void	check_files(char *infile, char *outfile)
{
	if (access(infile, R_OK) == -1)
	{
		perror("Input file error");
		exit(EXIT_FAILURE);
	}
	if (access(outfile, F_OK) == 0 && access(outfile, W_OK) == -1)
	{
		perror("Output file error");
		exit (EXIT_FAILURE);
	}
}

void	check_commands(char *cmd1, char *cmd2, char **envp)
{
	char	**cmd1_split;
	char	**cmd2_split;
	char	*cmd1_path;
	char	*cmd2_path;

	cmd1_split = ft_split(cmd1, ' ');
	cmd2_split = ft_split(cmd2, ' ');
	cmd1_path = get_cmd_path(cmd1_split[0], envp);
	cmd2_path = get_cmd_path(cmd2_split[0], envp);
	if (!cmd1_path || !cmd2_path || !cmd1_split || !cmd2_split)
	{
		free(cmd1_path);
		free(cmd2_path);
		free_array_of_strings(cmd1_split);
		free_array_of_strings(cmd2_split);
		perror("command not found");
		exit(127);
	}
	free(cmd1_path);
	free(cmd2_path);
	free_array_of_strings(cmd1_split);
	free_array_of_strings(cmd2_split);
}

int	handle_processes(int fd[2], char **argv, char **envp)
{
	pid_t	pid1;
	pid_t	pid2;
	int		status1;
	int		status2;

	pid1 = fork();
	if (pid1 < 0)
		exit(EXIT_FAILURE);
	if (pid1 == 0)
		execute_cmd1(fd, argv, envp);
	pid2 = fork();
	if (pid2 < 0)
		exit(EXIT_FAILURE);
	if (pid2 == 0)
		execute_cmd2(fd, argv, envp);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status1) && WEXITSTATUS(status1) == 127)
		return (127);
	if (WIFEXITED(status2))
		return (WEXITSTATUS(status2));
	return (EXIT_FAILURE);
}
