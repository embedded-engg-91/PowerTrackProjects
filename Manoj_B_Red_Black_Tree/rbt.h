
#ifndef RBT_H
#define RBT_H


#include<stdio.h>
#include<stdlib.h>
typedef int data_t;
typedef enum
{
	SUCCESS,
	FAILURE,DUPLICATE, NOELEMENT, EQUAL, UNEQUAL
}Status;

typedef struct node
{
	struct node *left;
	data_t data;
	struct node *right;
	struct node *parent;
	char color;
}tree_t;



int insert(tree_t **root, data_t data);
tree_t * create_node(int data);
void print_tree(tree_t *root);
Status fix_insert(tree_t **root, tree_t **new_node);
void swap_colors(tree_t **a, tree_t **b);
void rotate_left(tree_t **root, tree_t **x);
void rotate_right(tree_t **root, tree_t **x);
void get_gp_uncle(tree_t *node, tree_t **uncle, tree_t **grandparent);
int findmin(tree_t *root);
int findmax(tree_t * root);
Status fix_delete(tree_t **root, tree_t *replacement, tree_t *parent, int was_left); 
Status delete_element(tree_t **root, int data);
tree_t *findmin_node(tree_t *root);
Status validate_rbt(tree_t *root);
int black_height(tree_t *root);






#endif
