#include "main.h"

/*
Name:Nikhil 
Date:12/oct/23
Description: Search the data in the database
*/

int search(char * key, main_node **arr )
{
		int i = 0;

		int pos = 0;

		pos = tolower(key[0]) % 97;
		if(pos > 25)
		{
			pos =26;
		}

		if( arr[pos] == NULL )	// if no main node then no word from that alphabet found
		{
				printf("key not found\n");
				return 0;
		}

		main_node * m = arr[pos];  // take temporary to traverse thro main nodes

		while( m != NULL )
		{
				if( strcmp(key,m->str) == 0 )	// if word found in main node's word part
				{
						if( m->subnode->subnode == NULL ) // if word in only 1 file
						{
							printf("word %s found in 1 file\n",key);
							printf("In %s %d time(s)\n",m->subnode->file_name,m->subnode->word_count);
								return 1;
						}

						sub_node * s = m->subnode; // if word in multiple files

						printf("word %s found in %d file(s)\n",key,m->file_count);

						while( s != NULL )	// traverse till last sub node
						{
								printf("%s found %d time(s) in %s\n",key,s->word_count,s->file_name); 
								s = s->subnode; //travers the subnode
						}
						return 1;
				}
				m = m->mainnode; //traveres the mainnode
		}
		printf("%s not found\n",key); //print if the key is not presenr
		return 0;
}

