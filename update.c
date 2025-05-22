#include <stdio.h>
/*
Name: Nikhil
Date: 12/oct/23
Description : update the database from the backup file
Input:
Output:
*/

#include "main.h"
int update(main_node **arr, char *fileName)
{
    FILE *fptr = fopen(fileName, "r"); //open the file in read mode
    if (fptr == NULL)
    {
        printf("ERROR opening file\n");
        return FAILURE;
    }

    fseek(fptr, 0, SEEK_END); //move the cursor to the end 
    int size = ftell(fptr); 
    rewind(fptr); //now the cursor is at the end, rewind function will move the cursor at the starting

    if(size == 0)  //check the file size
    {
        printf("File %s is empty\n", fileName);
        return FAILURE;
    }

    char str[100]; //initialzing the character arr
    int index;

    while(fscanf(fptr, "%s", str) > 0) //scanning the content of fptr in file and copying to the str
    {
        if(str[0] == '#')
        {
            index = atoi(strtok(&str[1], ";"));
            main_node *newmainnode = malloc(sizeof(main_node)); //creating a new main node
            if (newmainnode == NULL) //if the mainnode is not creating print the error mess
            {
                printf("ERROR : Mainnode creation is failed\n");
                return FAILURE;
            }
            
            char *word = strtok(NULL, ";"); //scanning the word till ";" get
            strcpy(newmainnode->str, word); //copying the char word to the mainnode of str
            int filecount = (atoi(strtok(NULL, ";")));
            newmainnode->file_count = filecount;  //assign the filecount
            newmainnode->subnode = NULL; 
            newmainnode->mainnode = NULL;
            for(int i = 0; i < newmainnode->file_count; i++) //run the for loop filecount times
            {
                sub_node *newsubnode = malloc(sizeof(sub_node));
                strcpy(newsubnode->file_name, strtok(NULL, ";")); //copy the filename using strtokj function till ';' it will scan
                newsubnode->word_count = atoi(strtok(NULL,";"));
                if(newmainnode->subnode == NULL)
                {
                    newmainnode->subnode = newsubnode;
                }
                else
                {
                    sub_node *tempsubnode = newmainnode->subnode; //creating the subnode
                    while(tempsubnode->subnode != NULL) //run till the subnode address reach the null
                    {
                        tempsubnode = tempsubnode->subnode;
                    }
                    tempsubnode->subnode = newsubnode; //update the link part 
                }
            }
            if(arr[index] == NULL)
            {
                arr[index] = newmainnode; //check the arr of link is null, if null assign  the mainnode address
            }
            else
            {
                main_node *nextmainnode = arr[index]; //else create a another mainnode 
                while(nextmainnode->mainnode != NULL)
                {
                    nextmainnode = nextmainnode->mainnode; //travers the main node
                }
                nextmainnode->mainnode = newmainnode;  //update the link part
            }
        }
    }
    fclose(fptr); //close the file pointer
    return SUCCESS;
}
