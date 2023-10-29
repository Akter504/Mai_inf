#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

enum errors_geo
{
    error_empty,
    error_NullNum,
    error_negativeNum,
    input_correct
};

enum errors_geo geometric_mean(double* result, int count, ...) 
{
    va_list args;
    va_start(args, count);
    if (count <= 0) {
        va_end(args);
        return error_empty;
    }

    double product = 1.0;

    for (int i = 0; i < count; i++) 
    {
        double num = va_arg(args, double);
        if (num == 0.0) 
        {
            return error_NullNum;
        }
        else if (num < 0.0)
        {
            return error_negativeNum;
        }
        product *= num;
    }
    va_end(args);
    (*result) = pow(product, 1.0 / count);
    return input_correct;
}

enum error_dec
{
    error_negative,
    error_allocation,
    bynary_correct
};

enum error_dec decimal_to_2(int num,size_t* count, size_t** arr) 
{
    if (num < 0)
    {
        return error_negative;
    }
    size_t i = 0;
    size_t binary[32];
    while (num > 0) {
        binary[i] = num % 2;
        num /= 2;
        i++;
    }
    (*count) = i;
    (*arr) = (size_t*)malloc(i * sizeof(size_t));
    if ((*arr) == NULL)
    {
        return error_allocation;
    }
    for (size_t j = 0; j < i; j++) {
        (*arr)[j] = binary[j]; // заполняем переданный массив двоичным представлением степени в обратом порядке "12->0011"
    }
    return bynary_correct;
}

void rapid_exponentiation(double num, size_t* degree, size_t count, double* res)
{
    if (count == 1)
    {
        (*res) *= pow(num,degree[count-1]);
    }
    else if(count == 0)
    {
        (*res) = 1.0;
    }
    else
    {
        (*res) *= pow(num,degree[count-1]);
        (*res) = pow((*res),2);
        rapid_exponentiation(num,degree,count-1,res);
    }
}

int main()
{
    //double result = 0.0; 
    //enum errors_geo check = geometric_mean(&result,3, 3.0, 3.0, 3.0);
    //enum errors_geo check = geometric_mean(4, 1.0, 2.0, -3.0, 4.0);
    //enum errors_geo check = geometric_mean(0);
    //enum errors_geo check = geometric_mean(5, 2.0, -1.0, -4.0, -8.0, -16.0);
    //enum errors_geo check = geometric_mean(5, 2.0, 1.0, 4.0, 8.0, 16.0);
    /*switch(check)
    {
        case error_empty:
            printf("Count is 0\n");
            break;
        case error_negativeNum:
            printf("One of the input numbers was negative\n");
            break;
        case error_NullNum:
            printf("One of the input numbers was Null\n");
            break;
        case input_correct:
            printf("Geometric mean: %lf", result);
            break;
    }*/
    size_t count = 0;
    size_t* arr = NULL;
    double res = 1.0;
    char digit_str[50];
    printf("Enter a positive integer digit: ");
    if (scanf("%s", digit_str) != 1)
     {
        printf("Error: it is impossible to count the digit number.\n");
        return 1;
    }
    char* endPtr;
    int digit = strtoul(digit_str, &endPtr, 10);
    if (*endPtr != '\0' || digit < 0)
     {
        printf("Error: digit must be a positive integer.\n");
        return 1;
    }
    printf("Enter the number num: ");
    if (scanf("%s", digit_str) != 1)
     {
        printf("Error: it is impossible to count the number num.\n");
        return 1;
    }
    double num = strtod(digit_str, &endPtr);
    if (*endPtr != '\0') 
    {
        printf("Error: incorrect format of the number num.\n");
        return 1;
    }
    enum error_dec bynary_check = decimal_to_2(digit,&count,&arr);
    if (bynary_check == error_negative)
    {
        printf("Digit cant be negative\n");
    }
    else if (bynary_check == error_allocation)
    {
        printf("Allocation memory error\n");
    }
    else
    {
        rapid_exponentiation(num, arr, count, &res);
        printf("Result: %f", res);
    }
}