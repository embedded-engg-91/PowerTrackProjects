
#include "rbt.h"

int main()
{
	
	tree_t *root = NULL;
	data_t data;
	data_t minimum;
	data_t maximum;
	int operation;
	do
	{
		
		printf("1. Insert\n2. Display\n3. Find Minimum\n4. Find Maximum\n5. Delete Minimum\n6. Delete Maximum\n7. Delete Element\n8. Exit\n");
		
		if (scanf("%d", &operation) != 1)
		{
			printf("Please enter a valid operations as the input!\n");
			continue;
		}

		
		switch (operation)
		{
		case 1: 
			printf("Enter the data to be inserted into the RB Tree: \n");
			scanf("%d", &data);
			insert(&root, data);
			
			printf("Data Insertion Successful!!!\n");
			break;
		
			case 2: 
			print_tree(root);
			break;
		case 3:

			printf("Minimum data in the tree: %d\n", findmin(root)); 
			print_tree(root);

			break;
		case 4:
			printf("Maximum data in the tree: %d\n", findmax(root)); 
			print_tree(root);
			break;
		
			case 5: 
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

		case 6: 
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
		case 7: 

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
