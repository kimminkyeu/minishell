/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand_wildcard.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 21:05:08 by minkyeki          #+#    #+#             */
/*   Updated: 2022/08/02 15:15:27 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_EXPAND_WILDCARD_H
# define TOKEN_EXPAND_WILDCARD_H

# include <sys/types.h>
# include <dirent.h>
# include <sys/stat.h>
# include "token_expand.h"
# include "executer.h"
# include "../parser/parse_tree.h"

/* Main Wildcard function  */
int		expand_wildcard_glob(t_list *tokens, t_shell_config *config);

/* Helper functions. */
void	get_prefix(t_iterator *iter, t_string *prefix);
void	get_suffix(t_iterator *iter, t_string *suffix);
int		ft_strncmp_reverse(const char *s1, const char *s2, size_t n);
void	set_path_and_run_stat(struct dirent *file, char *path, \
		struct stat *buf);
void	match_pattern(struct dirent *file, struct stat *buf, t_list **match, \
		t_pattern *pattern);
t_list	*match_path_and_return_list(char *path, t_pattern *pattern);
void	set_pattern_path(t_token *tok, t_pattern *pattern, t_iterator *iter);
void	set_pattern_prefix_suffix_remainder(t_pattern *pattern, \
		t_iterator *iter);
void	set_pattern(t_token *tok, t_pattern *pattern, t_shell_config *config);
void	init_pattern(t_pattern *pattern);
void	delete_pattern(t_pattern *pattern);
t_list	*expand_single_wildcard(t_token *tok, t_shell_config *config);
int		has_wild_card(t_list *cur_token);
t_list	*expand_wildcard_glob_and_return_list(t_list *cur_token, \
		t_shell_config *config, int *is_error);
int		expand_wildcard_glob_once(t_list *cur, t_shell_config *config, \
		int *is_error);
int		has_wild_card_in_list(t_list *tokens);

#endif /* TOKEN_EXPAND_WILDCARD_H */
