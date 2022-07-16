/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 10:02:06 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/16 17:18:00 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/** Standard library */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
/** Readline library */
#include <readline/readline.h>
#include <readline/history.h>

/** Our Source header */
/* (1) Lexer header */
#include "../../include/lexer.h"


/** (2) helper functions. delete later */
#include "helper.h"

/** typedef enum e_token_type {
  *                                 //
  *     E_TYPE_DOUBLE_AMPERSAND,	//	[&&]
  *     E_TYPE_DOUBLE_PIPE, 		//  [||]
  *     E_TYPE_PIPE,				//	[|]
  *     E_TYPE_BRACKET,				//  [(] [)] --> for subshell
  *     E_TYPE_CMD_OR_ARG,			// simple_command
  *                                 //
  *     E_TYPE_CMD_OPTION,			//  [-*]
  *     E_TYPE_REDIRECT,			//	[< << > >>]
  *     E_TYPE_SINGLE_QUOTE,		//  [']
  *     E_TYPE_DOUBLE_QUOTE,		//  ["]
  *                                 //
  *     E_TYPE_DEFAULT, 			// No type
  *     // E_TYPE_SEMICOLON,		//	[;]
  *     // E_TYPE_EOF,				//  [ EOF special token like NULL ]
  *     // E_TYPE_ERROR				//  [ Error token. ]
  * }	t_token_type; */

/** 
 * NOTE : simple_cmd의 형식은 항상 [cmd]-[option]-[arg]-[arg...]
 *
 *
 * */

typedef struct s_tree	t_tree;

typedef struct s_tree {
	/** t_token_type	type; */
	/** free 과정 단순화를 위해 t_list 노드로 통일. */
	t_list		*token; // list of tokens
	t_list		*redirection;
								  
	t_tree		*left;
	t_tree		*right;
}	t_tree;

t_tree	*new_tree_node(void)
{
	t_tree	*node;

	node = ft_calloc(1, sizeof(*node));
	node->left = NULL;
	node->right = NULL;
	/** node->redirection = NULL; */
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
			break ;
		cur = cur->next;
	}
	return (cur);
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
	while (i < 5) // 총 5개의 타입( 1.[&&] 2.[||] 3.[|] 4.[()] 5.[cmd_or_arg] )을 찾는다. 
	{
		target = find_target_token(tokens, i); // enum이라 index 넣어줘도 됨.
		if (target != NULL) // 만약 찾았다면 return
			return (target);
		i++;
	} // 만약 못찾았다면 NULL 반환
	return (target);
}


/** 노드를 제거하지는 않고, 그냥 리스트에서 뽑아내기만 함. */
void	ft_lst_pop_one(t_list *node, t_list *pop_target)
{
	t_list	*cur;

	if (node == NULL || pop_target == NULL)
		return ;
	cur = node;
	while (cur->next != pop_target)
		cur = cur->next; // 이제 cur은 target의 이전 노드가 됨.
	
	/** 이전 노드의 next를 pop_target의 다음노드로 바꿔서, pop_target이 리스트에서 빠진다. */
	cur->next = pop_target->next;
}


t_list	*collect_redirection_node(t_list *tokens)
{
	/** 주어진 토큰 리스트를 순회하면서, redirection을 (모두) 찾아 하나의 리스트로 갱신 및  반환한다.
	 *
	 * NOTE : 이때, 토큰 리스트의 왼쪽부터 순회하면서 발견시 redirection list의 뒤에 순서대로 추가한다.
	 * 
	 * 이렇게 순서대로 추가해줘야 나중에 적용시 가장 마지막 애가 적용되도록 편하게 로직 작성가능하다.
	 * */
	t_list	*target;
	t_list	*redir_list;
	t_list	*cur;
	
	/* FIXME : 이 경우 malloc 을 해줘야 하나? .. 해줄 필요가 없어 보이는데...*/

	// (0) 리다이렉션을 왼쪽부터 계속 찾는다.
	redir_list = NULL;
	target = find_target_token(tokens, E_TYPE_REDIRECT);
	ft_lst_pop_one(tokens, target); // tokens 리스트에서 링크 해제
	redir_list = target; // 첫 시작이니까 list에 target을 대입
	cur = redir_list; // cur은 노드 주소 추가용 변수
	while (target != NULL)
	{
		target = find_target_token(tokens, E_TYPE_REDIRECT);
		ft_lst_pop_one(tokens, target);
		cur->next = target; // add to redir_list
		cur = cur->next; // move pointer to next.
	}
	return (redir_list); // 만약 redirection을 못찾으면 target이 null이므로 null이 반환된다.
}

/** TODO : free tree node while execution! */
t_tree *parse_to_tree_recur(t_list *tokens)
{
	t_tree	*parent;
	t_list	*target_token;
	t_token *target_token_ptr;


	/** 재귀 중단 조건. tokens가 null일 때 까지. */
	if (tokens == NULL)
		return (NULL);

	printf("here1\n");

	parent = new_tree_node();

	printf("here2\n");

	/** 잘라낼 높은 우선순위 연산자 찾기 (&& || | ...) */
	/** null이 나올 때 까지 token 리스트를 순회  */
	target_token = find_top_priority_token(tokens);

	printf("here3\n");

	/** 찾은 타겟의 주소를 노드에 복사 */
	parent->token = target_token;

	/** 캐스팅 귀찮음 해결을 위한 변수 사용. */
	target_token_ptr = target_token->content;

	// set redirection list. cmd일때만 redirection list가 존재함.
	
	if (target_token_ptr->type == E_TYPE_CMD_OR_ARG)
	{
		parent->redirection = collect_redirection_node(tokens);

		printf("here4\n");
	}

	/** t_list *left_tokens = tokens; */
	/** t_list *right_tokens = target_token->next; */
	target_token->next = NULL; // 오른쪽 노드와 연결 끊기
	
	/** 재귀적으로 들어가기 */
	/** parent->left = parse_to_tree_recur(left_tokens); */
	/** parent->right = parse_to_tree_recur(right_tokens); */

	return (parent);
}

/** NOTE : below functions are for tree checking
 * ---------------------------------------------------------*/
void	print_tree_node(t_list *token)
{
	t_list	*tmp;

	if (token == NULL)
		printf("[null]");
	tmp = token;
	/** printf("[%s]", get_token_type(tok->type)); */
	while (tmp != NULL)
	{
		t_token *tok = tmp->content;
		printf("[%s]", tok->str->text);
		tmp = tmp->next;
	}
}

/** tree print utils (for checking) */
void	print_tree_2d(t_tree *root, int space)
{
	const int COUNT = 10;
	

	if (root == NULL)
		return ;

	space += COUNT;

	print_tree_2d(root->right, space);
	
	printf("\n");
	int i = 0;
	while (i < COUNT)
	{
		printf(" ");
		i++;
	}

	printf("token: ");
	print_tree_node(root->token);
	printf("redir: ");
	print_tree_node(root->redirection);
	printf("\n");

	print_tree_2d(root->left, space);
}

/* ---------------------------------------------------------*/
/** int	execute(t_tree *syntax_tree)
  * {
  *     (void)syntax_tree;
  *
  *     return (1);
  * } */

void	shell_loop(void)
{
	int		status;
	char	*line;
	t_list	*tokens;
	t_tree	*syntax_tree;

	status = 1;
	while (status)
	{
		/* Readline library  */
		line = readline("& ");
		add_history(line);

		/* */

		/* (1) Lexer */
		tokens = tokenize(line);
		printf("Tokenize result ---------------------\n");
		print_tokens(tokens);
		printf("\n");
		printf("Parsing result (AST) ---------------------\n");

		/* (2) Parser */
		syntax_tree = parse_to_tree_recur(tokens);
		print_tree_2d(syntax_tree, 20);


		/* (3) Executer */
		/** status = execute(syntax_tree); */

		/** (4) free token list and line... etc */
		/** ft_lstclear(&tokens, delete_token); */
		// TODO : 
		// token_list를 편집해서 tree로 만들었기 때문에, 실행하는 executer에서 
		// 실행 후 free해주는 (자원 정리) 과정을 책임져야 한다.
		free(line);
	}
}

/* NOTE : https://brennan.io/2015/01/16/write-a-shell-in-c/ */

int main(int ac, char **av, char **env)
{	
	(void)ac;
	(void)av;

	/* (1) Load config files, Environ, set data etc... */
	(void)env;

	/* (2) Run command loop */
	shell_loop();

	/* (3) Perform any shutdown/cleanup  */

	return (EXIT_SUCCESS);
}
