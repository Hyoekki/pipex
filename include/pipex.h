/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhyokki <jhyokki@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 18:01:26 by jhyokki           #+#    #+#             */
/*   Updated: 2025/02/24 15:08:38 by jhyokki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include "../libft/include/libft.h"

char		*get_cmd_path(char *cmd, char **envp);
void		check_files(char *infile, char *outfile);
void		execute_command(char **cmd_args, char **envp);
void		execute_cmd1(int fd[2], char **argv, char **envp);
void		execute_cmd2(int fd[2], char **argv, char **envp);
int			handle_processes(int fd[2], char **argv, char **envp);
void		check_commands(char *cmd1, char *cmd2, char **envp);
static char	*try_path(char *cmd, char *dir);

#endif
