#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>


typedef struct Point
{
    double x;
    double y;
}Points;

double check_convex(Points a, Points b, Points c) 
{
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

int is_convex_polygon(size_t count_coord, ...)
{
    va_list args;
    va_start(args,count_coord);
    if (count_coord < 3)
    {
        va_end(args);
        return 0;
    }
    Points* points = (Points*)malloc(count_coord * sizeof(Points));
    for (int i = 0; i < count_coord; i++) 
    {
        points[i] = va_arg(args, Points);
    }

    for (int i = 0; i < count_coord-2; i++)
    {
        double check = check_convex(points[i],points[i+1],points[i+2]);
        if (check < 0)
        {
            return 0;
        }
    }

    va_end(args);
    free(points);
    return 1;
}

enum errors_poly
{
    error_degree,
    input_correct
};

enum errors_poly polynomial_value(double* res,double x, int degree, ...)
{
    if (degree < 0)
    {
        return error_degree;
    }
    va_list args;
    va_start(args,degree);
    double elem = 0.0;
    for (size_t i = 0; i <= degree; i++)
    {
        elem = va_arg(args,double);
        (*res) += elem*pow(x,degree-i); 
    }
    va_end(args);
    return input_correct;
}

int main()
{
    /*if (is_convex_polygon(4, (Points){0.0, 0.0}, (Points){1.0, 0.0}, (Points){1.0, 1.0}, (Points){0.0, 1.0}))
    {
        printf("Convex is polygon\n");
    }
    else
    {
        printf("Convex is not polygon\n");
    }*/
    double x = 1.0;
    int degree = -1;
    double res = 0.0;
    enum errors_poly check = polynomial_value(&res,x,degree,1.0,1.0,1.0);
    if (check == error_degree)
    {
        printf("Wrong degree\n");
    }
    else if (check == input_correct)
    {
        printf("%f\n", res);
    }

}