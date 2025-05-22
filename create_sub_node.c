/*
Name: Nikhil
Date: 12/oct/23
Description: Creating a sub node
*/

#include"main.h"

sub_node * create_sub_node(char * filename) 
{
		sub_node * s = malloc(sizeof(sub_node)); //creating the new sub_node 

		s->subnode = NULL; //asigning the s of subnode link as NULL

		s->word_count = 1; //initailzing the word count as 1

		strcpy(s->file_name, filename); //copying the filename to thr subnode of char filename

		return s; //return the node address

}

