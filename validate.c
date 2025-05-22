#include "main.h"

/*
Name : Nikhil
Date : 12/oct/23
Title:validating the file arguments
*/

int validate(int argc, char *argv[], S_list **head)
{
	FILE *ptr; //create a file pointer
	
        //check the user as pass the CLA more then 1
	if(argc > 1)
	{
		for(int i = 1; i < argc; i++) //run the loop upto argc - 1
		{
			if(strstr(argv[i] , ".txt") == NULL)  //check the file as .txt file
			{
					printf("ERROR : %s is not a .txt file\n",argv[i]);
					continue;
			}

			ptr = fopen (argv[i], "r"); //open the file in read mode 
			if(ptr == NULL) 
			{
				printf("ERROR : %s file is not present in the current directory\n",argv[i]);
				continue;
			}

			fseek(ptr,0,SEEK_END); //check the file is content any data are not  with the help od fseek function
			if(ftell(ptr) != 0) //ftell will tell in which index the cursor is present
			{
				if(*head == NULL) //check the head is pointing to NULL
				{
					S_list *new = malloc(sizeof(S_list));  //creating the SLL node
					strcpy(new->data,argv[i]); //update the data part as filename  and link as NULL
					new -> link = NULL;
					*head = new; //point the n ew node address to the head
				}			
			//if the head is not null 
			else
			{
				//create a two pointer 
				S_list *temp = *head;					
				S_list *prev = *head;

				while(temp) //run the loop until it reachers NULL
				{

					if( strcmp(temp->data,argv[i]) == 0) //compare the arguments with single linked data 
					{
						printf("ERROR : %s is repeated\n",argv[i]);
						break;
					}

					else
					{
						prev = temp;
						temp = temp->link; //travers through the next node
					}
				}
				S_list *new = malloc(sizeof(S_list));	 //creating the SLL node and updata the dilename for data ,and link part as NULL
				strcpy(new->data,argv[i]);
				new->link = NULL;
				prev->link = new;								
			}
			}
			else
			{
				printf("Error : %s File is a empty\n", argv[i]);
			}
			fclose(ptr); //close the file
		}
	}

	else
	{
		printf("!!!ERROR please pass CLA more then 1\n");
	       	printf("only you can do the update database\n");
	       return 0;
	}

	//printing the file name 
        S_list *print = *head;
        printf("THE SLL saves files are : ");
	while(print)
	{
		printf("%s ", print->data);
		print = print -> link;
	}
        printf("\n");
	return SUCCESS;
}	









