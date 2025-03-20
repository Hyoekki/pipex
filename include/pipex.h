/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhyokki <jhyokki@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 18:01:26 by jhyokki           #+#    #+#             */
/*   Updated: 2025/03/19 14:39:00 by jhyokki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "../libft/include/libft.h"

char	*get_cmd_path(char *cmd, char **envp);
void	check_file(char *file, const int permission);
void	execute_command(char **cmd_args, char **envp);
void	child_process(int fd[2], char **argv, char **envp);
void	parent_process(int fd[2], char **argv, char **envp);
int		execute_pipeline(int fd[2], char **argv, char **envp);
void	check_command(char *cmd, char **envp);
char	*try_path(char *cmd, char *dir);

#endif
