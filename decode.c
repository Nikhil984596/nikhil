#include <stdio.h>
#include <string.h>
#include"decode.h"
#include "types.h"

Status do_decoding(DecodeInfo * decInfo)
{
    if(decode_magic_string_len(decInfo -> fptr_stego_image) == e_success)
		printf("Decoding magic string length -------------------->successful\n");
    else
		printf("Decoding magic string length ------------------>unsuccessful\n");
    if(decode_secret_file_extension(decInfo -> fptr_stego_image) == e_success)
		printf("Decoding secret file extension ------------------>successful\n");
    else
		printf("Decoding secret file extension ---------------->unsuccessful\n");
    if(decode_secret_message(decInfo -> fptr_stego_image, decInfo -> fptr_output_text ) == e_success)
		printf("Decoding secret message ------------------------->successful\n");
    else
		printf("Decoding secret message ----------------------->unsuccessful\n");
	return e_success;
}

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    decInfo -> stego_image_fname = argv[2];
    if(argv[3] == NULL)
    {   
        decInfo -> output_text_fname = "output.txt";
    }   
    else
    {   
        decInfo -> output_text_fname = argv[3];
    }   

    if(open_file(decInfo) == e_success)
		printf("File opening ------------------------------------>successful\n");
    else
		printf("File opening ----------------------------------->unsuccessful\n");
    //check file is bmp or not!!
	
    char data[2];
    fread(data, 1, 2, decInfo -> fptr_stego_image);
    if(data[0] == 0X42 && data[1] == 0X4d)
		printf("BMP file validation ------------------------------->successful\n");
    else
		printf("BMP file validation ----------------------------->unsuccessful\n");

    char cptr[20];
	strcpy(cptr, decInfo -> output_text_fname);
    char *out_extn = strtok(cptr, ".");
    out_extn = strtok(NULL, ".");
	if(!strcmp(out_extn, "txt"))
		printf("Checking of Output File Extension ------------------>successful\n");
	else
		printf("Checking of Output File Extension ---------------->unsuccessful\n");
	return e_success;
}


Status open_file(DecodeInfo * decInfo)
{
    decInfo -> fptr_stego_image = fopen(decInfo -> stego_image_fname, "r");
    if(decInfo -> fptr_stego_image == NULL)
        return e_failure;
    decInfo -> fptr_output_text = fopen(decInfo -> output_text_fname, "w");
    if(decInfo -> fptr_output_text == NULL)
        return e_failure;
    else
        return e_success;
}

Status decode_magic_string_len(FILE *fptr_src)
{
    fseek(fptr_src, 54, SEEK_SET);
    char buffer[32];
    fread(buffer, 1, 32, fptr_src);
    int len = 0;
    for(int i = 31; i >= 0; i--)
    {
        len = len | (buffer[31 - i] & 1) << (i);
    }
   decode_magic_string(fptr_src, len);
   return e_success;
}

Status decode_magic_string(FILE *fptr_src, int len)
{
    char ms[len], buffer[8], ch;
    for(int i = 0; i < len; i++)
    {
        ch = 0;
        fread(buffer, 1, 8, fptr_src);
        for(int j = 0; j <= 7; j++)
        {
            ch = ch | ((buffer[j] & 1) << (7-j));
        }
        ms[i] = ch;
    }
    return e_success;
}

Status decode_secret_file_extension(FILE *fptr_src)
{
    char buffer[32];
    fread(buffer, 1, 32, fptr_src);
    int len = 0;
    for(int i = 31; i >= 0; i--)
    {
        len = len | (buffer[31 - i] & 1) << (i);
    }
   fseek(fptr_src, len*8, SEEK_CUR);
   return e_success;
}

Status decode_secret_message(FILE *fptr_src, FILE *fptr_dest)
{
    char buffer[32];
    fread(buffer, 1, 32, fptr_src);
    int len = 0;
    for(int i = 31; i >= 0; i--)
    {
        len = len | (buffer[31 - i] & 1) << (i);
    }

    int i;
    char secret[len], buffer1[8], ch;
    for( i = 0; i < len; i++)
    {
        ch = 0;
        fread(buffer1, 1, 8, fptr_src);
        for(int j = 0; j <= 7; j++)
        {
            ch = ch | ((buffer1[j] & 1) << (7-j));
        }
        fwrite(&ch,1, 1, fptr_dest);
    }
    return e_success;
}

