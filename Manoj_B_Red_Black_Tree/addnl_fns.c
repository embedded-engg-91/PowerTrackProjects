#include "rbt.h"

tree_t *create_node(int data)
{
    /*adding only data for every new node the color will be considered as red only*/
    tree_t *new_node = malloc(sizeof(tree_t));
    new_node->color = 'R';
    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = NULL;

    return new_node;
}

void print_tree(tree_t *root)
{
    if (root == NULL)
    {
        return;
    }
    /*followed preorder printing logic*/
    /* print parent, go left, go right*/
    printf("%d [%c]", root->data, root->color);
    printf("\n");
    print_tree(root->left);
    print_tree(root->right);
}

void rotate_left(tree_t **root, tree_t **x) // the node passed is grandparent of inserted node
{                                           // but we are collecting that as x

    tree_t *y = (*x)->right;
    tree_t *t2 = y->left;
    tree_t *old_parent = (*x)->parent;

    y->left = (*x);
    (*x)->parent = y;
    y->parent = old_parent;
    (*x)->right = t2;
    if (t2 != NULL)
    {
        t2->parent = (*x);
    }
    if (old_parent == NULL)
    {
        (*root) = y;
    }
    else if ((*x) == old_parent->left) /*need to determine whether x was the left or right child of its old parent.*/
    {
        old_parent->left = y;
    }
    else
    {
        old_parent->right = y;
    }
}

void rotate_right(tree_t **root, tree_t **x) // the node passed is grandparent of inserted node
{                                            // but we are collecting that as x

    tree_t *y = (*x)->left;
    tree_t *t2 = y->right;
    tree_t *old_parent = (*x)->parent;

    y->right = (*x);
    (*x)->parent = y;
    y->parent = old_parent;
    (*x)->left = t2;
    if (t2 != NULL)
    {
        t2->parent = (*x);
    }
    if (old_parent == NULL)
    {
        (*root) = y;
    }
    else if ((*x) == old_parent->left) /*need to determine whether x was the left or right child of its old parent.*/
    {
        old_parent->left = y;
    }
    else
    {
        old_parent->right = y;
    }
}

void swap_colors(tree_t **a, tree_t **b)
{
    char temp = (*a)->color;
    (*a)->color = (*b)->color;
    (*b)->color = temp;
}

Status validate_rbt(tree_t *root)
{
    /*
    Every node is either red or black.
    The root is black.
    Every NULL leaf is considered black.
    A red node cannot have a red child.
    Every path from a node to its descendant NULL leaves has the same black height.
    */

    if (root == NULL)
        return SUCCESS;

    /* R1: Only the actual root must be black */
    if (root->parent == NULL && root->color != 'B')
    {
        printf("Violation: Root is not BLACK\n");
        return FAILURE;
    }


    /* R2: No RED node can have a RED child */
    if (root->color == 'R')
    {
        if (root->left != NULL && root->left->color == 'R')
        {
            printf("Violation: RED node has RED left child\n");
            return FAILURE;
        }

        if (root->right != NULL && root->right->color == 'R')
        {
            printf("Violation: RED node has RED right child\n");
            return FAILURE;
        }
    }

    /* check the number og black nodes*/
    /* Check entire tree */
    if (validate_rbt(root->left) == FAILURE)
        return FAILURE;

    if (validate_rbt(root->right) == FAILURE)
        return FAILURE;

    /* Black height */
    if (black_height(root) == -1)
        return FAILURE;


    return SUCCESS;
}

int black_height(tree_t *root)
{
    if (root == NULL)
        return 1; // leaf node is null is being considered as black

    int left_height = black_height(root->left); // check for evey node on either side of the node
    int right_height = black_height(root->right);

    if (left_height == -1 || right_height == -1) //if subtree is alredy invalid need to pass upawards
    return -1;
    
    if (left_height != right_height)
    {
        return -1;
    }

    if (root->color == 'B')
    {
        return left_height + 1;
    }
    //else
    return left_height;
    
}