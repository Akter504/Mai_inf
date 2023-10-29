#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>

size_t strlen_f(const char* str)

{
    size_t i;
    for (i = 0; str[i] != '\0'; i++);
    return i;
}

int strcmp_f(const char* str1, const char* str2) 
{
    while (*str1) 
    {
        if (*str1 != *str2) 
        {
            break;
        }
        str1++;
        str2++;
    }
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

enum errors 
{
    error_incorrect_flag,
    error_file,
    error_not_uns_int,
    error_allocation_memory,
    input_correct
};

typedef struct Parts
{
    int** num_of_line;
    char* name_of_file;
    int* count_answers;
    int* num_of_lines;
    int count_lines;
}Answer;

int is_substring(const char *substring, const char *line, int* indices) 
{
    int substring_length = strlen_f(substring);
    int line_length = strlen_f(line);
    int found = 0;
    for (int j = 0; j <= line_length - substring_length; j++) 
    {
        int k;
        for (k = 0; k < substring_length; k++) 
        {
            if (line[j + k] != substring[k]) 
            {
                break;
            }
        }
        if (k == substring_length) 
        {
            indices[found] = j;
            found++;
        }
    }
    return found;
}

enum errors check(Answer** remember, int* i, const char *substring, ...) 
{
    va_list args;
    va_start(args, substring);
    char *file_path;
    while ((file_path = va_arg(args, char *)) != NULL) 
    {
        FILE *file = fopen(file_path, "r");
        if (file == NULL) 
        {
            return error_file;
        }
        char line[1024];
        int line_num = 0;
        (*remember)[*i].name_of_file = strdup(file_path);
        (*remember)[*i].count_answers = NULL;
        (*remember)[*i].num_of_line = (int**)malloc(1);
        (*remember)[*i].num_of_lines = (int*)malloc(1);
        (*remember)[*i].count_lines = line_num;
        int file_line_num = 0;
        if (((*remember)[*i].num_of_line == NULL || (*remember)[*i].num_of_lines == NULL) && *i != 0)
        {
            for (int j = 0; j < *i; j++) // num_file 
            {
                free((*remember)[j].name_of_file);

                for (int k = 0; k < (*remember)[j].count_lines; k++) 
                {
                    free((*remember)[j].num_of_line[k]);
                }
                free((*remember)[j].num_of_line);
                free((*remember)[j].count_answers);
                free((*remember)[j].num_of_lines);
            }
            return error_allocation_memory;
        }
        while (fgets(line, sizeof(line), file) != NULL) 
        {
            int index[1024];
            int found = is_substring(substring, line, index);
            if (found > 0) 
            {
                (*remember)[*i].num_of_line = (int**)realloc((*remember)[*i].num_of_line, (line_num + 1)*2 * sizeof(int*));
                (*remember)[*i].count_answers = (int*)realloc((*remember)[*i].count_answers, (line_num + 1)*2 * sizeof(int));
                (*remember)[*i].num_of_lines = (int*)realloc((*remember)[*i].num_of_lines, (line_num+1)*2*sizeof(int));
                (*remember)[*i].num_of_line[line_num] = (int*)malloc(found * sizeof(int));
                if ((*remember)[*i].num_of_line[line_num] == NULL && *i != 0)
                {
                    for (int j = 0; j < *i; j++)
                    {
                        free((*remember)[j].name_of_file);

                        for (int k = 0; k < (*remember)[j].count_lines; k++) 
                        {
                            free((*remember)[j].num_of_line[k]);
                        }
                        free((*remember)[j].num_of_line);
                        free((*remember)[j].count_answers);
                        free((*remember)[j].num_of_lines);
                    }
                    return error_allocation_memory;
                }
                for (int j = 0; j < found; j++) 
                {
                    (*remember)[*i].num_of_line[line_num][j] = index[j];
                }
                (*remember)[*i].count_answers[line_num] = found;

                line_num++;
                (*remember)[*i].num_of_lines[line_num-1] = file_line_num;
                (*remember)[*i].count_lines = line_num;
            }
            file_line_num++;
        }
        fclose(file);
        (*i)++;
    }
    va_end(args);
    return input_correct;
}

int main(int argc, char* argv[]) 
{
    Answer* remember = (Answer*)malloc(sizeof(Answer) * 4);
    if (remember == NULL)
    {
        printf("Error: allocated memory\n");
        return 2;
    }
    printf("Input your substring:\n");
    char* substr = (char *)malloc(sizeof(char)*257);
    char* file_path = (char *)malloc(sizeof(char)*257);
    if (substr == NULL || file_path == NULL)
    {
        printf("Error: allocated memory\n");
        free(substr);
        free(file_path);
        return 2;
    }
    fgets(substr, 257, stdin);
    size_t len_s = strlen_f(substr);
    if (len_s == 257)
    {
        printf("Error: Max alphas in substring 256\n");
        return 1;
    }
    if (len_s > 0 && substr[len_s - 1] == '\n') {
        substr[len_s - 1] = '\0';
    }
    int num_files = 0;
    int allocated_memory = 4;
    int i = 0;
    while (1) 
    {
        printf("Enter another file path or 'q' to finish:\n");
        scanf("%256s", file_path);

        if (strlen_f(file_path) == 257) 
        {
            printf("Error: Max alphas in file_path 256\n");
            return 1;
        }

        if (file_path[0] == 'q') 
        {
            break;
        }

        if (num_files >= allocated_memory) 
        {
            allocated_memory *=2;
            remember = (Answer*)realloc(remember, allocated_memory * sizeof(Answer));
        }
        enum errors result = check(&remember,&i, substr, file_path,"2.txt","3.txt", NULL);

        if (result != input_correct && result != error_allocation_memory) 
        {
            printf("Error: incorrect input\n");
            free(remember);
            free(substr);
            free(file_path);
            return 1;
        }
        else if (result == error_allocation_memory)
        {
            printf("Memory allocation error\n");
            free(remember);
            free(substr);
            free(file_path);
            return 2;
        }
        num_files++;
    }
    if (num_files > 0) 
    {
        for (int i = 0; i < 3; i++) //num_file
        {
            printf("find in file %s:\n", remember[i].name_of_file);
            int k = 0;
            for (int k = 0; k < remember[i].count_lines; k++) 
            {
                int line_num = remember[i].num_of_lines[k];
                printf("Str %d:\n", line_num + 1);

                for (int j = 0; j < remember[i].count_answers[k]; j++) 
                {
                    printf("Index %d\n", remember[i].num_of_line[k][j]);
                }
            }
        }
    } 
    else 
    {
        printf("Substring not found in any of the files.\n");
    }
    for (int i = 0; i < 3; i++) // num_file 
    {
        free(remember[i].name_of_file);

        for (int k = 0; k < remember[i].count_lines; k++) 
        {
            free(remember[i].num_of_line[k]);
        }
        free(remember[i].num_of_line);
        free(remember[i].count_answers);
        free(remember[i].num_of_lines);
    }

    free(remember);
    free(substr);
    free(file_path);
}
