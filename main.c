#include"main.h"

/*
Name : NIKHIL
Date : 12/oct/23
Tilte : Inverted Search
*/

int main(int argc, char*argv[])
{	
	char ch;
	int option;
	S_list *head = NULL;  //creating a head pointer
	main_node *arr[27]={NULL}; //creating the hashtable name of arr
	char key[10];
	char backup[20];
	char filename[10];
	if(validate(argc,argv,&head) == SUCCESS)
	{
		printf("\nRead and validation of file is ---------> SUCCESSFULL\n");
do{

	//functions and operations in the project
	printf("1. Create Database\n2. Display Database\n3. Search Database\n4. Save Database\n5. Update Database\n6. Exit\n");
	printf("Enter the option: "); //user will enter the choice
		scanf("%d", &option);
		switch(option) 
		{
			case 1:
				{
					if(create_data_base(&head, arr) == SUCCESS) //function call will happens in create_node
					{
						printf("\ncreating node is ---------> SUCCESSFULL\n");
					}
					else
						printf("\ncreating node is ------------>UNSUCCESSFULL\n");
					break;
				}
			case 2:
				{
					display_data(arr);
					break;
				}
			case 3:
				{
					printf("Enter the word to be searched : ");
					scanf("%s",key);
					if(search(key,arr))
					{
						printf("\nsuccessfully key found\n");
					}
					else
						printf("\nunsuccessfull key found\n");

					break;
				}

			case 4: 
				{
					printf("Enter the backup file to which data needs to be saved : ");
					scanf("%s",backup);
					save(arr,backup);
					break;
				}
			case 5: 
				{
					printf("Enter the file name\n");
					scanf("%s", filename);
					if(update(arr,filename))
					{
					printf("\nSuccessfull the data is updated\n");
					}
					break;
				}			
			case 6:
				{
					return SUCCESS;
					break;
				}
			default:
				{
					printf("\nInvalid Option\n");
					break;
				}
		}
		getchar();
                printf("\nDo you want to Continue ?\nEnter y/Y to continue and n/N to diconnect\n");
                ch = getchar();
            }while (ch == 'y' || ch == 'Y');
	}
	else
	{
		printf("\nRead and validation of file is ----------> Unsuccessfull\n");
do{

	//functions and operations in the project
	printf("1. Display Database\n2. Search Database\n3. Save Database\n4. Update Database\n5. Exit\n");
	printf("Enter the option: "); //user will enter the choice
		scanf("%d", &option);
		switch(option) 
		{
			case 1:
				{
					display_data(arr);
					break;
				}
			case 2:
				{
					printf("Enter the word to be searched : ");
					scanf("%s",key);
					if(search(key,arr))
					{
						printf("\nsuccessfully key found\n");
					}
					else
						printf("\nunsuccessfull key found\n");

					break;
				}

			case 3: 
				{
					printf("Enter the backup file to which data needs to be saved : ");
					scanf("%s",backup);
					save(arr,backup);
					break;
				}
			case 4: 
				{
					printf("Enter the file name\n");
					scanf("%s", filename);
					if(update(arr,filename))
					{
					printf("\nSuccessfull the data is updated\n");
					}
					break;
				}			
			case 5:
				{
					return SUCCESS;
					break;
				}
			default:
				{
					printf("\nInvalid Option\n");
					break;
				}
		}
		getchar();
                printf("Do you want to Continue ?\nEnter y/Y to continue and n/N to diconnect\n");
                ch = getchar();
            }while (ch == 'y' || ch == 'Y');
	}
	return 0;
}
