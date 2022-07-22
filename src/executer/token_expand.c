/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 15:44:57 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/22 16:03:08 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_expand.h"
#include "executer.h"

char	**get_arglist(t_list *token)
{
	t_string	*joined_str;
	t_list		*cur;
	char		**arglist;

	/** (3) 모든 토큰 리스트를 strjoin으로 사이사이 공백을 넣어 합친다.
	 *      ex. [echo][-n][hello] 는 "echo -n hello" 이렇게 합쳐진다.
	 *      ex. [ec$ECHO!][hi] -> "echo hello! hi" 이렇게 합쳐진다.
	 **/
	printf("\n\033[93m#Making char **arglist...\033[0m\n");

	joined_str = new_string(50);
	cur = token;
	while (cur != NULL)
	{
		t_token *tok = cur->content;
		joined_str->f_append(joined_str, tok->str->text); // 토큰 삽입
		joined_str->f_push_back(joined_str, ' '); // 공백 삽입
		cur = cur->next;
	}
	/** (4) join된 문자열을 공백 기준으로 split하면 char **arglist가 한번에 구해진다. */
	arglist = ft_split(joined_str->text, ' ');
	delete_string(&joined_str);

	/**TODO : delete later!! 
	 * printing arglist (print_strs is at in environ.c) */
	print_strs(arglist);
	printf("\n");

	return (arglist);
}


void	expand_dollar_sign(t_token *tok, t_shell_config *config)
{

	/** ARG=hello 형식 */
	/** use str_replace_all(str, "$ARG", "hello") */
	/** 1. 달러 싸인을 반복해서 찾아 그 위치를 파악한다. (여러가지 있을 수 있으니) */

	char	*ptr_start;
	char	*ptr_end;
	char	*replace_str;
	char	*env_key;

	/** 만약 달러싸인이 없다면 포인터 둘다 null문자를 가르키고 있을 것임.  */
	while (true)
	{
		ptr_start = tok->str->text;
		while (*ptr_start != '\0' && *ptr_start != '$')
			ptr_start++;

		ptr_end = ptr_start;
		if (*ptr_end != '\0')
			ptr_end++;

		while (*ptr_end != '\0' && *ptr_end != '$' && ft_isalnum(*ptr_end))
			ptr_end++;
		ptr_end--;

		/** printf("p1:%c, p2:%c\n", *ptr_start, *ptr_end); */
		if (*ptr_start == '\0')
			return ;

		/** 달러싸인 치환 시작 */
		env_key = ft_substr(tok->str->text, ptr_start + 1 - tok->str->text, ptr_end - ptr_start);
		if (*env_key != '\0')
		{
			/** printf("env_key : %s\n", env_key); */
			/** get_environ_value에서 null을 주면, 그냥 치환을 빈 문자로 해줘야 한다. */
			replace_str = get_environ_value(env_key, config->envp);
			/** printf("replace : %s\n", replace_str); */
			if (replace_str == NULL)
				tok->str->f_replace(tok->str, ptr_start - tok->str->text, ptr_end - ptr_start + 1, "");
			else
				tok->str->f_replace(tok->str, ptr_start - tok->str->text, ptr_end - ptr_start + 1, replace_str);
		}
		free(env_key);
	}
}

/** expand (1. quote removal, 2. dollar sign expansion) */
int	expand_token(t_list *tokens, t_shell_config *config)
{
	t_list	*cur;
	t_token *tok;
	size_t	cnt;
	size_t	i;

	int		is_double_quote;

	cur = tokens;
	while (cur != NULL)
	{
		tok = cur->content;
		if (tok->str->text[0] == '\"' && tok->str->text[tok->str->text_len - 1] == '\"')
		{
			expand_dollar_sign(tok, config);
			tok->str->f_replace_all(tok->str, "\"", "");
		}
		else if (tok->str->text[0] == '\'' && tok->str->text[tok->str->text_len - 1] == '\'') // NOTE : 이 부분은 처리할 게 몇게 더 있다.
		{
			i = 0;
			cnt = 0; // 달러싸인이 나오기 전까지 기호가 몇개인지 알아낸다. 만약 달러싸인 이전에 기호가 홀수개라면 달러를 해석하지 않는다.
			while (tok->str->text[i] != '$' && tok->str->text[i] != '\0')
			{
				if (tok->str->text[i] == '\'')
					cnt++;
				i++;
			}
			if (cnt % 2 == 0) // 달러 이전에 등장하는 쿼트 개수가 홀수라면 달러싸인 확장을 하지 않는다. 
				expand_dollar_sign(tok, config);
			tok->str->f_replace_all(tok->str, "\'", "");
		}
		else if (tok->str->text[0] == '\"' && tok->str->text[tok->str->text_len - 1] != '\"')
			return (ERROR);
		else if (tok->str->text[0] == '\'' && tok->str->text[tok->str->text_len - 1] != '\'')
			return (ERROR);
		else
			// NOTE : 여기부터는 토큰의 양끝으로 쿼트가 묶이지 않았을때, 특히 export를 위한 부분이다.
		{
			is_double_quote = 0;
			i = 0;
			cnt = 0; // 달러싸인이 나오기 전까지 기호가 몇개인지 알아낸다. 만약 달러싸인 이전에 기호가 홀수개라면 달러를 해석하지 않는다.
			while (tok->str->text[i] != '$' && tok->str->text[i] != '\0')
			{
				/** export T=''$HOME'' 은 확장 되어야 한다.  */
				if ( cnt == 0 && is_double_quote == 0 && tok->str->text[i] == '\"')
					is_double_quote = 1;
				if (tok->str->text[i] == '\'')
					cnt++;
				i++;
			}
			if (is_double_quote == 1 || cnt % 2 == 0) // 달러 이전에 등장하는 쿼트 개수가 홀수라면 달러싸인 확장을 하지 않는다. 
				expand_dollar_sign(tok, config);
			if (is_double_quote == 0)
			{
				/** export에서 싱글쿼트에 쌍이 안맞는 경우 */
				tok->str->f_replace_all(tok->str, "\'", "");
				// 쿼트 제거 후 항상 마지막 문자는 isalnum에 걸릴 것이다.
			}
			if (is_double_quote == 1)
			{
				/** 만약 export에서 쿼트를 지우는데 그 쌍이 안맞을 경우 */
				tok->str->f_replace_all(tok->str, "\"", "");
			}
			// 만약 모든 과정이 끝난 뒤 마지막 문자가 al_num이 아니라면, 에러처리.
			if (!ft_isalnum(tok->str->text[tok->str->text_len - 1]))
				return (ERROR);
		}
		cur = cur->next;
	}
	return (SUCCESS);
}
