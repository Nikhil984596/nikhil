#ifndef MAIN_H
#define MAIN_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define SUCCESS 1
#define FAILURE 0

typedef struct node
{
	char data[50];
	struct node *link;
}S_list;

typedef struct subnode
{
	int word_count;
	char file_name[20];
	struct subnode *subnode;
}sub_node;

typedef struct mainnode
{
	int file_count;
	char str[20];
	sub_node *subnode;
	struct mainnode *mainnode;
}main_node;



int validate(int argc, char *argv[], S_list ** head);

int create_data_base(S_list **head,main_node **arr);

main_node *create_main_node(char *str1, char *str2);

sub_node *create_sub_node(char *str);

void display_data(main_node **arr);

int search(char * key, main_node **arr);

int save(main_node **arr, char * str);

int update( main_node **arr,char *str);
#endif
