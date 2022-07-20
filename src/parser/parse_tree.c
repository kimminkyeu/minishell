/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 14:21:40 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/20 18:02:44 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../../include/lexer.h"
#include "parse_tree.h"

t_tree	*new_tree_node(void)
{
	t_tree	*node;

	node = ft_calloc(1, sizeof(*node));
	node->left = NULL;
	node->need_fork = 0;
	node->right = NULL;
	node->redirection = NULL;
	node->token = NULL;
	return (node);
}

/** Helper function for find_top_priority token */
t_list	*find_target_token(t_list *tokens, t_token_type _type)
{
	t_list	*cur;
	t_token	*token;

	cur = tokens;
	while (cur != NULL)
	{
		token = cur->content;
		if (token->type == _type)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

/** 우선순위가 가장 높은 토큰을 찾아서 그 주소를 반환 */
t_list	*find_top_priority_token(t_list *tokens)
{
	t_list	*target;
	int		i;

	/** 가장 연산자 우선순위가 높은 친구를 연속해서 찾는다. */
	/* (1) find && || DOUBLE_AMPERSAND DOUBLE_PIPE */
	i = 0;
	target = NULL;
	while (i < 5) // 총 5개의 타입( 1.[&&] 2.[||] 3.[|] 4.[()] 5.[cmd_or_arg])을 찾는다. 
	{
		target = find_target_token(tokens, i); // enum이라 index 넣어줘도 됨.
		if (target != NULL) // 만약 찾았다면 return
			return (target);
		i++;
	} // 만약 못찾았다면 NULL 반환
	return (target);
}

t_list	*ft_lst_get_prev_node(t_list *node, t_list *target)
{
	t_list	*cur;

	if (node == NULL || target == NULL)
		return (NULL);
	if (node == target)
		return (NULL);
	cur = node;
	while (cur->next != NULL)
	{
		if (cur->next == target)
		{
			return (cur);
		}
		cur = cur->next; // 이제 cur은 target의 이전 노드가 됨.
	}
	return (NULL);
}

/** 노드를 제거하지는 않고, 그냥 리스트에서 뽑아내기만 함. */
void	ft_lst_pop_one(t_list **node, t_list *pop_target)
{
	t_list	*cur;

	if (node == NULL || *node == NULL || pop_target == NULL)
		return ;
	if (*node == pop_target)
	{
		*node = (*node)->next;
		return ;
	}
	cur = *node;

	while (cur != NULL)
	{
		if (cur->next == pop_target)
		{
			/** 이전 노드의 next를 pop_target의 다음노드로 바꿔서, pop_target이 리스트에서 빠진다. */
			cur->next = pop_target->next;
			return ;
		}
		cur = cur->next; // 이제 cur은 target의 이전 노드가 됨.
	}
}

t_list	*collect_redirection_node(t_list *tokens)
{
	/** 주어진 토큰 리스트를 순회하면서, redirection을 (모두) 찾아 하나의 리스트로 갱신 및  반환한다.
	 * NOTE : 이때, 토큰 리스트의 왼쪽부터 순회하면서 발견시 redirection list의 뒤에 순서대로 추가한다.
	 * 이렇게 순서대로 추가해줘야 나중에 적용시 가장 마지막 애가 적용되도록 편하게 로직 작성가능하다.
	 * */
	t_list	*target; // redirection
	t_list	*target_arg; // redirection arg
	t_list	*redir_list;
	t_list	*cur;
	
	/** find redirection pair */
	target = find_target_token(tokens, E_TYPE_REDIRECT); // redirecion 기호(ex. < )
	redir_list = target;
	cur = redir_list; /* 만약 redir이 없으면 null이 들어올 거임.*/
	ft_lst_pop_one(&tokens, target);
	while (target != NULL && target->next != NULL)
	{
		if (target != NULL && target->next != NULL \
				&& ((t_token *)target->next->content)->type == E_TYPE_REDIR_ARG)
		{
			target_arg = target->next;
			cur->next = target_arg;
			ft_lst_pop_one(&tokens, target_arg);
			cur = cur->next;
		}
		target = find_target_token(tokens, E_TYPE_REDIRECT); // redirecion 기호(ex. < )
		cur->next = target; /* 만약 redir이 없으면 null이 들어올 거임.*/
		ft_lst_pop_one(&tokens, target);
		cur = cur->next;
	}
  	return (redir_list);
}

/** TODO : free tree node while execution! */
t_tree *parse_to_tree_recur(t_list *tokens, int need_fork, int is_last_pipe_cmd)
{
	t_tree	*parent;
	t_list	*target_token;
	t_token *target_token_ptr;

	/** 재귀 중단 조건. tokens가 null일 때 까지. */
	if (tokens == NULL)
		return (NULL);

	parent = new_tree_node();
	target_token = find_top_priority_token(tokens); // 잘라낼 타겟 찾기
	
	parent->token = target_token; /** 찾은 타겟의 주소를 노드에 복사(NULL이면 NULL이 복사됨) */
	if (target_token != NULL)
		target_token_ptr = target_token->content;


	if (need_fork == 1 && target_token_ptr->type == E_TYPE_SIMPLE_CMD)
	{
		parent->need_fork = 1;
		if (is_last_pipe_cmd == 1)
			parent->is_last_pipe_cmd = 1;
	}

	/* Set redirection list */
	if (target_token == NULL || target_token_ptr->type == E_TYPE_SIMPLE_CMD \
			|| target_token_ptr->type == E_TYPE_BRACKET)
	{
		parent->redirection = collect_redirection_node(tokens);
	}
	else // if [|] [&&] [||] [( )] ...
	{
		t_list *left_tokens = tokens;
		t_list *right_tokens = target_token->next;

		/** Target_token의 왼쪽 노드와 연결 끊기 */
		t_list	*target_prev = ft_lst_get_prev_node(tokens, target_token);
		if (target_prev != NULL)
			target_prev->next = NULL;

		target_token->next = NULL; /* 오른쪽 노드와 연결 끊기 */

		if (target_token_ptr->type == E_TYPE_PIPE)
		{
			need_fork = 1;
			is_last_pipe_cmd = 1;
		}

		
		/** 재귀 들어가기 */
		parent->left = parse_to_tree_recur(left_tokens, need_fork, 0);
		parent->right = parse_to_tree_recur(right_tokens, need_fork, is_last_pipe_cmd);
	}	
	return (parent);
}

t_tree *parse(t_list *tokens)
{
	return (parse_to_tree_recur(tokens, 0, 0));
}
