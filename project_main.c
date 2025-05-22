#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "decode.h"
#include <string.h>

int main(int argc, char*argv[])
{
	if(!strcmp(argv[1], "-e"))
	{
		if(argc == 4 || argc == 5)
		{
			EncodeInfo encInfo;
			if(read_and_validate_encode_args(argv, &encInfo) == e_success)
				printf("Reading and validation of encoding arguments------------> successful\n");
			else
				printf("Reading and validation of encoding arguments ------------->unsuccessful\n");
			if(do_encoding(&encInfo) == e_success)
				printf("Encoding is Complete --------------------->Successful\n");
			else
				printf("Encoding is Incomplete --------------------->Unsuccessful\n");
		}
		else
			printf("Insufficient Arguments\n");
	}
	else if(!strcmp(argv[1], "-d"))
	{
		if(argc == 3 || argc == 4)
		{
			DecodeInfo decInfo;
			if(read_and_validate_decode_args(argv, &decInfo) == e_success)
				printf("Reading and validation of decoding arguments is ----------------->Successful\n");
			else
				printf("Reading and validation of decoding arguments --------------->Unsuccessfull\n");
				
			if(do_decoding(&decInfo) == e_success)
				printf("Decoding is Complete ------------------------->Successful\n");
			else
				printf("Decoding is Incomplete --------------------->Unsuccessful\n");
				
		}
		else
			printf("Insufficient Arguments\n");
	}
	else
		printf("Invalid CLA\n");
    return 0;
}
