/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 22:15:09 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/21 15:09:53 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../parser/parse_tree.h"
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

void	execute_node(t_tree *node, int *status, t_shell_config *config)
{
	/** 아래 코드는 방문 순서를 확인하기 위한 코드일 뿐, 구현시엔 제거할 것. */

	if (*status != CMD_SUCCEESS)
		return ;

	print_tree_node(node->token);
	printf("\n");
	print_tree_node(node->redirection);
	printf("\n");
	printf("fork: %d\n", node->need_fork);
	printf("last_pipe_cmd : %d\n", node->is_last_pipe_cmd);
	printf("\n\033[93mExecuting command...\033[0m\n\n");
	

	/* ------------------------------------- 
	 * | NOTE : write execution code here  |
	 * ------------------------------------*/

	(void)config; // --> 나중에 사용할 예정.

	/** (0) fork flag가 1이면 fork를 떠서 실행한다. 
	 *      fork flag가 0이면 fork를 뜨지 않는다. */





	/** (0) last_pipe_cmd가 1이면 실행 후 dup2를 원상복구한다. */





	/** (1) token 노드 단일 확장- Quote Removal
	 *      시작과 끝이 같은 쿼트("") ('')일 경우에만 쿼트 제거. 
	 *      이때 주의사항: export로 넘기는 쿼트 ARG="something"은 쿼트제거하면 안됨.
	 *      + export로 넘긴 쿼트가 서로 쌍을 이루고 있는지 ("" / '') 인지도 검사해야 함.
	 *      안그러면 export로 split 할 때 문제 생김.*/
	





	/** (2) token 노드 단일 확장- Dollar sign expension
	 *      $가 있을 경우, 환경변수를 순회하면서 해당 환경 변수를 찾아 치환 실행. */






	/** (3) 모든 토큰 리스트를 strjoin으로 사이사이 공백을 넣어 합친다.
	 *      ex. [echo][-n][hello] 는 "echo -n hello" 이렇게 합쳐진다.
	 *      ex. [ec$ECHO!][hi] -> "echo hello! hi" 이렇게 합쳐진다.
	 **/





	/** (4) join된 문자열을 공백 기준으로 split하면 char **arglist가 한번에 구해진다. */






	/** (5) arglist[0]이 is_builtin()에 걸리면 builtin으로, 
	 *      안걸리면 execve 함수로 생성된 arglist 를 넘겨준다. */






	
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
