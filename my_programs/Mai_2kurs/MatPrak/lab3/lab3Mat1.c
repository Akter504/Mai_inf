#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum errors
{
    error_allocation_memory,
    error_incorrect_num,
    no_error
};

int sum(int a,int b)
{
    int sum_args;
    int carry;
    do {
        sum_args = a ^ b;
        carry = (a & b) << 1;
        a = sum_args;
        b = carry;
    } while (carry != 0);
    return sum_args;
}

enum errors decimal_to_r(int a,char** str , int digit)
{
    int index = 0;
    int remember  = a;
    int count = 0;
    char* binary_string;
    if (digit == 1)
    {
        binary_string = (char*)malloc(33);
        if (binary_string == NULL)
        {
            return error_allocation_memory;
        }
        while (a > 0) 
        {
            int bit = a & 1;
            binary_string[index] = sum(bit,'0');
            index = sum(index, 1);
            a >>= 1;
        }
    }
    else if (digit == 2)
    {
        while (remember > 0)
        {
            count = sum(count,1);
            remember >>= 2;
        }
        binary_string = (char*)malloc(sum(count,1));
        while (a > 0) 
        {
            int bit = a & 3;
            binary_string[index] = sum(bit,'0');
            index = sum(index, 1);
            a >>= 2;
        }
    }
    else if (digit == 3)
    {
        while (remember > 0)
        {
            count = sum(count,1);
            remember >>= 3;
        }
        binary_string = (char*)malloc(sum(count,1));
        if (binary_string == NULL)
        {
            return error_allocation_memory;
        }
        while (a > 0) {
            int bit = a & 7;
            binary_string[index] = sum(bit,'0');
            index = sum(index, 1);
            a >>= 3;
        }
    }
    else if (digit == 4)
    {
        while (remember > 0)
        {
            count = sum(count,1);
            remember >>= 4;
        }
        binary_string = (char*)malloc(sum(count,1));
        if (binary_string == NULL)
        {
            return error_allocation_memory;
        }
        while (a > 0) {
            int bit = a & 15;
            if (bit >= 10)
            {
                bit = sum(bit,-10);
                binary_string[index] = sum(bit,'A');
                index = sum(index, 1);
                a >>= 4;
            }
            else if (bit >= 0 && bit < 10)
            {
                binary_string[index] = sum(bit,'0');
                index = sum(index, 1);
                a >>= 4;
            }
        }
    }
    else if (digit == 5)
    {
        while (remember > 0)
        {
            count = sum(count,1);
            remember >>= 5;
        }
        binary_string = (char*)malloc(sum(count,1));
        if (binary_string == NULL)
        {
            return error_allocation_memory;
        }
        while (a > 0) {
            int bit = a & 31;
            if (bit >= 10)
            {
                bit = sum(bit,-10);
                binary_string[index] = sum(bit,'A');
                index = sum(index, 1);
                a >>= 5;
            }
            else if (bit >= 0 && bit < 10)
            {
                binary_string[index] = sum(bit,'0');
                index = sum(index, 1);
                a >>= 5;
            }
        }
    }
    else
    {
        free(binary_string);
        return error_incorrect_num;
    }
    binary_string[index] = '\0';
    int temp = 0;
    char temp_char;
    while (index > temp)
    {
        temp_char = binary_string[temp];
        binary_string[temp] = binary_string[index-1];
        binary_string[index-1] = temp_char;
        temp = sum(temp,1);
        index = sum(index,-1);
    }
    (*str) = binary_string;
    return no_error;
}

char const* get_error_message(enum errors check)
{
    switch (check)
    {
    case error_allocation_memory:
        return "Allocation memory\n";
    case error_incorrect_num:
        return "Digit <1 or >5\n";
    default:
        return "Another error\n";
    }
} 

int main()
{
    printf("Input your num\n");
    char num_str[256];
    scanf("%256s", num_str);
    char *end;
    int num = strtol(num_str,&end,10);
    if (*end != '\0')
    {
        printf("Incorrect number!\n");
        return 1;
    }
    printf("Input your digit(1,2,3,4,5)\n");
    char digit_str[256];
    scanf("%256s", digit_str);
    int digit = strtol(digit_str,&end,10);
    if (*end != '\0')
    {
        printf("Incorrect digit!\n");
        return 1;
    }
    char *str;
    enum errors check = decimal_to_r(num, &str, digit);
    if (check != no_error)
    {
        printf("Error: %s\n", get_error_message(check));
        return 1;
    }
    printf("%s\n",str);
    free(str);
}