#ifndef AVL_HEADER
#define AVL_HEADER

typedef struct Avl_node
{
	int id;
	int index;
	int level;
	struct Avl_node *left;
	struct Avl_node *right;
} Avl_node;

void visit(Avl_node *n);
void destruct(Avl_node *n);
int get_weight(Avl_node *n);
void update_level(Avl_node *n);
Avl_node *rotate_left(Avl_node *, int);
Avl_node *rotate_right(Avl_node *, int);
void balance(Avl_node **n);
void insert_node(Avl_node **n, Avl_node *m);
void insert(Avl_node **n, int id, int index);
void erase(Avl_node **n, int id);
Avl_node *search(Avl_node **n, int id);

#endif
