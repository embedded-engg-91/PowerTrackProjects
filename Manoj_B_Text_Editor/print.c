#include "text_editor.h"

void print_list(DLL *head)
{
	/* Cheking the list is empty or not */
	if (head == NULL)
	{
		printf("INFO : Editor is empty\n");
	}
	else
	{
		int line_no = 1;
		// printf("%d. ",line_no);
		while (head)
		{
			/* Printing the list */
			printf("%d. ", line_no++);

			printf("%s", head->str);
			printf("\n");
			/* Travering in forward direction */
			head = head->next;
		}
	}
	printf("\n");
}