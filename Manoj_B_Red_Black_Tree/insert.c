/*******************************************************************************************************************************************************************
*Title			: Insertion
*Description		: This function performs inserting the new data into the given Red Black tree.
*Prototype		: int insert(tree_t **root, data_t data);
*Input Parameters	: root – Pointer to the root node of the Red Black tree.
			: data – New data to be inserted into the Red Black tree.
*Output			: Status (SUCCESS / FAILURE)
*******************************************************************************************************************************************************************/
#include "rbt.h"


int insert(tree_t **root, data_t data)
{
	/* Definition goes here */
	tree_t *node=create_node(data);
	if(*root == NULL)
	{
		*root = node;
		(*root)->color='B'; //need to update the color cuz  nodes are created with red but root is black 
		return SUCCESS;
	}
	else
	{
	    
        /* there are elementss in the tree- do the normal onsert opwrationncuz its a bst only */
		int flag=0; //to know whether to move left or right
        tree_t *temp=(*root);
        tree_t *prev=NULL;
        while(temp!=NULL)
        {
            if(data<temp->data)
            {
                flag=0;
                prev=temp; //we need na the postion where we need to insert

                temp=temp->left; //move on the left side 
            }
            else if (data>temp->data)
            {
                flag=1;
                prev=temp;//we need na the postion where we need to insert
                temp=temp->right;
            }
            else 
            {
                return DUPLICATE; 
            }
        }
        if(flag==0)
        {
            prev->left=node;
            node->parent=prev; //must update the nodes parent as well 

        } 
		else                      
        {               
            prev->right=node;
            node->parent=prev;
        }

	}
	//call fix_insert
	fix_insert(root, &node);
    return SUCCESS;
}
