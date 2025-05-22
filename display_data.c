/*
Name:Nikhil
Date: 12/oct/23
Description: displaying tha database
*/

#include "main.h"

void display_data(main_node **arr)
{
	int i =0;
	printf("Displaying Database : \n");
	printf("-------------------------------------------------------------------------------------------------------------\n");
	printf("[index]\t\t[Word]\t\tFile_count\t   Files\t\t\tword_count\n");

	for(i= 0; i<= 26;i++) //run the loop
	{
		if(arr[i])
		{
			main_node *temp = arr[i];
			while(temp)
			{ 
				printf("[%d]\t\t%s\t\t%d\t\t", i, temp->str, temp->file_count); //printing the word and filecount

				sub_node *temp1 = temp -> subnode;
				while(temp1)
				{
			 		printf(":file(s)  [%s]\t\t%d\t", (temp1->file_name), (temp1-> word_count));  //printing the filename and wordcount
					temp1 = temp1 -> subnode; //travers through the subnode
				}
				temp = temp -> mainnode; //travers through the mainnode
				printf("\n"); 
			}
		}
	}
	printf("------------------------------------------------------------------------------------------------------------\n");
}



