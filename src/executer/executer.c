/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 22:15:09 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/17 22:52:21 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../parser/parse_tree.h"
#include "executer.h"

void	delete_tree_node(t_tree **node)
{
	if (*node != NULL)
	{
		if ((*node)->redirection != NULL)
			ft_lstclear(&((*node)->redirection), delete_token);
		if ((*node)->token != NULL)
			ft_lstclear(&((*node)->token), delete_token);
		printf("check\n");
	}
}

int	execute_node(t_tree *node)
{
	/** 아래 코드는 방문 순서를 확인하기 위한 코드일 뿐, 구현시엔 제거할 것. */
	print_tree_node(node->token);
	printf("\n");
	print_tree_node(node->redirection);
	printf("\n");
	printf("-------------------");
	printf("\n\n");


	/** NOTE : write execution code here */


	// code here ...



	/** NOTE : 만약 해당 노드를 실행했다면, node 자원 해제하기 */
	delete_tree_node(&node);

	return (1);
}


void	inorder_recur(t_tree *node, int *status)
{
	/** status가 몇일 때 어떤 행동을 할지는 구현할 때 정하기 */
	if (node == NULL || *status == CMD_FAILURE)
		return ;
	inorder_recur(node->left, status);
	*status = execute_node(node);
	inorder_recur(node->right, status);
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

	/** 실행 도중 에러 발생시, status 변수를 0으로 변경. */
	inorder_recur(syntax_tree, &status);

	return (status);
}
