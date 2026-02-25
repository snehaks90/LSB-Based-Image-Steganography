#ifndef ENCODE_H
#define ENCODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1   //Only 1 byte of secret data is processed at a time.
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)  //Since 1 byte = 8 bits, you need 8 image bytes to store 1 secret byte (each bit in one image pixel byte’s LSB).
#define MAX_FILE_SUFFIX 4    //For example, .txt has 4 characters — enough space to store file extension.

typedef struct _EncodeInfo
{
    /* Source Image info */
    char *src_image_fname;   // store the beautiful.bmp filename
    FILE *fptr_src_image;    // store the beautiful.bmp file address
    uint image_capacity;  //store the file size
    char image_data[MAX_IMAGE_BUF_SIZE]; //store the image data

    /* Secret File Info */
    char *secret_fname;   // store the secret.txt filename
    FILE *fptr_secret;    // store the secret.txt file address
    char extn_secret_file[MAX_FILE_SUFFIX];    //to store the secret file extension
    char secret_data[MAX_SECRET_BUF_SIZE];   // to store file data ,secret_data is just 1 byte (8 bits).
    long size_secret_file;       //store size of secret.xt file

    /* Stego Image Info */
    char *stego_image_fname; //store output.bmp filename
    FILE *fptr_stego_image;

} EncodeInfo;


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(int argc, char *argv[], EncodeInfo *encInfo);

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
uint get_file_size(FILE *fptr);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* Store Magic String */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo);

/* Encode secret file extenstion */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo);

Status encode_secret_file_extn_size(const char *file_extn, EncodeInfo *encInfo);

/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo);

/* Encode function, which does the real encoding */
Status encode_data_to_image(const unsigned char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, unsigned char *image_buffer);

Status encode_int_to_lsb(int size, unsigned char *image_buffer);


/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

#endif
