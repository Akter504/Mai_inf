#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void copy_file(FILE* input, FILE* output)
{
    int c;
    while ((c = fgetc(input)) != EOF)
    {
        fputc(c,output);
    }
}

int main() 
{
    char file_input[100];
    char file_output[100];
    printf("Enter the name of the file from which you want to copy the data: ");
    scanf("%s", file_input);
    printf("\n");
    FILE* input = fopen(file_input, "rb");
    if (input == NULL)
    {
        printf("The entered input file was not found\n");
        return 1;
    }

    printf("Enter the name of the file to which you need to copy the data: ");
    scanf("%s", file_output);
    printf("\n");
    FILE* output = fopen(file_output, "wb");
    if (output == NULL)
    {
        printf("The entered output file was not found\n");
        fclose(input);
        return 2;
    }
    copy_file(input,output);

    fseek(input, 0, SEEK_END);
    fseek(output, 0, SEEK_END);
    long input_size = ftell(input);
    long output_size = ftell(output);
    fseek(input, 0, SEEK_SET);
    fseek(output, 0, SEEK_SET);
    if (input_size == output_size) 
    {
        printf("Data successfully copied from input to output.\n");
    } 
    else 
    {
        printf("Data copy was not successful.\n");
    }
    fclose(input);
    fclose(output);
}
