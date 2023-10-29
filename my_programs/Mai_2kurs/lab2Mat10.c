#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

void derivative(int* n, double** values) {
    if ((*n) == 0)
        return;
        
    for (int i = 1; i <= *n; ++i) {
        (*values)[i-1] = (*values)[i]*i;
    }
    
    (*n) = (*n)-1;
}

enum errors_func
{
    error_allocation,
    error_degree,
    input_correct
};

enum errors_func func(double x, double epsilon, double **coefficients, int degree, ...) 
{
    *coefficients = (double *)malloc((degree + 1) * sizeof(double));
    double* values = (double *)malloc((degree + 1) * sizeof(double));
    if (coefficients == NULL || values == NULL)
    {
        return error_allocation;
    }
    if (degree < 0)
    {
        
        return error_degree;
    }
    va_list args;
    va_start(args,degree);
    for (int i = 0; i <= degree; i++)
    {
        double temp = va_arg(args,double);
        values[i] = temp;
        (*coefficients)[i] = 0.0;
    }
    va_end(args);
    int k = 0;
    int fact = 1;
    int flag_exit = 1;
    for (int i = 0; i <= degree && flag_exit; i++)
    {
        if (i != 0)
        {
            int temp_degree = degree-k;
            derivative(&temp_degree,&values);
            k++;
        }
        for (int j = 0; j <= degree-i; j++)
        {
            if (i == 0)
            {
                (*coefficients)[i] += values[j]*pow(x,j);
            }
            else
            {
                (*coefficients)[i] += values[j]*pow(x,j);
                if (j == degree-i)
                {
                    fact = fact*i;
                    (*coefficients)[i] /= (fact);
                    if (fabs((*coefficients)[i-1]-(*coefficients)[i]) < epsilon)
                    {
                        flag_exit = 0;
                    }
                }
            }
        }
    }
    free(values);
    return input_correct;
}

int main()
{
    int degree = 2;
    double x = 2.0 ,e = 0.001;
    double *coefficients;
    enum errors_func check = func(x,e,&coefficients,degree,1.0,2.0,3.0);
    if (check == error_allocation)
    {
        printf("Allocation memory error\n");
    }
    else if (check == error_degree)
    {
        printf("Error: wrong degree\n");
    }
    else if (check == input_correct)
    {
        for (int i = 0; i <= degree; i++)
        {
            printf("Num %d: %f\n",i,coefficients[i]);
        }
        free(coefficients);
    }
}

