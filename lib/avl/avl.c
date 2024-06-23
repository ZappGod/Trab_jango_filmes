//////////////////////////////////////////////////

// curitiba 2024
// universidade positivo
// escola de tecnologia da informação
// prof jean diogo

//////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

#include "avl.h"

void visit(Avl_node *n)
{
	if (n)
	{
		printf("[");
		visit(n->left);
		printf("%d - %d", n->id, n->index);
		visit(n->right);
		printf("]");
	}
}

void destruct(Avl_node *n)
{
	if (n)
	{
		destruct(n->left);
		destruct(n->right);
		free(n);
	}
}

//////////////////////////////////////////////////

int get_weight(Avl_node *n)
{
	if (!n)
	{
		return 0;
	}
	int const left_level = n->left ? n->left->level : -1;
	int const right_level = n->right ? n->right->level : -1;
	return right_level - left_level;
}

void update_level(Avl_node *n)
{
	if (n)
	{
		int const left_level = n->left ? n->left->level : -1;
		int const right_level = n->right ? n->right->level : -1;
		int const max_level = left_level > right_level ? left_level : right_level;
		n->level = max_level + 1;
	}
}

Avl_node *rotate_left(Avl_node *n, int weight)
{
	if (!n)
	{
		return NULL;
	}

	int const right_weight = get_weight(n->right);

	if ((weight < 0 && right_weight > 0) || (weight > 0 && right_weight < 0))
	{
		n->right = rotate_right(n->right, right_weight);
	}

	Avl_node *temp = n->right;
	n->right = temp->left;
	temp->left = n;

	update_level(temp->left);
	update_level(temp);

	return temp;
}

Avl_node *rotate_right(Avl_node *n, int weight)
{
	if (!n)
	{
		return NULL;
	}

	int const left_weight = get_weight(n->left);

	if ((weight < 0 && left_weight > 0) || (weight > 0 && left_weight < 0))
	{
		n->left = rotate_left(n->left, left_weight);
	}

	Avl_node *temp = n->left;
	n->left = temp->right;
	temp->right = n;

	update_level(temp->right);
	update_level(temp);

	return temp;
}

void balance(Avl_node **n)
{
	if (*n)
	{
		int const weight = get_weight(*n);
		if (weight > 1)
		{
			*n = rotate_left(*n, weight);
		}
		else if (weight < -1)
		{
			*n = rotate_right(*n, weight);
		}
		else
		{
			update_level(*n);
		}
	}
}

//////////////////////////////////////////////////

void insert_node(Avl_node **n, Avl_node *m)
{
	if (!m)
	{
		return;
	}

	if (!*n)
	{
		*n = m;
		return;
	}

	if (m->id < (*n)->id)
	{
		insert_node(&(*n)->left, m);
	}
	else
	{
		insert_node(&(*n)->right, m);
	}

	balance(n);
}

void insert(Avl_node **n, int id, int index)
{
	Avl_node *m = (Avl_node *)malloc(sizeof(Avl_node));
	if (m)
	{
		m->id = id;
		m->index = index;
		m->level = 0;
		m->left = NULL;
		m->right = NULL;
	}
	insert_node(n, m);
}

void erase(Avl_node **n, int id)
{
	if (!(*n))
	{
		return;
	}

	if (id < (*n)->id)
	{
		erase(&(*n)->left, id);
	}
	else if (id > (*n)->id)
	{
		erase(&(*n)->right, id);
	}
	else
	{
		insert_node(&((*n)->left), (*n)->right);
		Avl_node *temp = (*n)->left;
		free(*n);
		(*n) = temp;
	}

	balance(n);
}

Avl_node *search(Avl_node **n, int id)
{
	if (!(*n))
	{
		return;
	}

	if (id < (*n)->id)
	{
		search(&(*n)->left, id);
	}
	else if (id > (*n)->id)
	{
		search(&(*n)->right, id);
	}
	else
	{
		return *n;
	}
}