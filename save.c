/*
Name:NIkhil
Date:12/oct/23
Description: saving the database in txt file
Input:
Output:
 */

#include"main.h"

int save(main_node **arr ,char * str)
{
	int i = 0;

	if( strstr(str,".txt") == NULL )  // check if .txt file
	{
		printf("ERROR : Not a .txt file\n");
		return 0;
	}

	FILE * backup; //create a file pointer

	if( (backup = fopen(str,"w")) == NULL )		// if file not present
	{
		printf("ERROR : Not able to open file in write mode\n");
		return 0;
	}
 
	for(i = 0; i <= 26; i++)  
	{
		main_node * main = arr[i]; //create a pointer in main_node

		while( main != NULL ) //check the condition and run the loop
		{
			fprintf(backup ,"#%d;", i);// fprint index
			fprintf(backup, "%s;%d;",main->str,main->file_count);	// fprint word and file count
			sub_node * sub = main -> subnode;  // create a temp subnode

			while( sub != NULL ) //check the condition and run the loop
			{
				fprintf(backup,"%s;%d;",sub->file_name,sub->word_count); // fprint filename and word count from sub node
				sub = sub->subnode; //travers the subnode
			}

			main = main->mainnode; //travers the mainniode
			fprintf(backup,"#\n");
		}
	}

	printf("Save operation completed successfully\n");
	fclose(backup); //close the file
}
