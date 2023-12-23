#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct MemoryCell
{
    int value;
    char* name;
}MemoryCell;

int find_variable(MemoryCell* variables, const char* variable, int count_variables)
{
    for (int i = 0; i < count_variables; i++)
    {
        if (strcmp(variables[i].name,variable) == 0)
        {
            return i;
        }
    }
    return -1;
}

void file_processing(FILE* input)
{
    char c;
    char print[6];
    print[5] = '\0';
    char* buffer = NULL;
    size_t buffer_size = 0;
    MemoryCell* variables = (MemoryCell*)malloc(sizeof(MemoryCell)*2+1);
    if (variables == NULL)
    {
        return;
    }
    int count_variables = 0;
    while (getline(&buffer, &buffer_size, input) != -1) {
        size_t length = strlen(buffer);
        char* ptr_equal = strchr(buffer,'=');
        char* ptr_addition = strchr(buffer,'+');
        char* ptr_difference = strchr(buffer,'-');
        char* ptr_multiplication = strchr(buffer,'*');
        char* ptr_division = strchr(buffer,'/');
        char* ptr_end = strchr(buffer,';');
        char* current_ptr = NULL;
        if (ptr_addition != NULL)
        {
            current_ptr = ptr_addition;
        }
        else if (ptr_difference != NULL)
        {
            current_ptr = ptr_difference;
        }
        else if(ptr_division != NULL)
        {
            current_ptr = ptr_division;
        }
        else if (ptr_multiplication != NULL)
        {
            current_ptr = ptr_multiplication;
        }

        if (ptr_end == NULL)
        {
            return;
        }
        if (ptr_equal != NULL)
        {
            variables->name = (char*)malloc(sizeof(char)*(ptr_equal-buffer+1));
            if (variables->name == NULL)
            {
                return;
            }
            for (int i = 0; i < ptr_equal-buffer; i++)
            {
                if (i < ptr_equal-buffer)
                {
                    (variables[count_variables]).name[i] = buffer[i];
                }
            }
            variables[count_variables].name[ptr_equal-buffer] = '\0';
            if (current_ptr == NULL)
            {
                if (isdigit(buffer[ptr_equal-buffer+1]))
                {
                    char* start = buffer;
                    start = start + (ptr_equal-buffer+1);
                    variables[count_variables].value = strtol(start,NULL,10);
                    count_variables++;
                }
                else
                {
                    char name_of_variable[(ptr_end-buffer)-(current_ptr-buffer)+1];
                    int j = 0;
                    for (int i = current_ptr-buffer+1; i < (ptr_end-buffer); i++)
                    {
                        name_of_variable[j] = buffer[i];
                        j++;
                    }
                    name_of_variable[j+1] = '\0';
                    if (count_variables == 0)
                    {
                        return; // error_no_varialbe
                    }
                    else
                    {
                        int i = find_variable(variables, name_of_variable, count_variables);
                        if (i != -1)
                        {
                            variables[count_variables].value = variables[i].value;
                            count_variables++;
                        }
                    }
                }
                continue;
            }
            if (isdigit(buffer[ptr_equal-buffer+1]))
            {
                char* start = buffer;
                start = start + (ptr_equal-buffer+1);
                char* end;
                int num1 = strtol(start, &end,10);
                if (isdigit(buffer[current_ptr-buffer+1]))
                {
                    char* start = buffer;
                    start = start+((*end)+1);
                    int num2 = strtol(start,NULL,10);
                    if ((*end) == '+')
                    {
                        variables[count_variables].value = num1+num2;
                    }
                    else if ((*end) == '-')
                    {
                        variables[count_variables].value = num1-num2;
                    }
                    else if ((*end) == '*')
                    {
                        variables[count_variables].value = num1*num2;
                    }
                    else if ((*end) == '/')
                    {
                        variables[count_variables].value = num1/num2;
                    }
                    count_variables++;
                }
                else
                {
                    char name_of_variable[(ptr_end-buffer)-(current_ptr-buffer)+1];
                    int j = 0;
                    for (int i = current_ptr-buffer+1; i < (ptr_end-buffer); i++)
                    {
                        name_of_variable[j] = buffer[i];
                        j++;
                    }
                    name_of_variable[j+1] = '\0';
                    if (count_variables == 0)
                    {
                        return; // error_no_varialbe
                    }
                    else
                    {
                        int i = find_variable(variables, name_of_variable, count_variables);
                        if (i != -1)
                        {
                            if ((*end) == '+')
                            {
                                variables[count_variables].value = num1+variables[i].value;
                            }
                            else if ((*end) == '-')
                            {
                                variables[count_variables].value = num1-variables[i].value;
                            }
                            else if ((*end) == '*')
                            {
                                variables[count_variables].value = num1*variables[i].value;
                            }
                            else if ((*end) == '/')
                            {
                                variables[count_variables].value = num1/variables[i].value;
                            }
                            count_variables++;
                        }
                        else
                        {
                            return; // error_no_varialbe
                        }
                    }
                }
            }
            else
            {
                char name_of_variable1[(current_ptr-buffer)-(ptr_equal-buffer)];
                int j = 0;
                for (int i = ptr_equal-buffer+1; i < (current_ptr-buffer); i++)
                {
                    name_of_variable1[j] = buffer[i];
                    j++;
                }
                name_of_variable1[j+1] = '\0';
                if (count_variables == 0)
                {
                    return; // error_no_varialbe
                }
                else
                {
                    int i = find_variable(variables, name_of_variable1, count_variables);
                    if (i != -1)
                    {
                        if (isdigit(buffer[current_ptr-buffer+1]))
                        {
                            char* start = buffer;
                            start = start + (current_ptr-buffer+1);
                            int num2 = strtol(start,NULL,10);
                            if (buffer[current_ptr-buffer] == '+')
                            {
                                variables[count_variables].value = num2+variables[i].value;
                            }
                            else if (buffer[current_ptr-buffer] == '-')
                            {
                                variables[count_variables].value = variables[i].value - num2;
                            }
                            else if (buffer[current_ptr-buffer] == '*')
                            {
                                variables[count_variables].value = num2*variables[i].value;
                            }
                            else if (buffer[current_ptr-buffer] == '/')
                            {
                                variables[count_variables].value = variables[i].value / num2;
                            }
                            count_variables++;
                        }
                        else
                        {
                            char name_of_variable2[(ptr_end-buffer)-(current_ptr-buffer)+1];
                            int j = 0;
                            for (int i = current_ptr-buffer+1; i < (ptr_end-buffer); i++)
                            {
                                name_of_variable2[j] = buffer[i];
                                j++;
                            }
                            name_of_variable2[j+1] = '\0';
                            j = find_variable(variables, name_of_variable1, count_variables);
                            if (j != -1)
                            {
                                if (buffer[current_ptr-buffer] == '+')
                                {
                                    variables[count_variables].value = variables[j].value+variables[i].value;
                                }
                                else if (buffer[current_ptr-buffer] == '-')
                                {
                                    variables[count_variables].value = variables[i].value - variables[j].value;
                                }
                                else if (buffer[current_ptr-buffer] == '*')
                                {
                                    variables[count_variables].value = variables[j].value*variables[i].value;
                                }
                                else if (buffer[current_ptr-buffer] == '/')
                                {
                                    variables[count_variables].value = variables[i].value / variables[j].value;
                                }
                                count_variables++;
                            }
                            else
                            {
                                return;
                            }
                        }
                    }
                    else
                    {
                        return; // error_no_varialbe
                    }
                }

            }
        }
    }

    free(buffer);
}

int main(int agrc, char* argv[])
{
    FILE* input = fopen(argv[1],"r");
    if (input == NULL)
    {
        printf("Error open file\n");
        return 1;
    }
    file_processing(input);

}