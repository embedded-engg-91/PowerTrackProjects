#include "text_editor.h"

void print_list(DLL *head)
{
	
	if (head == NULL)
	{
		printf("INFO : Editor is empty\n");
	}
	else
	{
		int line_no = 1;
		
		while (head)
		{
			
			printf("%d. ", line_no++);

			printf("%s", head->str);
			printf("\n");
			
			head = head->next;
		}
	}
	printf("\n");
}
