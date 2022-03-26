/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@students.21-school.ru>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 15:07:41 by lalex             #+#    #+#             */
/*   Updated: 2022/03/23 22:45:07 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <unistd.h>

typedef struct s_child
{
	char	**args;
	pid_t	pid;
	int		status;
	int		fds[2];
}	t_child;

char	**split_args(char const *s);
char	**ft_split(char const *s, char c);
void	clean_strings(char **splitted);
int		ft_isspace(int c);
int		ft_isquote(int c);
void	close_pipe(int fds[2]);
size_t	ft_strlen(const char *str);
// Returns 0 if `str2` is not the beginning of `str1`
// Returns 1 otherwise
int		starts_with(char *str1, char *str2);
// Compares not more than `n` symbols from strings `s1` and `s2`
// Returns value above zero, zero or value below zero
// if `s1` is greater than, equal or less than `s2`
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*path_join(const char *dir, const char *file);
int		printf_err(const char *s, ...);
char	**env_path(char **envp, int command);
char	**get_paths(char **envp);
char	**parse_command(const char *command);
int		execute_commands(int fds[2], char **commands, int cmdc);

t_child *create_child(int in_fd, int out_fd, char *cmd);
int		execute_child(t_child *child);
int		destroy_child(t_child *child);
int		wait_child(t_child *child);

# define HERE_DOC	"here_doc"

# define BAD_USAGE_ERROR		"Bad usage\nTry '%s file1 cmd1 cmd2 file2'\n"
# define INPUT_FILE_ERROR		"Invalid input file %s: %s\n"
# define FILE_ERROR				"pipex: %s: %s\n"
# define OUTPUT_FILE_ERROR		"Invalid output file %s: %s\n"
# define PIPE_CREATION_ERROR	"Failed to create pipe: %s\n"
# define FORK_CREATION_ERROR	"Fork is dropped: %s\n"
# define INVALID_COMMAND_ERROR	"Command not found: %s\n"

# define PATH_INIT	0b0
# define PATH_READ	0b1
# define PATH_CLEAN	0b10

# define FD_CHANGE_ERROR	0b1
# define FORK_ERROR			0b10
# define ARG_PARSING_ERROR	0b100
# define CHILD_ERROR		0b1000

# define EXIT_CMD_NOT_FOUND	127

#endif