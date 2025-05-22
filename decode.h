#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types
typedef struct _DecodeInfo
{
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* Output text file info */
    char *output_text_fname;
    FILE *fptr_output_text;

} DecodeInfo;


Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

Status do_decoding(DecodeInfo *decInfo); 

Status open_file(DecodeInfo *decInfo); 

Status decode_magic_string_len(FILE *fptr_src);

Status decode_magic_string(FILE *fptr_src, int len);

Status decode_secret_file_extension(FILE *fptr_src);

Status decode_secret_message(FILE *fptr_src, FILE *fptr_output_text);

#endif

