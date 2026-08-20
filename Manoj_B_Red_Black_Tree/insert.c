
#include "rbt.h"


int insert(tree_t **root, data_t data)
{
	
	tree_t *node=create_node(data);
	if(*root == NULL)
	{
		*root = node;
		(*root)->color='B'; 
		return SUCCESS;
	}
	else
	{
	    
        
		int flag=0; 
        tree_t *temp=(*root);
        tree_t *prev=NULL;
        while(temp!=NULL)
        {
            if(data<temp->data)
            {
                flag=0;
                prev=temp; 

                temp=temp->left; 
            }
            else if (data>temp->data)
            {
                flag=1;
                prev=temp;
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
            node->parent=prev; 

        } 
		else                      
        {               
            prev->right=node;
            node->parent=prev;
        }

	}
	
	fix_insert(root, &node);
    return SUCCESS;
}
