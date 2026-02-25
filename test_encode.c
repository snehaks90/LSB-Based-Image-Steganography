#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include <stdlib.h>
#include "decode.h"


int main(int argc, char *argv[])
{   
    if (argc < 2)
    {
        printf("Usage:\n");
        printf("./a.out -e <source.bmp> <secret_file> [output.bmp]\n");
        printf("./a.out -d <stego.bmp> [output_file]\n");
        return 0;
    }

    EncodeInfo encInfo;
    uint img_size;
    DecodeInfo decInfo;
    
    OperationType op = check_operation_type(argv);

    if(op  == e_encode)
    {
        if(read_and_validate_encode_args(argc,argv, &encInfo) == e_success)
        {
            do_encoding(&encInfo);
        }
        else
        {
          printf("Encode argument validation failed!\n");
        }
    }
    else if(op  == e_decode)
    {  
        if (read_and_validate_decode_args(argc, argv, &decInfo) == e_success)
        {
            do_decoding(&decInfo);
        }
       else
        {
          printf("Decode argument validation failed!\n");
        }
    }
        
    else
    {
        printf("Unsupported operation type\n");
    }

   
    return 0;
}

OperationType check_operation_type(char *argv[])
{
        if(strcmp(argv[1], "-e") == 0)
        {
            return e_encode;
        }
        else if(strcmp(argv[1], "-d") == 0)
        {
            return e_decode;
        }
        else
        {
            return e_unsupported; 
        }

}
Status read_and_validate_encode_args(int argc, char *argv[], EncodeInfo *encInfo)
{   
    
    if(argc < 4)
    { 
        printf("Insufficient arguments!\n");
        return e_failure;
    }
    else
    {    // Step 1: validate source filename
        char *dot1 = strrchr(argv[2], '.'); // Find last '.' in filename

        if(dot1 == NULL || strcmp(dot1, ".bmp") != 0)    // if dot character is not found returns NULL
        {
            printf("No .bmp extension found in source file name!\n");
            return e_failure;
        }
        else
        {  
            printf("Extension is: %s\n", dot1);
           encInfo-> src_image_fname = argv[2] ;   // Store filename in structure
        
           // Step 2: validate secret filename
           char *dot2 = strrchr(argv[3], '.');
           if(dot2 == NULL)
           {
             printf("Secret file has no extension!\n");
             return e_failure;
           }
           else
           {
                encInfo->secret_fname  = argv[3];  // Store secret filename in structure
               
                // Step 3: Handle Output File
                if(argc == 4)
                {
                     // If no output file is given, use default "stego.bmp"
                    printf("Output file not provided. Using default: stego.bmp\n");
                    encInfo->stego_image_fname = "stego.bmp";
                }
                else
                {    
                    // Check if provided output file has a .bmp extension
                     char *dot3 = strrchr(argv[4], '.');
                       if(dot3 == NULL || strcmp(dot3, ".bmp") != 0)
                       {
                          printf("No.bmp  extension found in output file name!\n");
                          return e_failure;
                       }
                       else
                       {
                        printf("Yes bmp\n");
                        encInfo->stego_image_fname = argv[4];  // store output filename to structure
                       }
                       
                }
           }
        }
    }
    return e_success;
    
}
Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfo *decInfo)
{
    if(argc < 3)
    { 
        printf("Insufficient arguments!\n");
        return e_failure;
    }
    else
    {    
        char *dot1 = strrchr(argv[2], '.'); // Find last '.' in filename

        if(dot1 == NULL || strcmp(dot1, ".bmp") != 0)    // if dot character is not found returns NULL
        {
            printf("No .bmp extension found in source file name!\n");
            return e_failure;
        }
        else
        {
           decInfo->stego_image_fname = argv[2];
            if (argc == 3)
            {
              printf("Output file not provided. Using default: secretdata\n");
             decInfo->secret_file_fname = malloc(50);
               strcpy(decInfo->secret_file_fname, "secretdata");
            }
           else
            {
                 decInfo->secret_file_fname = malloc(strlen(argv[3]) + 10);
                  strcpy(decInfo->secret_file_fname, argv[3]);
            }

        }
    }
  
    return e_success;
}



    // Test get_image_size_for_bmp
   /* img_size = get_image_size_for_bmp(encInfo.fptr_src_image);
    printf("INFO: Image size = %u\n", img_size);
    */
