/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 22:15:09 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/20 18:03:58 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../parser/parse_tree.h"
#include "executer.h"

void	delete_tree_node(t_tree *node, int *status)
{
	(void)status;
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

void	execute_node(t_tree *node, int *status)
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
	

	/** NOTE : write execution code here */

	
	// Write code here ...
	printf("-------------------\n");
	/** TODO : if execution is exit, then set state to CMD_STOP_SHELL */

	/** NOTE : if success, then set status to ... CMD_SUCCESS
	 *         else, set status to ...            CMD_FAILURE 
	 *         if exit, then set status to ...    CMD_STOP_SHELL 
	 * */

	/** *status = CMD_SUCCEESS; */
	/** *status = CMD_STOP_SHELL; */
	/** *status = CMD_FAILURE; */
	/** printf("\033[31mCMD_FAILURE set, stopping execution...\033[0m\n"); */
	/** NOTE : 위 코드는 확인용이니 꼭 지울 것. */
}


void	inorder_recur(t_tree *node, int *status, void (*callback)(t_tree *node, int *status))
{
	/** status가 몇일 때 어떤 행동을 할지는 구현할 때 정하기 */
	if (node == NULL)
		return ;

	/** (1) if status == CMD_STOP_SHELL (ex. calling exit) then stop all */

	/** NOTE : 노드 삭제 함수의 경우 에러 여부와 관계 없이 순회해야함.  */
	if (callback != delete_tree_node && *status == CMD_SUCCEESS)
	{
		inorder_recur(node->left, status, callback);
		callback(node, status);
		inorder_recur(node->right, status, callback);
	}
	else if (callback == delete_tree_node)
	{
		inorder_recur(node->left, status, callback);
		inorder_recur(node->right, status, callback);
		callback(node, status);
	}
}

/** Executer function. make traversing */
int	execute(t_tree *syntax_tree)
{
	int	status;

	status = CMD_SUCCEESS;

	printf("\n");
	printf("------------------------------------------\n");
	printf("|     Execution (inorder recursive)      |\n");
	printf("------------------------------------------\n");

	/** 모든 노드 실행 */
	inorder_recur(syntax_tree, &status, execute_node);

	/** 모든 노드 삭제 */
	inorder_recur(syntax_tree, &status, delete_tree_node);

	return (status);
}
