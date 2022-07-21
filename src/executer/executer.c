/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 22:15:09 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/22 00:53:56 by minkyeki         ###   ########.fr       */
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
	(void)tok;
	(void)config;

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
		{
			is_double_quote = 0;
			i = 0;
			cnt = 0; // 달러싸인이 나오기 전까지 기호가 몇개인지 알아낸다. 만약 달러싸인 이전에 기호가 홀수개라면 달러를 해석하지 않는다.
			while (tok->str->text[i] != '$' && tok->str->text[i] != '\0')
			{
				if (cnt == 0 && is_double_quote == 0 && tok->str->text[i] == '\"')
					is_double_quote = 1;
				if (tok->str->text[i] == '\'')
					cnt++;
				i++;
			}
			if (is_double_quote == 1 || cnt % 2 == 0) // 달러 이전에 등장하는 쿼트 개수가 홀수라면 달러싸인 확장을 하지 않는다. 
				expand_dollar_sign(tok, config);
			if (is_double_quote == 0)
				tok->str->f_replace_all(tok->str, "\'", "");
			if (is_double_quote == 1)
				tok->str->f_replace_all(tok->str, "\"", "");
		}
		cur = cur->next;
	}
	return (SUCCESS);
}

void	execute_node(t_tree *node, int *status, t_shell_config *config)
{
	/** 아래 코드는 방문 순서를 확인하기 위한 코드일 뿐, 구현시엔 제거할 것. */

	if (*status != CMD_SUCCEESS)
		return ;

	printf("\n\033[93m#Initial tokens\033[0m\n");
	print_tree_node(node->token);
	printf("\n");
	print_tree_node(node->redirection);
	printf("\n");
	printf("fork: %d\n", node->need_fork);
	printf("last_pipe_cmd : %d\n", node->is_last_pipe_cmd);
	

	/* ------------------------------------- 
	 * | NOTE : write execution code here  |
	 * ------------------------------------*/


	/** (0) fork flag가 1이면 fork를 떠서 실행한다. 
	 *      fork flag가 0이면 fork를 뜨지 않는다. */





	/** (0) last_pipe_cmd가 1이면 실행 후 dup2를 원상복구한다. */




	//2. 나중에 추가 -> exec_env에서 쿼트에러 따로 처리해야 한다.! (FIXME!!)

	/** (1) token 노드 단일 확장- Quote Removal
	 *      시작과 끝이 같은 쿼트("") ('')일 경우에만 쿼트 제거. 
	 *      이때 주의사항: export로 넘기는 쿼트 ARG="something"은 쿼트제거하면 안됨.
	 *      + export로 넘긴 쿼트가 서로 쌍을 이루고 있는지 ("" / '') 인지도 검사해야 함.
	 *      안그러면 export로 split 할 때 문제 생김.*/

	/** (2) token 노드 단일 확장- Dollar sign expension
	 *      $가 있을 경우, 환경변수를 순회하면서 해당 환경 변수를 찾아 치환 실행. */
	t_token *tok2 = node->token->content;
	if (tok2->type == E_TYPE_SIMPLE_CMD)
	{

		if (expand_token(node->token, config) == ERROR \
				|| expand_token(node->redirection, config) == ERROR)
		{
			printf("\n\033[91mError while parsing! Skipping Command...\033[0m\n");
			return ; // expand error
		}

		printf("\n\033[93m#After Word Expansion...\033[0m\n");
		print_tree_node(node->token);
		printf("\n");
		print_tree_node(node->redirection);
		printf("\n");


		/** (3) 모든 토큰 리스트를 strjoin으로 사이사이 공백을 넣어 합친다.
		 *      ex. [echo][-n][hello] 는 "echo -n hello" 이렇게 합쳐진다.
		 *      ex. [ec$ECHO!][hi] -> "echo hello! hi" 이렇게 합쳐진다.
		 **/
		printf("\n\033[93m#Making char **arglist...\033[0m\n");
		t_string *joined_str = new_string(50);
		t_list *cur = node->token;
		while (cur != NULL)
		{
			t_token *tok = cur->content;
			joined_str->f_append(joined_str, tok->str->text); // 토큰 삽입
			joined_str->f_push_back(joined_str, ' '); // 공백 삽입
			cur = cur->next;
		}

		/** (4) join된 문자열을 공백 기준으로 split하면 char **arglist가 한번에 구해진다. */
		char **arglist = ft_split(joined_str->text, ' ');
		delete_string(&joined_str);


		/** printing arglist (print_strs is at in environ.c) */
		print_strs(arglist);
		printf("\n");


		/** Execute command...  */
		/** (5) arglist[0]이 is_builtin()에 걸리면 builtin으로, 
		 *      안걸리면 execve 함수로 생성된 arglist 를 넘겨준다. */
		printf("\n\033[94m#Executing command...\033[0m\n");
		// ...



		/** after running command, delete arglist */
		delete_strs(&arglist);
	}
	
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
