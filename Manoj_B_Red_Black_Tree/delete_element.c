#include "rbt.h"

/*iterative  v*/
Status delete_element(tree_t **root, int data)
{
	if (root == NULL || *root == NULL)
    {
        return NOELEMENT;
    }

    // 1. Use a temp variable for traversal
    tree_t *current = *root;

    // 2. Iteratively search for the data
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
            break; // Data found
        }
    }

    // If current is NULL, the data wasn't in the tree
    if (current == NULL)
    {
        return NOELEMENT;
    }
	
	tree_t *target=current;
	tree_t  *node_to_del, *replacement, *inorder_succ;
	

		// if it has come till here meand data has been found
		// remeber if its not greate if its not less than than its equal
		/* cases 1. leaf node 2. node with one child 3. node with 2 child*/
		char del_clr;
		tree_t *parent;
		int was_left = 0;
		if (target->left != NULL && target->right != NULL) // case 1 leaf node
		{
			inorder_succ = findmin_node(target->right);
			target->data = inorder_succ->data;
			node_to_del = inorder_succ;
		}
		else
		{
			node_to_del = target;
		}
		if (node_to_del->left != NULL && node_to_del->right == NULL) // single child node
		{
			// tree_t *bkp=(*root)->left;
			// free((*root));
			// return bkp;
			replacement = node_to_del->left;
		}
		else if (node_to_del->right != NULL && node_to_del->left == NULL) // single child nodde
		{
			// tree_t *bkp=(*root)->right;
			// free((*root));
			// return bkp;
			replacement = node_to_del->right;
		}
		else /* target has 0/no child*/
		{
			replacement = NULL;
		}
		del_clr = node_to_del->color; /* save deleted color*/
		parent = node_to_del->parent; /* save the parent */

		if (parent == NULL)
		{
			(*root) = replacement;
			if (replacement != NULL)
				replacement->parent = NULL;
		}
		else
		{
			/*connect parent to replacement */
			if (parent->left == node_to_del)
			{
				was_left = 1;
				parent->left = replacement;
			}
			else
			{
				parent->right = replacement;
			}
			/* connect replacement to parent */
			if (replacement != NULL)
				replacement->parent = parent;
		}
		free(node_to_del); /* free the node to del*/

		if (del_clr == 'B') /* deleted color is black only we need to fix it*/
		{					/* cus delenting a black node will afftect the the no og black nodes in each path */
			fix_delete(root, replacement, parent, was_left);
		}
	
	return SUCCESS;
}
