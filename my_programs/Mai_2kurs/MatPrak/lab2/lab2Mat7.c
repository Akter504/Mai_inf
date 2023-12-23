#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

double f1(double x)
{
    return 4-exp(x)-2*pow(x,2);
}

double f2(double x)
{
    return pow(x,3)-3*x+1;
}

double f3(double x)
{
    return sin(x)-pow(x,2);
}

enum error_dix
{
    error_dixot,
    error_a_more_b,
    input_correct
};

enum error_dix dixotomia(double* res, double eps, double a, double b, double (*f)(double))
{
    if (f(a) * f(b) >= 0) 
    {
        return error_dixot;
    }
    if (a > b)
    {
        return error_a_more_b;
    }
    double c;
    
    while (fabs(b - a) > eps) 
    {
        c = (a + b) / 2.0;
        
        if (fabs(f(c)) < eps) 
        {
            (*res) = c;
        }
        
        if (f(c) * f(a) < 0) 
        {
            b = c;
        } 
        else 
        {
            a = c;
        }
    }
    (*res) = (a+b) / 2.0;
    return input_correct;
}

int main()
{
    double res;

    double eps = 0.01;
    double a = 0.0 ,b = 1.0;
    double (*operation)(double);
    operation = f1;
    enum error_dix check = dixotomia(&res,eps,a,b, operation);
    if (check == input_correct)
    {
        printf("%f\n",res);
    }
    else if (check == error_dixot)
    {
        printf("f(a)*f(b) >= 0\n");
    }
    else if (check == error_a_more_b)
    {
        printf("a>b\n");
    }

    a = 0.0, b = 1.0;
    eps = 0.001;
    operation = f2;
    check = dixotomia(&res, eps,a,b, operation);
    if (check == input_correct)
    {
        printf("%f\n",res);
    }
    else if (check == error_dixot)
    {
        printf("f(a)*f(b) >= 0\n");
    }
    else if (check == error_a_more_b)
    {
        printf("a>b\n");
    }

    a = 0.5, b = 1.0;
    eps = 0.001;
    operation = f3;
    check = dixotomia(&res, eps,a,b, operation);
    if (check == input_correct)
    {
        printf("%f\n",res);
    }
    else if (check == error_dixot)
    {
        printf("f(a)*f(b) >= 0\n");
    }
    else if (check == error_a_more_b)
    {
        printf("a>b\n");
    }
}