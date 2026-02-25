#include <stdio.h>
#include "decode.h"
#include "types.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>


Status do_decoding(DecodeInfo *decInfo)
{   
  printf("INFO: ## Decoding Procedure Started ##\n");

    if(open_image_file(decInfo) != e_success)
      return e_failure;

    if(skip_bmp_header(decInfo)!= e_success)
       return e_failure;

    if(decode_magic_string(decInfo) != e_success)
       return e_failure;

   if(decode_secret_file_extn_size(decInfo) != e_success)
       return e_failure;

    if(decode_secret_file_extn(decInfo) != e_success)
       return e_failure;

    if(decode_secret_file_size(decInfo) != e_success)
      return e_failure;

    if(decode_secret_file_data(decInfo) != e_success)
      return e_failure;

   printf("INFO: ## Decoding Done Successfully ##\n");

   return e_success; 
}

Status open_image_file(DecodeInfo *decInfo)
{  
   printf("INFO: Opening required files\n");
   decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    if (decInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

    	return e_failure;
    }
    printf("INFO: Opened %s\n", decInfo->stego_image_fname);
    printf("INFO: Done\n");

   return e_success;
}

Status skip_bmp_header(DecodeInfo *decInfo)
{   
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
    return e_success;

}

Status decode_magic_string(DecodeInfo *decInfo)
{   
  printf("INFO: Decoding Magic String Signature\n");

    char magicstring[3];
    unsigned char image_buffer[8];
    
    for(int i = 0; i < 2; i++)
    {  
       fread(image_buffer, 1, 8, decInfo->fptr_stego_image);
       unsigned char byte;
       decode_byte_from_lsb(image_buffer, &byte);
       magicstring[i] = byte;
    }
     magicstring[2] = '\0';

   if(strcmp(MAGIC_STRING, magicstring) != 0)
    {
      printf("Magic string mismatch!\n");
      return e_failure;
    }
  printf("INFO: Done\n");
 return e_success;   

}
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{   
  printf("INFO: Decoding Output File Extenstion\n");

    unsigned char buffer[32];

    // read 32 bytes from the encoded image
    fread(buffer, 1, 32, decInfo->fptr_stego_image);
    decode_int_from_lsb(buffer, &decInfo->size_of_extn);

    printf("INFO: Done\n");

    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char image_buffer[8];
    char ch;

    char extn[10];  // Supports extensions like .txt, .c, .jpg
    for (int i = 0; i < decInfo->size_of_extn; i++)
    {
        fread(image_buffer, 1, 8, decInfo->fptr_stego_image);
        decode_byte_from_lsb(image_buffer, &ch);
        extn[i] = ch;
    }
    extn[decInfo->size_of_extn] = '\0';
    
    strcat(decInfo->secret_file_fname, extn);
    decInfo->fptr_secret_file = fopen( decInfo->secret_file_fname, "wb");
    if (decInfo->fptr_secret_file == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->secret_file_fname);

    	return e_failure;
    }
    printf("INFO: Opened %s\n", decInfo->secret_file_fname);
    printf("INFO: Done. Opened all required files\n");


    return e_success;
}


Status decode_secret_file_size(DecodeInfo *decInfo)
{
  printf("INFO: Decoding File Size\n");
  unsigned char buffer[32];

    // read 32 bytes from the encoded image
    fread(buffer, 1, 32, decInfo->fptr_stego_image);
    decode_int_from_lsb(buffer, (unsigned int*)&decInfo->secret_filesize);
    printf("INFO: Done\n");

    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{   
     printf("INFO: Decoding File Data\n");

    char image_buffer[8];
    char data_byte;
    for (int i = 0; i < decInfo->secret_filesize; i++)
    {
        fread(image_buffer, 1, 8, decInfo->fptr_stego_image);
        decode_byte_from_lsb(image_buffer, &data_byte);
        fwrite(&data_byte, 1, 1, decInfo->fptr_secret_file);
    }
    printf("INFO: Done\n");
    return e_success;
}


Status decode_byte_from_lsb(unsigned char *image_buffer, unsigned char *decoded_char)
{
   unsigned char value = 0;
   for(int i = 0; i< 8; i++)
   {
    uint get = image_buffer[i] & 1;  // extract LSB
    value = value | (get << i);   // place bit in correct position, because encoding stored LSB first
   }
   *decoded_char = value;

   return e_success;

}

Status decode_int_from_lsb(unsigned char *image_buffer, uint *decoded_int)
{
  
   uint value = 0;
   for(int i = 0; i< 32; i++)
   {
    uint get = image_buffer[i] & 1;  // extract LSB
    value = value | (get << i);   // place bit in correct position, because encoding stored LSB first
   }

   *decoded_int = value;

   return e_success;
}