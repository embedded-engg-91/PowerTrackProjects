/**************************************************************************************************************************************************************
 *Title		: main function(Driver function)
 *Description	: This function is used as the driver function for the all the functions
 ***************************************************************************************************************************************************************/
#include "rbt.h"

int main()
{
	/* Declare the pointers */
	tree_t *root = NULL;
	data_t data;
	data_t minimum;
	data_t maximum;
	int operation;
	do
	{
		/* Display the menu */
		printf("1. Insert\n2. Display\n3. Find Minimum\n4. Find Maximum\n5. Delete Minimum\n6. Delete Maximum\n7. Delete Element\n8. Exit\n");
		/* Read the option for performing the operation */
		if (scanf("%d", &operation) != 1)
		{
			printf("Please enter a valid operations as the input!\n");
			continue;
		}

		/* Jump to the option entered by the user */
		switch (operation)
		{
		case 1: /* insert an element */
			printf("Enter the data to be inserted into the RB Tree: \n");
			scanf("%d", &data);
			insert(&root, data);
			
			printf("Data Insertion Successful!!!\n");
			break;
		
			case 2: /*display operation*/
			print_tree(root);
			break;
		case 3:

			printf("Minimum data in the tree: %d\n", findmin(root)); /*dont need to input data this is minimum in tree na */
			print_tree(root);

			break;
		case 4:
			printf("Maximum data in the tree: %d\n", findmax(root)); /*dont need to input data this is minimum in tree na */
			print_tree(root);
			break;
		
			case 5: /*5. Delete Minimum*/
		{
			data = findmin(root);

			Status result = delete_element(&root, data);

			if (result == SUCCESS)
			{
				if (validate_rbt(root) == SUCCESS)
					print_tree(root);
				else
					printf("RBT Validation Failed!!!\n");
			}
			break;
		}

		case 6: /*5. Delete maximum*/
		{
			data = findmax(root);

			Status result = delete_element(&root, data);

			if (result == SUCCESS)
			{
				if (validate_rbt(root) == SUCCESS)
					print_tree(root);
				else
					printf("RBT Validation Failed!!!\n");
			}
			break;
		}
		case 7: /* delete element*/

		{
			printf("Enter the data you want to delete: \n");

			scanf("%d", &data);

			Status result = delete_element(&root, data);
			if (result == SUCCESS)
			{
				if (validate_rbt(root) == SUCCESS)
				{
					printf("The element has been deleted successfully!!!\n");

					print_tree(root);
				}
				else
				{
					printf("RBT Validations Failed!!!\n");
					return -1;
				}
			}
			else if (result == NOELEMENT)
			{
				printf("The data you have entered does not exist!!!");
				printf("Please enter a valid element and try again.\n");
			}
			else
			{
				printf("Element Deletion Failed!!!\n");
				return -1;
			}

			break;
		}
		case 8:
			return 0;
			break;
		}

	} while (operation >= 1 && operation <= 8);

	return 0;
}
