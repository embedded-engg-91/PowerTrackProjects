#include "rbt.h"


Status delete_element(tree_t **root, int data)
{
	if (root == NULL || *root == NULL)
    {
        return NOELEMENT;
    }

    
    tree_t *current = *root;

    
    while (current != NULL)
    {
        if (data < current->data)
        {
            current = current->left;
        }
        else if (data > current->data)
        {
            current = current->right;
        }
        else
        {
            break; 
        }
    }

    
    if (current == NULL)
    {
        return NOELEMENT;
    }
	
	tree_t *target=current;
	tree_t  *node_to_del, *replacement, *inorder_succ;
	

		
		
		
		char del_clr;
		tree_t *parent;
		int was_left = 0;
		if (target->left != NULL && target->right != NULL) 
		{
			inorder_succ = findmin_node(target->right);
			target->data = inorder_succ->data;
			node_to_del = inorder_succ;
		}
		else
		{
			node_to_del = target;
		}
		if (node_to_del->left != NULL && node_to_del->right == NULL) 
		{
			
			
			
			replacement = node_to_del->left;
		}
		else if (node_to_del->right != NULL && node_to_del->left == NULL) 
		{
			
			
			
			replacement = node_to_del->right;
		}
		else 
		{
			replacement = NULL;
		}
		del_clr = node_to_del->color; 
		parent = node_to_del->parent; 

		if (parent == NULL)
		{
			(*root) = replacement;
			if (replacement != NULL)
				replacement->parent = NULL;
		}
		else
		{
			
			if (parent->left == node_to_del)
			{
				was_left = 1;
				parent->left = replacement;
			}
			else
			{
				parent->right = replacement;
			}
			
			if (replacement != NULL)
				replacement->parent = parent;
		}
		free(node_to_del); 

		if (del_clr == 'B') 
		{					
			fix_delete(root, replacement, parent, was_left);
		}
	
	return SUCCESS;
}
