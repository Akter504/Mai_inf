#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>

size_t atoi_f(const char* arg) {
    size_t num = 0;
    int i = 0;
    while (arg[i] && (arg[i] >= '0' && arg[i] <= '9')) {
        num = num * 10 + (arg[i] - '0');
        i++;
    }
    return num;
}

int strcmp_f(const char* str1, const char* str2) {
    while (*str1) {
        if (*str1 != *str2) {
            break;
        }
        str1++;
        str2++;
    }
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

size_t strlen_f(const char* str)
{
    size_t i;
    for (i = 0; str[i] != '\0'; i++);
    return i;
}

int check_unsigned_int(const char *str)
{
    size_t len = strlen_f(str);
    for (size_t i = 0; i < len; i++)
    {
        if ((str[i] < '0') || (str[i] > '9'))
        {
            return 0;
        }
    }
    return 1;
}

enum errors {
    error_incorrect_flag,
    error_empty_or_incorrect_input,
    error_not_uns_int,
    input_correct
};

enum errors check(int len, char* argv[]) {
    char correct_flags[][5] = { "-l", "-r", "-u", "-n", "-c" };
    if (len < 2)
    {
       return error_empty_or_incorrect_input;
    }
    for (int i = 0; i < 5; i++) {
        if (argv[1][1] == 'l' || argv[1][1] == 'r' || argv[1][1] == 'u' || argv[1][1] == 'n') 
        {
            return input_correct;
        }
        else if(argv[1][1] == 'c' && len > 2){
           if (check_unsigned_int(argv[2]))
           {
              return input_correct;
           }
           else
           {
              return error_not_uns_int;
           }
        
        }
        else{
           return error_incorrect_flag;
        }
    }
}

void r_func(char* str, size_t len)
{
    size_t rev = len-2;
    char temp;
    for (size_t i = 0; i <= (len-2)/2; i++)
    {
        temp = str[i];
        str[i] = str[rev];
        str[rev] = temp;
        rev--;
    }
}

void u_func(char* str,size_t len)
{

    for (size_t i = 0; i < len-1; i++)
    {
        if (i % 2 != 0)
        {
            if (str[i] >= 'a' && str[i] <= 'z')
            {
                str[i] = str[i] - 'a'+'A';
            }
        }
    }
}

void strcpy_f(char *dest, const char *src, size_t max_len) {
    if (dest == NULL || src == NULL || max_len == 0) {
        return;
    }

    size_t i;
    for (i = 0; i < max_len - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}

enum func {
    error_allocation_memory,
    correct
};

enum func n_func(char** str, size_t len)
{
    char *digits = (char*)malloc(sizeof(char)*len);
    if (digits == NULL)
    {
        return error_allocation_memory;
    }
    char *alphas = (char*)malloc(sizeof(char)*len);
    if (alphas == NULL)
    {
        free(digits);
        return error_allocation_memory;
    }
    char *other = (char*)malloc(sizeof(char)*len);
    if (other == NULL)
    {
        free(digits);
        free(alphas);
        return error_allocation_memory;
    }
    char *res = (char*)malloc(sizeof(char)*len*2);
    if (res == NULL)
    {
        free(digits);
        free(alphas);
        free(other);
        return error_allocation_memory;
    }
    size_t digitsIndex = 0;
    size_t alphasIndex = 0;
    size_t otherIndex = 0;

    for (size_t i = 0; i < len; i++)
    {
        if (isdigit((*str)[i]))
        {
            digits[digitsIndex++] = ((*str)[i]);
        }
        else if (isalpha((*str)[i]))
        {
            alphas[alphasIndex++] = ((*str)[i]);
        }
        else
        {
            other[otherIndex++] = ((*str)[i]);
        }
    }

    digits[digitsIndex] = '\0';
    alphas[alphasIndex] = '\0';
    other[otherIndex] = '\0';

    strcpy_f(res, digits, 2*len);
    strcpy_f(res + digitsIndex, alphas,2*len);
    strcpy_f(res + digitsIndex + alphasIndex, other,2*len);

    free(digits);
    free(alphas);
    free(other);
    (*str) = res;
    return correct;
}

enum func c_func(char** strings, unsigned int seed, int count_strings, char* result) 
{
    srand(seed);
    result[0] = '\0';
    int* used_indices = (int*)malloc(count_strings * sizeof(int));
    if (used_indices)
    {
        return error_allocation_memory;
    }

    for (int i = 0; i < count_strings; i++) 
    {
        used_indices[i] = 0;
    }
    for (int i = 0; i < count_strings; i++) 
    {
        int random_index;
        do 
        {
            random_index = rand() % count_strings;
        } while (used_indices[random_index]);
        
        used_indices[random_index] = 1;
        char* random_string = strings[random_index];
        strcpy_f(result + strlen_f(result), random_string, 258);
    }
    free(used_indices);
    return correct;
}

int main(int argc, char* argv[]) 
{
    char correct_flags[][5] = { "-l", "-r", "-u", "-n", "-c" };
    char* input;
    switch (check(argc, argv)) {
        case error_incorrect_flag:
            printf("The flag enter incorrectly.\n");
            break;
        case error_not_uns_int:
            printf("The number type is not unsigned int.\n");
            break;
        case error_empty_or_incorrect_input:
            printf("Input is empty or incorrect.\n");
            break;
        case input_correct:
            input = (char*)malloc(sizeof(char)*257);
            if (input == NULL)
            {
                printf("Error allocation memory\n");
                return 2;
            }
            if (argv[1][1] == 'l') {
                printf("Input your input: ");
                fgets(input, 258, stdin);
                size_t len = strlen_f(input);
                if (len == 257)
                {
                    printf("Error: Max len of input 256\n");
                    return 1;
                }
                else
                {
                    printf("Input input: %s, Len: %lu\n",input,len);
                }

            }
            else if(argv[1][1] == 'r')
            {
                printf("Input your input: ");
                fgets(input, 258, stdin);
                size_t len = strlen_f(input);
                if (len == 257)
                {
                    printf("Error: Max len of input 256\n");
                    return 1;
                }
                else
                {
                    r_func(input,len);
                    printf("Reversed:\n%s\n", input);
                }
            }
            else if(argv[1][1] == 'u')
            {
                printf("Input your input: ");
                fgets(input, 258, stdin);
                size_t len = strlen_f(input);
                if (len == 257)
                {
                    printf("Error: Max len of input 256\n");
                    return 1;
                }
                else
                {
                    u_func(input,len);
                    printf("After use func:\n%s\n", input);
                }
            }
            else if(argv[1][1] == 'n')
            {
                printf("Input your input: ");
                fgets(input, 258, stdin);
                size_t len = strlen_f(input);
                if (len == 257)
                {
                    printf("Error: Max len of input 256\n");
                    return 1;
                }
                else
                {
                    if (n_func(&input,len) == error_allocation_memory)
                    {
                        printf("Error: allocation memory\n");
                        return 2;
                    }
                    else if(n_func(&input,len) == correct)
                    {
                        printf("After use func:\n%s\n", input);
                    }
                }
            }
            if (argv[1][1] == 'c') {
                int capacity = 10;
                char **strings = (char **)malloc(capacity * sizeof(char *));
                int num_strings = 0;
                if (strings == NULL) {
                    printf("Error: allocation memory.\n");
                    return 2;
                }
                while (1) {
                    printf("Enter a line (or type 'exit' to complete): ");
                    fgets(input, 258, stdin);
                    size_t len = strlen_f(input);
                    if (len == 257)
                    {
                        printf("Error: Max len of input 256\n");
                        for (int i = 0; i < num_strings; i++) {
                            free(strings[i]);
                        }
                        free(strings);
                        return 1;
                    }
                    if (strcmp_f(input, "exit\n") == 0) {
                        break;
                    }
                    if (num_strings >= capacity) {
                        capacity *= 2;
                        strings = (char **)realloc(strings, capacity * sizeof(char *));
                        if (strings == NULL) {
                            printf("Error: allocation memory.\n");
                            return 1;
                        }
                    }

                    strings[num_strings] = (char *)malloc(strlen_f(input) + 1);
                    if (strings[num_strings] == NULL)
                    {
                        printf("Error alocation memory\n");
                        return 2;
                    }
                    strcpy_f(strings[num_strings], input,strlen_f(input));
                    num_strings++;
                }
                char* result = (char*)malloc((sizeof(char) * 257 + 1) * num_strings);
                if (result == NULL)
                {
                    return 2;
                }
                enum func check_c = c_func(strings,atoi_f(argv[2]),num_strings,result);
                if (check_c = error_allocation_memory)
                {
                    printf("Error allocation memory\n");
                }
                else
                {
                    printf("Result string:\n%s\n", result);
                    free(result);
                }

                for (int i = 0; i < num_strings; i++) {
                    free(strings[i]);
                }
                free(strings);
                break;
        }
    }
}