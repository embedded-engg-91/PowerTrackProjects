#include "rbt.h"

Status fix_insert(tree_t **root, tree_t **node)
{

    /*find the parent, grandparent, uncle of the node*/
    if (*node == (*root))
        return SUCCESS;
    tree_t *parent = (*node)->parent;
    tree_t *uncle;
    tree_t *grandparent;
    if (parent->color == 'B')
        return SUCCESS;

    get_gp_uncle(*node, &uncle, &grandparent);
    if (uncle != NULL && uncle->color == 'R')
    {
        // recolor
        parent->color = 'B';
        uncle->color = 'B';
        if (grandparent != (*root))
        {
            grandparent->color = 'R';
        }
        /*otherwise no need to update */
        fix_insert(root, &grandparent); // needs to continue cuz the granparent might itself cause violation
    }
    else
    {
        if (grandparent->right == parent && parent->right == (*node)) /* right right rotate case */
        {
            rotate_left(root, &grandparent); // we need to send the granparent of the cuurent node which is grandparent already
            swap_colors(&grandparent, &parent);
        }

        else if (grandparent->left == parent && parent->left == (*node)) /* left left rotate case */
        {
            rotate_right(root, &grandparent); // we need to send the granparent of the cuurent node which is grandparent already
            swap_colors(&grandparent, &parent);
        }

        else if (grandparent->left == parent && parent->right == (*node)) /* left right rotate case  */
        {
            /*here we wold need two cases*/
            rotate_left(root, &parent); // we need to send the granparent of the cuurent node which is grandparent already
            // swap_colors(&grandparent, &parent); at this point the colore should not be swaapped
            rotate_right(root, &grandparent); // we need to send the granparent of the cuurent node which is grandparent already
            swap_colors(&grandparent, node);
        }
        else /* right left case*/
        {
            rotate_right(root, &parent);
            rotate_left(root, &grandparent);
            swap_colors(&grandparent, node);
        }
    }
    return SUCCESS;
}