#include"main.h"

/*
Name: Nikhil
Date: 12/oct/23
Description: Creating a main node
*/

main_node *create_main_node(char * str, char * filename)
{
		//creating the main_node 
		main_node * new = malloc(sizeof(main_node));
		new->mainnode = NULL; //assigning the next mainnode link as NULL
		//new->subnode = NULL; //assigning the subnode link as NULL
		new->file_count= 1; //initializing filecount as 0 
		strcpy(new->str, str); //copying the contenet of file single word in srt  


		new->subnode = create_sub_node(filename); //calling the function of creating a sub node and passing the filename 

		return new; //returing the new main_node address
}

