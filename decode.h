#ifndef DECODE_H
#define DECODE_H


#include "types.h" // Contains user defined types

typedef struct _DecodeInfo
{ 
    char *stego_image_fname; //store output.bmp filename
    FILE *fptr_stego_image;
    
    int secret_filesize;   // 32-bit decoded value
    unsigned int size_of_extn;

    char *secret_file_fname;   
    FILE *fptr_secret_file; 
     
} DecodeInfo;

/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfo *decInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_image_file(DecodeInfo *decInfo);

Status skip_bmp_header(DecodeInfo *decInfo);

Status decode_magic_string(DecodeInfo *decInfo);

Status decode_secret_file_extn_size(DecodeInfo *decInfo);

Status decode_secret_file_extn(DecodeInfo *decInfo);

Status decode_secret_file_size(DecodeInfo *decInfo);

Status decode_secret_file_data(DecodeInfo *decInfo);

Status decode_byte_from_lsb(unsigned char *image_buffer, unsigned char *decoded_byte);

Status decode_int_from_lsb(unsigned char *image_buffer, unsigned int *decoded_int);



#endif