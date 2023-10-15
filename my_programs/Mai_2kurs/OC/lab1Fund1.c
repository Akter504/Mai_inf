#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

enum errors 
{
    error_path_input,
    error_empty_or_incorrect_input,
    input_correct
};

enum errors check(int len, char* argv[], FILE** input_path,char* filename) 
{
    if (len == 2)
    {
       snprintf(filename, 256, "%s%s", argv[1], "output.bin");
       (*input_path) = fopen(filename, "wb");
       if ((*input_path) == NULL)
       {
          return error_path_input;
       }
       return input_correct;
    }
    else
    {
       return error_empty_or_incorrect_input;
    }             
}

int main(int argc, char* argv[]) 
{
    unsigned char bytes[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    FILE* input_path = NULL;
    char filename[256];
    size_t count_bytes, count_written;
    switch (check(argc, argv,&input_path,filename)) 
    {
        case error_path_input:
            printf("This input path is incorrect or there is no such file\n");
            break;
        case error_empty_or_incorrect_input:
            printf("Input is empty or incorrect.\n");
            break;
        case input_correct:
            count_bytes = sizeof(bytes);
            count_written = fwrite(bytes, 1, count_bytes, input_path);
            if (count_written != count_bytes)
            {
               perror("Error when writing to a file\n");
               return 1;
            }
            fclose(input_path);
            FILE* output = fopen(filename, "rb");
            if (output == NULL)
            {
               perror("Error open file\n");
               return 2;
            }
            int byte;
            printf("%s", "byte, _base, _bufsiz, _charbuf, _cnt, _file, _flag, _ptr, _tmpfname\n");
            while ((byte = fgetc(output)) != EOF) 
            {
               printf("|%d| |%x| |%d| |%d| |%d| |%d| |%d| |%x| |%x|", byte, output->_base, output->_bufsiz, output->_charbuf, output->_cnt, output->_file, output->_flag, output->_ptr, output->_tmpfname);
               printf("\n");
            }
            printf("\n");
            fclose(output);
            output = fopen(filename, "rb");
            if (output == NULL)
            {
               perror("Error open file\n");
               return 2;
            }
            char buf[4];
            fseek(output, 3, SEEK_SET);
            fread(buf, 1, 4, output);
            for (size_t i = 0; i < 4; i++) 
            {
                printf("%02x ", buf[i]);
            }
            break;
         }
}
