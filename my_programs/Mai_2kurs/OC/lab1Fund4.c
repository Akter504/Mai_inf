#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

enum errors 
{
    error_path_input,
    error_empty_or_incorrect_input,
    error_flag,
    error_hex,
    input_correct
};

int check_sixteen(char *str)
{
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++)
    {
        if ((str[i] < '0') || (str[i] > '9') && (str[i] < 'A' || str[i] > 'F'))
        {
            return 0;
        }
    }
    return 1;
}


enum errors check(int len, char* argv[]) 
{
    FILE* input = fopen(argv[1],"rb");
    if (input == NULL)
    {
        return error_path_input;
    }
    fclose(input);
    if (len == 3)
    {
       if (strcmp(argv[2],"xor8") == 0 || strcmp(argv[2],"xor32") == 0)
       {
        return input_correct;
       }
       else
       {
        return error_flag;
       }
    }
    else if (len == 4)
    {
        if (strcmp(argv[2],"mask") == 0)
        {
            if (check_sixteen(argv[3]))
            {
                return input_correct;
            }
            else
            {
                return error_hex;
            }
        }
        else
        {
            return error_flag;
        }
    }
    else
    {
       return error_empty_or_incorrect_input;
    }             
}

void xor8_func(FILE* input, FILE* output)
{
    int c;
    uint8_t key = 0xAA;
    while ((c = fgetc(input)) != EOF)
    {
        fputc(c ^ key, output);
    }
}

void xor32_func(FILE* input, FILE* output)
{
    uint8_t group[4];
    uint8_t key = 0xAA;
    int bytesRead;

    while ((bytesRead = fread(group, 1, sizeof(group), input)) == sizeof(group)) 
    {
        for (int i = 0; i < 4; i++) 
        {
            group[i] ^= key;
        }
        fwrite(group, 1, sizeof(group), output);
    }
    if (bytesRead > 0) 
    {
        while (bytesRead < 4) 
        {
            group[bytesRead] = 0;
            bytesRead++;
        }
        for (int i = 0; i < 4; i++) 
        {
            group[i] ^= key;
        }
        fwrite(group, 1, bytesRead, output);
    }
}

size_t mask_func(FILE* input, FILE* output, char* mask) {
    int h_mask = strtol(mask, NULL, 16);
    int value;
    size_t count = 0;

    while (fread(&value, sizeof(value), 1, input) == 1) {
        if ((value & h_mask) == h_mask) {
            count++;
        }
    }
    return count;
}

int main(int argc, char* argv[]) 
{
    FILE* input;
    FILE* output;
    switch (check(argc, argv)) 
    {
        case error_path_input:
            printf("This input path is incorrect or there is no such file\n");
            break;
        case error_empty_or_incorrect_input:
            printf("Input is empty or incorrect.\n");
            break;
        case error_flag:
            printf("Error: incorrect flag.\n");
            break;
        case error_hex:
            printf("The entered mask must be in the 16th number system.\n");
            break;
        case input_correct:
            input = fopen(argv[1],"rb");
            output = fopen("out_lab4.bin", "wb");
            if (output == NULL)
            {
                perror("Error: cant find output file\n");
                return 1;
            }
            if (strcmp(argv[2],"xor8") == 0)
            {
                xor8_func(input,output);
                fclose(input);
                fclose(output);
            }
            else if (strcmp(argv[2],"xor32") == 0)
            {
                xor32_func(input,output);
                fclose(input);
                fclose(output);
            }
            else if (strcmp(argv[2],"mask") == 0)
            {
                size_t res = mask_func(input,output,"0x12345678");
                printf("%d\n",res);
                fclose(input);
                fclose(output);
            }
            break;
    }
}
