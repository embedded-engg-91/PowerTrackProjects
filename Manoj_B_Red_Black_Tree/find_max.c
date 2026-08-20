#include "rbt.h"

int findmax(tree_t * root)
{

    if(root==NULL) return FAILURE;

    //go on traversing until you reach root->right ==NULL there our max element wil be present
    //cuz of how data is ORGANSIED in tree
    if(root->right == NULL)
    {
        return root->data;
    }
    return findmax(root->right);
}

