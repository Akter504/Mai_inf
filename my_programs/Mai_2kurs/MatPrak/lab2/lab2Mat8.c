#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

int max(int a, int b) {
    return (a > b) ? a : b;
}

int get_digit_value(char c, int base) {
    if (base <= 10) {
        return c - '0';
    } else {
        if (c >= '0' && c <= '9') {
            return c - '0';
        } else if (c >= 'A' && c <= 'Z') {
            return c - 'A' + 10;
        } else if (c >= 'a' && c <= 'z') {
            return c - 'a' + 10;
        } else {
            return 0;
        }
    }
}

char* sum_str(const char* str1, const char* str2, int base) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int maxLen = max(len1, len2);

    char* result = (char*)malloc((maxLen + 2) * sizeof(char));
    if (result == NULL)
    {
        return "Error";
    }
    result[maxLen + 1] = '\0';

    int carry = 0;

    for (int i = 0; i < maxLen; i++) {
        int digit1 = (i < len1) ? (isdigit(str1[len1 - 1 - i]) ? str1[len1 - 1 - i] - '0' : str1[len1 - 1 - i] - 'A' + 10) : 0;
        int digit2 = (i < len2) ? (isdigit(str2[len2 - 1 - i]) ? str2[len2 - 1 - i] - '0' : str2[len2 - 1 - i] - 'A' + 10) : 0;

        int sum = digit1 + digit2 + carry;
        carry = sum / base;
        result[maxLen - i] = (sum % base) + (sum % base < 10 ? '0' : 'A' - 10);
    }

    if (carry) {
        result[0] = '1';
        return result;
    } else {
        return result + 1;
    }
}

int out_base(char *str,int base)
{
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++)
    {
        if (isdigit(str[i]))
        {
            if ((str[i]-'0') > base)
            {
                return 1;
            }
        }
        else if (isalpha(str[i]))
        {
            if ((str[i]-'A') > base)
            {
                return 1;
            }
        }
    }
    return 0;
}

enum errors_func
{
    error_count,
    error_base,
    error_num,
    error_allocation,
    input_correct
};

enum errors_func func_sum(char ** res, int base,size_t count, ... )
{
    va_list args;
    va_start(args,count);
    if (count <= 1) {
        va_end(args);
        return error_count;
    }
    if (base < 2 || base > 36)
    {
        return error_base;
    }
    char* str1;
    char* str2;
    for (size_t i = 0; i < count; i++)
    {
        if (i == 0)
        {
            str1 = va_arg(args,char *);
            if (out_base(str1,base))
            {
                return error_num;
            }
        }
        else if (i >= 1)
        {
            str2 = va_arg(args,char *);
            if (out_base(str2,base))
            {
                return error_num;
            }
            char* temp_str = sum_str(str1,str2,base);
            if (i >=2)
            {
                free(str1);
            }
            if (strcmp(temp_str,"Error") == 0)
            {
                return error_allocation;
            }
            str1 = temp_str;
        }
        
    }
    va_end(args);
    (*res) = str1;
    return input_correct;
}

void removeLeadingZeros(char *str) {
    int length = strlen(str);
    int nonZeroIndex = 0;
    while (nonZeroIndex < length && str[nonZeroIndex] == '0') {
        nonZeroIndex++;
    }
    if (nonZeroIndex == length) {
        str[1] = '\0';
        return;
    }
    for (int i = 0; i < length - nonZeroIndex; i++) {
        str[i] = str[i + nonZeroIndex];
    }
    str[length - nonZeroIndex] = '\0';
}

int main()
{
    char* res = NULL;
    enum errors_func check = func_sum(&res,10,3,"999","1","0");   
    if (check == error_count)
    {
        printf("Wrong count\n");
    } 
    else if (check == error_base)
    {
        printf("Wrong base\n");
    }
    else if (check == error_num)
    {
        printf("Wrong number\n");
    }
    else if (check == error_allocation)
    {
        printf("Allocation memory error\n");
    }
    else if (check == input_correct)
    {
        removeLeadingZeros(res);
        printf("%s\n",res);
    }
    if (res != NULL)
    {
        free(res);
    }
}