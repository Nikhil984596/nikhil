#include"main.h"

/*
Name : NIKHIL
Date : 12/oct/23
Title : creating the database
 */

int create_data_base(S_list **head,main_node  **arr)
{
	S_list *temp = *head; //in SLL creating the temp pointer and assign addresss of head
	char str[10] = {0}; //create a character array which can hold 10 char 
	FILE *fptr; //create a file pointer 
	int index,i;
	int countw=0, countm=0;
	main_node *prevmain = NULL;
	sub_node *prevsub = NULL;
	while(temp) //run the loop until it reachers NULL
	{
		fptr = fopen(temp -> data, "r"); //opening the file in read mode
		while(fscanf(fptr, "%s", str) != EOF) //fscanf will scan the one word until it reachers end of file 
		{

			if (islower(str[0]) || isupper(str[0]))
				index = tolower(str[0]) % 97;
			else
				index = 26;
			if(arr[index] == NULL) //check in the hashtable of index of link is pointing to the NULL
			{
				arr[index] = create_main_node(str, temp->data); //function returning the mainnode address		
			}

			else
			{
				main_node *tempm =arr[index]; //creating the tempory pointer in main node  
				while(tempm) //traversing through the mainnode 
				{
					if(strcmp(tempm -> str,str) == 0) //comparing the data present in the mainnode str
					{

						countm = 1; //making the varibale as 1
						sub_node *temps = tempm -> subnode; //creating the temp pointer in subnode
						while(temps) //traversing the subnode
						{
							if(strcmp(temps -> file_name, temp -> data) ==0) //checking the filename is same are not
							{
								(temps -> word_count)++; //increment the wordcount
								countw=1; //assign this variable as 1
								break;
							}
							else
							{
								countw = 0;
								prevsub = temps;
								temps= temps-> subnode; //travers
							}
						} 

						if(countw == 0) //check the conditon
						{
							prevsub -> subnode = create_sub_node(temp -> data); //create a new sub_node and create a link between old and new sub_node
							(tempm->file_count)++; //increment the filecount in main_node
						}
						break; //exit from while loop 
					}

					else
					{
						countm = 0;
						prevmain = tempm;
						tempm = tempm -> mainnode; //travers through the main_node
					}
				}

				if(countm == 0) //check the condition 
				{ 
					main_node *new = create_main_node(str,temp -> data); //create the new main_node 
					prevmain -> mainnode = new; 
				}
			}
		}
		temp = temp -> link; //traversing in the SLL
	}
	return SUCCESS;
}
