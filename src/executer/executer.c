/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 22:15:09 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/22 15:23:55 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"

void	delete_tree_node(t_tree *node, int *status, t_shell_config *config)
{
	(void)status;
	(void)config;
	if (node != NULL)
	{
		if ((node)->redirection != NULL)
			ft_lstclear(&node->redirection, delete_token);
		if ((node)->token != NULL)
			ft_lstclear(&node->token, delete_token);
		free(node);

		printf("\033[90mnode deleted\033[0m\n");
	}
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

void	execute_node(t_tree *node, int *status, t_shell_config *config)
{
	/** 아래 코드는 방문 순서를 확인하기 위한 코드일 뿐, 구현시엔 제거할 것. */

	if (*status != CMD_SUCCEESS)
		return ;

	print_tree_node(node->token);
	printf("\n");
	print_tree_node(node->redirection);
	printf("\n");
	printf("fork: %d\n", node->is_pipeline);
	printf("last_pipe_cmd : %d\n", node->is_last_pipe_cmd);
	printf("\n\033[93mExecuting command...\033[0m\n\n");


	/* -------------------------------------
	 * | NOTE : write execution code here  |
	 * ------------------------------------*/

	(void)config; // --> 나중에 사용할 예정.


// (0) void 쿼트와_달러사인_확장(t_list tokenlist);
	/** (1) token 노드 단일 확장- Quote Removal
	 *      시작과 끝이 같은 쿼트("") ('')일 경우에만 쿼트 제거.
	 *      이때 주의사항: export로 넘기는 쿼트 ARG="something"은 쿼트제거하면 안됨.
	 *      + export로 넘긴 쿼트가 서로 쌍을 이루고 있는지 ("" / '') 인지도 검사해야 함.
	 *      안그러면 export로 split 할 때 문제 생김.*/

	/** (2) token 노드 단일 확장- Dollar sign expension
	 *      $가 있을 경우, 환경변수를 순회하면서 해당 환경 변수를 찾아 치환 실행. */

// (0) char **cmd_argv = wordsplit과_이차원배열화(t_list tokenlist);
	/** (3) 모든 토큰 리스트를 strjoin으로 사이사이 공백을 넣어 합친다.
	 *      ex. [echo][-n][hello] 는 "echo -n hello" 이렇게 합쳐진다.
	 *      ex. [ec$ECHO!][hi] -> "echo hello! hi" 이렇게 합쳐진다.
	 **/

	/** (4) join된 문자열을 공백 기준으로 split하면 char **arglist가 한번에 구해진다. */

// (0) 명령어에 따라 분기하여 실행
	/* 전 과정: (pipe) - (fork) - redirection set - execute - (redirection restore)
	** 분기:
	** 1. 파이프라인이 아님 && cd나 exit: redir set - execute - redir restore
	** 2. 나머지 builtin: (pipe) - fork - redirection set - execute(status 수동)
	** 3. filesystem에서 실행하는 cmd: (pipe) - fork - redirection set - execute(status 자동)
	*/
	char **cmd_argv;


	if (is_builtin() == true)
	{
		if (node->is_pipeline == false &&
		(ft_strncmp(cmd_argv[0], "cd", 3) || ft_strncmp(cmd_argv[0], "exit", 5)))
			exec_builtin_exception(cmd_argv);
		else
			exec_builtin_general(cmd_argv);
	}
	else
		exec_general(cmd_argv);


	/** NOTE : if success, then set status to ... CMD_SUCCESS
	 *         else, set status to ...            CMD_FAILURE
	 *         if exit, then set status to ...    CMD_STOP_SHELL
	 * */

	/** *status = CMD_SUCCEESS; */
	/** *status = CMD_STOP_SHELL; */
	/** *status = CMD_FAILURE; */
	/** printf("\033[31mCMD_FAILURE set, stopping execution...\033[0m\n"); */
	/** NOTE : 위 코드는 확인용이니 꼭 지울 것. */
	printf("-------------------\n");
}
/** 함수 포인터 글자수 줄이는 용도 */
typedef void(*t_callback_func)(t_tree *, int *, t_shell_config *);

void	inorder_recur(t_tree *node, int *status, t_callback_func callback, t_shell_config *shell_config)
{
	/** status가 몇일 때 어떤 행동을 할지는 구현할 때 정하기 */
	if (node == NULL)
		return ;

	/** (1) if status == CMD_STOP_SHELL (ex. calling exit) then stop all */

	/** NOTE : 노드 삭제 함수의 경우 에러 여부와 관계 없이 순회해야함.  */
	if (callback != delete_tree_node && *status == CMD_SUCCEESS)
	{
		inorder_recur(node->left, status, callback, shell_config);
		callback(node, status, shell_config);
		inorder_recur(node->right, status, callback, shell_config);
	}
	else if (callback == delete_tree_node)
	{
		inorder_recur(node->left, status, callback, shell_config);
		inorder_recur(node->right, status, callback, shell_config);
		callback(node, status, shell_config);
	}
}

/** Executer function. make traversing */
int	execute(t_tree *syntax_tree, t_shell_config *config)
{
	int	status;

	(void)config;

	status = CMD_SUCCEESS;

	printf("\n");
	printf("------------------------------------------\n");
	printf("|     Execution (inorder recursive)      |\n");
	printf("------------------------------------------\n");

	/** 모든 노드 실행 */
	inorder_recur(syntax_tree, &status, execute_node, config);

	/** 모든 노드 삭제 */
	inorder_recur(syntax_tree, &status, delete_tree_node, config);

	return (status);
}
