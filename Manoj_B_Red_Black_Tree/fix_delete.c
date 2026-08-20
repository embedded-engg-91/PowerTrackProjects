#include "rbt.h"

Status fix_delete(tree_t **root, tree_t *replacement, tree_t *parent, int was_left) /*pass by value is enought to change the nodes content */
{
    if (replacement != NULL && replacement->color == 'R')
    {
        replacement->color = 'B'; /*make it black */
        return SUCCESS;
    }
    while (parent != NULL)
    {

        /*find sibling using parent, cnat use replacement cuz its already null na  */
        tree_t *sibling;

        if (was_left)
        {
            sibling = parent->right;
        }
        else
        {
            sibling = parent->left;
        }
        if (sibling == NULL)
        {
            replacement = parent;
            parent = replacement->parent;

            if (parent == NULL)
                return SUCCESS;

            if (parent->left == replacement)
                was_left = 1;
            else
                was_left = 0;

            continue;
        }

        if (sibling->color == 'R')
        {
            /* Case: RED sibling */
            sibling->color = 'B';
            parent->color = 'R';
            /*then we ned to foate the parent toward the double b;ack side */
            /*so first we need to find it*/
            if (was_left)
            {
                rotate_left(root, &parent); /*cuz here we want to rotate the parent */
                /*refind the new sibling*/
                sibling = parent->right;
            }
            else
            {
                rotate_right(root, &parent);
                sibling = parent->left;
            }
        }

        char left_clr;
        char right_clr;
        if (sibling->left == NULL || sibling->left == NULL) // children color
            left_clr = 'B';        /*remember no nodes means the color will be considered as black only */
        else
            left_clr = sibling->left->color;

        if (sibling->right == NULL || sibling->right == NULL)
            right_clr = 'B';
        else
            right_clr = sibling->right->color; // children color

        /* Case: BLACK sibling */

        /*cases */
        // case 1:any one red child exists
        // case 2: both children are black

        if (left_clr == 'B' && right_clr == 'B') // sibling children colors
        {
            sibling->color = 'R';

            if (parent->color == 'R')
            {
                parent->color = 'B';
                return SUCCESS;
            }
            else
            {
                /* parent is BLACK */
                /* Double Black moves upward */
                replacement = parent;
                parent = replacement->parent;

                /* continue fixing */
                if (parent == NULL)
                {
                    replacement->color = 'B';
                    return SUCCESS;
                }

                if (parent->left == replacement)
                    was_left = 1;
                else
                    was_left = 0;

                continue;
            }
        }
        /*means at least on one side there is a red child */

        // need to do rotatoins and recolor
        if (was_left)
        {
            if (right_clr == 'R') /*right roght roattion case */
            {
                sibling->color = parent->color;
                parent->color = 'B';

                sibling->right->color = 'B';

                rotate_left(root, &parent);

                return SUCCESS;
            }
            else
            {
                /* RIGHT-LEFT case */
                sibling->left->color = 'B';

                rotate_right(root, &sibling);

                /* new sibling */
                sibling = parent->right;

                /* now it is RR */

                sibling->color = parent->color;
                parent->color = 'B';

                sibling->right->color = 'B';

                rotate_left(root, &parent);

                return SUCCESS;
            }
        }
        else
        {
            if (left_clr == 'R') /*LEFT-LEFT ROATATION CASE */
            {
                sibling->color = parent->color;
                parent->color = 'B';

                sibling->left->color = 'B';

                rotate_right(root, &parent);

                return SUCCESS;
            }
            else
            {
                /* LEFT-RIGHT case */
                sibling->right->color = 'B';

                rotate_left(root, &sibling);

                /* new sibling */
                sibling = parent->left;

                /* now it is RR */

                sibling->color = parent->color;
                parent->color = 'B';

                sibling->left->color = 'B';

                rotate_right(root, &parent);

                return SUCCESS;
            }
        }
    }
    return SUCCESS;
}
