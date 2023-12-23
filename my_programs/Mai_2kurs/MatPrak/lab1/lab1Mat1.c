#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

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

int tollower(char c){
   if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    } 
    else {
        return c;
    }
}

int char_to_int(char elem){
    if ((elem >= '0') && (elem <= '9')){
       return elem - '0';
    }
    else if((tollower(elem) >= 'a') && (tollower(elem) <= 'z')){
       return tollower(elem) - 'a' + 10;
    }
}

int all_to_ten(char *buffer, int base) {
    size_t len = strlen(buffer);
    int res = 0;
    int sign = 1;
    if (buffer[0] == '-') {
        sign = -1;
        buffer++; 
        len--;
    }

    for (size_t i = 0; i < len; i++) {
        res = res * base + char_to_int(buffer[i]);
    }
    return sign * res;
}



int main(int argc, char* argv[]) 
{
    FILE* input_path = NULL;
    char filename[256];
    switch (check(argc, argv,&input_path,filename)) 
    {
        case error_path_input:
            printf("This input path is incorrect or there is no such file\n");
            break;
        case error_empty_or_incorrect_input:
            printf("Input is empty or incorrect.\n");
            break;
        case input_correct:
            unsigned char bytes[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
            size_t count_bytes = sizeof(bytes);
            size_t count_written = fwrite(bytes, 1, count_bytes, input_path);
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
            while ((byte = fgetc(output)) != EOF) 
            {
               printf("%d ", byte);
            }
            printf("\n");
            fclose(output); 
            break;
         }
}
