#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

Status do_encoding(EncodeInfo * encInfo)
{
    if(copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success) //function call to copy bmp header data
		printf("Copying BMP header ----------------------=-->successful\n");
    else
		printf("Copying BMP header ----------------------->unsuccessful\n");
    
	if(encode_magic_string(MAGIC_STRING, encInfo) == e_success)
		printf("Encoding MAGIC STRING ---------------------->successful\n");
    else
		printf("Encoding MAGIC STRING --------------------->unsuccessful\n");
    
	if(encode_secret_file_extn("txt", encInfo) == e_success)
		printf("Encoding of secret file extension ------------>successful\n");
    else
		printf("Encoding of secret file extension ---------->unsuccessful\n");
    
	fseek(encInfo -> fptr_secret, 0, SEEK_END);
    encInfo -> size_secret_file = ftell(encInfo -> fptr_secret);
    if(encode_secret_file_size(encInfo -> size_secret_file, encInfo) == e_success)
		printf("Encoding of secret file size ----------------->successful\n");
    else
		printf("Encoding of secret file size --------------->unsuccessful\n");
    
	if(encode_secret_file_data(encInfo) == e_success)
		printf("Encoding of secret file data ----------------->successful\n");
    else
		printf("Encoding of secret file data ---------------->unsuccessful\n");
    
	if(copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
		printf("Copying remaining data to destination file ------------>successful\n");
    else
		printf("Copying remaining data to destination file ------------>unsuccessful\n");
	
	return e_success;
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
	encInfo -> src_image_fname = argv[2];
	encInfo -> secret_fname = argv[3];
	if(argv[4] == NULL)
	{
		encInfo -> stego_image_fname = "output.bmp";
	}
	else
	{
		encInfo -> stego_image_fname = argv[4];
	}

    if(open_files(encInfo) == e_success)
		printf("File opening --------------------------------->successful\n");
    else
		printf("File opening -------------------------------->unsuccessful\n");

	//check file is bmp or not!!
	char data[2];
	fread(data, 1, 2, encInfo -> fptr_src_image);
	if(data[0] == 0X42 && data[1] == 0X4d)
		printf("BMP file validation -------------------------->successful\n");
	else
		printf("BMP file validation ------------------------->unsuccessful\n");
	//check capacity of beautiful.bmp is enough or not

	fseek(encInfo -> fptr_src_image, 0, SEEK_END);
	int size = ftell(encInfo -> fptr_src_image);
	rewind(encInfo -> fptr_src_image);

	int s1, s2, s3, total;
	s1 = 32 + strlen(MAGIC_STRING) * 8;

	char *cptr = encInfo -> secret_fname;
	char *secret_extn = strtok(cptr, ".");
	secret_extn = strtok(NULL, ".");
	s2 = 32 + strlen(secret_extn) * 8;

	fseek(encInfo -> fptr_secret, 0, SEEK_END);
	s3 = 32 + ftell(encInfo -> fptr_secret) * 8;
	rewind(encInfo -> fptr_secret);
	total = 54 + s1 + s2 + s3;
	if(size > total)
	{
		printf("Capacity check ------------------------------>successful\n");
	}
	else
		printf("Capacity check ---------------------------->unsuccessful\n");
	return e_success;
}
		
Status open_files(EncodeInfo * encInfo)
{
    encInfo -> fptr_src_image = fopen(encInfo -> src_image_fname , "r");
    if(encInfo -> fptr_src_image == NULL)
        return e_failure;
    encInfo -> fptr_secret = fopen(encInfo -> secret_fname, "r");
    if(encInfo -> fptr_secret == NULL)
        return e_failure;
    encInfo -> fptr_stego_image = fopen(encInfo -> stego_image_fname, "w");
    if(encInfo -> fptr_stego_image == NULL)
        return e_failure;
    else
        return e_success;
}
Status copy_bmp_header(FILE * p1, FILE *p2)
{
    char buffer[54];
    fread(buffer, 1, 54, p1); 
    fwrite(buffer, 1, 54,p2); 
    return e_success;
}

Status encode_magic_string(const char *str, EncodeInfo * encInfo)
{
    char buffer[32];
    int length = strlen(str);
    fread(buffer, 1, 32, encInfo -> fptr_src_image);
    for(int i = 31; i >= 0; i--)
    {
        buffer[i] = buffer[i] & (~1);
        if(length & (1 << i))
        {
            buffer[31-i] = (buffer[31-i] | 1);
        }
    }
    fwrite(buffer, 1, 32, encInfo -> fptr_stego_image);

    char str_char;
    for(int i = 0; i<length; i++)
    {
        for(int j = 7; j >= 0; j--)
        {
            fread(&str_char, 1, 1, encInfo -> fptr_src_image);
            str_char = str_char & (~1);
            if(str[i] & (1 << j))
            {
                str_char = str_char | 1;
            }
            fwrite(&str_char, 1, 1, encInfo -> fptr_stego_image);
        }
    }
    return e_success;
}

Status encode_secret_file_extn(const char *str , EncodeInfo * encInfo)
{
    int length = strlen(str);
    char buffer[32];
    fread(buffer, 1, 32, encInfo -> fptr_src_image);
    for(int i = 31; i >= 0; i--)
    {
        buffer[i] = buffer[i] & (~1);
        if(length & (1<<i))
        {
            buffer[31-i] = (buffer[31-i] | 1);
        }
    }
    fwrite(buffer, 1, 32, encInfo -> fptr_stego_image);

    char str_char;
    for(int i = 0; i < length; i++)
    {
        for(int j = 7; j >= 0; j--)
        {
            fread(&str_char, 1, 1, encInfo -> fptr_src_image);
            str_char = str_char & (~1);
            if(str[i] & (1<<j))
            {
                str_char = str_char | 1;
            }
            fwrite(&str_char, 1, 1, encInfo -> fptr_stego_image);
        }
    }
    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char buffer[32];
    fread(buffer, 1, 32, encInfo -> fptr_src_image);
    for( int i = 31; i >= 0; i-- )
    {
        buffer[i] = buffer[i] & (~1);
        if( file_size & (1 << i))
        {
            buffer[31-i] = (buffer[31-i] | 1);
        }
    }
    fwrite(buffer, 1, 32, encInfo -> fptr_stego_image);
    return e_success;
}

Status encode_secret_file_data(EncodeInfo * encInfo)
{
    rewind(encInfo -> fptr_secret);
    char file_data, str_char;
    for(int i = 0; i < encInfo -> size_secret_file; i++)
    {
        file_data = fgetc(encInfo -> fptr_secret);
        for(int j = 7; j >= 0; j--)
        {
            fread(&str_char, 1, 1, encInfo -> fptr_src_image);
            str_char = str_char & (~1);
            if(file_data & (1<<j))
            {
                str_char = str_char | 1;
            }
            fwrite(&str_char, 1, 1, encInfo -> fptr_stego_image);
        }
    }
    return e_success;
}

Status copy_remaining_img_data(FILE *p1, FILE *p2)
{
    char ch[1];
    while( fread(ch, 1, 1, p1) > 0)
    {
        fwrite(ch ,1 , 1, p2);
    }
    return e_success;

} 
