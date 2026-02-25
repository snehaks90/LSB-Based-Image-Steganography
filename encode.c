#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>

Status do_encoding(EncodeInfo *encInfo)
{ 
    printf("INFO: ## Encoding Procedure Started ##\n");


  // Open all necessary files: source image, secret file, and output stego image
    if(open_files(encInfo) != e_success)
      return e_failure;
    
    // Set secret file size
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
      
    // Extract extension here
    char *dot = strrchr(encInfo->secret_fname, '.');
    if (dot)
        strcpy(encInfo->extn_secret_file, dot);
    
     // Check if the source image has enough capacity to hold the secret data
    if(check_capacity(encInfo) != e_success)
       return e_failure;
    
     // Copy the BMP header (first 54 bytes) from source image to stego image
    if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success)
         return e_failure;

    // Encode the magic string to identify this as a stego image
    if(encode_magic_string(MAGIC_STRING, encInfo) != e_success)
               return e_failure;
    
    // Encode the length of the secret file's extension
    if(encode_secret_file_extn_size(encInfo->extn_secret_file, encInfo) != e_success)
                return e_failure;

    // Encode the secret file extension itself 
    if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo) != e_success)
                return e_failure;
    
    // Encode the size of the secret file
    if(encode_secret_file_size(encInfo->size_secret_file, encInfo) != e_success)
           return e_failure;
    
    // Encode the actual secret file data byte by byte
    if(encode_secret_file_data(encInfo) != e_success)
       return e_failure;

   //copy the remaining data of beautiful.bmp to stego.bmp
   if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success)
      return e_failure;

  printf("INFO: ## Encoding Done Successfully ##\n");

  return e_success;
}




/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
   // printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
   // printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

// to get secret file size
uint get_file_size(FILE *fptr)
{   
    uint size;
    fseek(fptr, 0 ,SEEK_END);
    size = ftell(fptr);
    fseek(fptr, 0 ,SEEK_SET);
    return size;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{   
  printf("INFO: Opening required files\n");
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
      
    	return e_failure;
    }
    printf("INFO: Opened %s\n", encInfo->src_image_fname);



    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }
    printf("INFO: Opened %s\n", encInfo->secret_fname);

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }
    printf("INFO: Opened %s\n", encInfo->stego_image_fname);

    printf("INFO: File opening Done\n");


    // No failure return e_success
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{ 
   printf("INFO: Checking for %s size\n", encInfo->secret_fname);

  int image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);

  int magic_len = strlen(MAGIC_STRING);
  int extn_len = strlen(encInfo->extn_secret_file);
  int secret_data = get_file_size(encInfo->fptr_secret);
  int Encode_data = (magic_len + sizeof(int) +  extn_len + sizeof(int) + secret_data) * 8;
  
  printf("INFO: Done. Not Empty\n");
  printf("INFO: Checking for %s capacity to handle %s\n",
       encInfo->src_image_fname, encInfo->secret_fname);
  printf("INFO: Done. Found OK\n");

  if(Encode_data <=  image_capacity)
  return e_success;
  else
  return e_failure;


}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{  
    printf("INFO: Copying Image Header\n");


    unsigned char header[54];
    fseek(fptr_src_image, 0, SEEK_SET);

    fread(header, 1, 54, fptr_src_image);
    fwrite(header, 1, 54, fptr_dest_image);
    printf("INFO: Done\n");

    return e_success;

}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)  //magic_string is a pointer to a constant character array, i.e., a string that should not be modified by this function."
{ 
  printf("INFO: Encoding Magic String Signature\n");

  if(encode_data_to_image(magic_string, strlen(magic_string), encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
  { 
    printf("INFO: Done\n");  
    return e_success;
  }
  else
    return e_failure;
  

}

Status encode_data_to_image(const unsigned char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{ 
  unsigned char image_buffer[8];
   
  for(int i = 0; i < size; i++)
  {   
       fread( image_buffer, 8, 1, fptr_src_image);  //data[i] = 1 byte (8 bits) of your secret data
       if(encode_byte_to_lsb(data[i], image_buffer) != e_success)   //image_buffer = 8 bytes from your source image/array of 8 image bytes 
            return e_failure;
       else
          fwrite( image_buffer, 8, 1, fptr_stego_image);      
  }
  return e_success;
}

Status encode_byte_to_lsb(char data, unsigned char *image_buffer)
{
   for(int i = 0; i < 8; i++)
   {
      uint bit = (data >> i) & 1;                      // extract i-th bit (either 0 or 1)
       image_buffer[i] = (image_buffer[i] & ~1) | bit;           // put it into LSB
   }
   return e_success;
}

Status encode_int_to_lsb(int size, unsigned char *image_buffer)
{
   for(int i = 0; i < 32; i++)
   {
      uint bit = (size >> i) & 1;          // extract i-th bit (either 0 or 1)
      image_buffer[i] = (image_buffer[i] & ~1) | bit;  // put it into LSB
   }
   return e_success;
}


Status encode_secret_file_extn_size(const char *file_extn, EncodeInfo *encInfo)
{
  printf("INFO: Encoding %s File Extenstion\n", encInfo->secret_fname);

  unsigned char buffer[32];
  fread(buffer, 1, 32, encInfo->fptr_src_image);

  if(encode_int_to_lsb(strlen(file_extn), buffer) == e_success)
   { 
    fwrite(buffer, 1, 32, encInfo->fptr_stego_image);
    printf("INFO: Done\n");
    return e_success;
   }
   else
    return e_failure;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
   if(encode_data_to_image(file_extn, strlen(file_extn), encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
   return e_success;
  else
   return e_failure;


}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
   printf("INFO: Encoding %s File Size\n", encInfo->secret_fname);

  unsigned char buffer[32];
    fread(buffer, 1, 32, encInfo->fptr_src_image);

    if(encode_int_to_lsb((int)file_size, buffer) == e_success)
    {
       fwrite(buffer, 1, 32, encInfo->fptr_stego_image);
       printf("INFO: Done\n");
       return e_success;
    }
    else
      return e_failure;
  
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
   printf("INFO: Encoding %s File Data\n", encInfo->secret_fname);


    unsigned char ch;
 // Read each byte from the secret file
    while (fread(&ch, 1, 1, encInfo->fptr_secret) == 1)
    {   
      // Each byte (8 bits) gets encoded into 8 image bytes
        if(encode_data_to_image(&ch, 1, encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success)
        { 
          printf("INFO: Done\n");
          return e_failure;
        }
    }
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_stego_image)
{ 
  printf("INFO: Copying Left Over Data\n");

  unsigned char buffer[1];
  while (fread(buffer, 1, 1, fptr_src) == 1)
    {
        fwrite(buffer, 1, 1, fptr_stego_image);
    }
  
printf("INFO: Done\n");

  return e_success;
}
