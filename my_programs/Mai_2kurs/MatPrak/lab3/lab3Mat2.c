#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

typedef struct Vctrs
{
    int** index;
    int current_func;
    int* count_vctrs;
}Indexes;

typedef struct Place
{
    int n;
    double* coord;
}Vector;

void max_norm(Vector* vectors, Indexes** answer_vectors, double eps, int count)
{
    double temp_max = fabs(vectors[0].coord[0]);
    double max = fabs(vectors[0].coord[0]);
    double max_arr[count];
    for(int j = 0; j < count; j++)
    {
        temp_max = fabs(vectors[j].coord[0]);
        for (int k = 1; k < vectors[j].n; k++)
        {
            if ((temp_max - fabs(vectors[j].coord[k])) < eps)
            {
                temp_max = fabs(vectors[j].coord[k]);
            }
        }
        if ((max - temp_max) < eps)
        {
            max = temp_max;
        }
        max_arr[j] = temp_max;
    }
    for (int j = 0; j < count; j++)
    {
        if (fabs(max - max_arr[j]) <= eps)
        {
            (*answer_vectors)->index[(*answer_vectors)->current_func][(*answer_vectors)->count_vctrs[(*answer_vectors)->current_func]] = j;
            (*answer_vectors)->count_vctrs[(*answer_vectors)->current_func]++;
        }
    }
    (*answer_vectors)->current_func++;
    
}

void evklidova_norm(Vector* vectors, Indexes** answer_vectors, double eps, int count,int p)
{
    double max_sum = 0.0;
    double sums[count];
    for (int j = 0; j < count; j++)
    {
        double sum = 0.0;
        for (int k = 0; k < vectors[j].n; k++)
        {
            sum += pow(fabs(vectors[j].coord[k]),p);
        }
        sum = pow(sum,1.0/(double)p);
        if ((max_sum - sum) < eps)
        {
            max_sum = sum;
        }
        sums[j] = sum;
    }
    for (int j = 0; j < count; j++)
    {
        if (fabs(max_sum - sums[j]) <= eps)
        {
            (*answer_vectors)->index[(*answer_vectors)->current_func][(*answer_vectors)->count_vctrs[(*answer_vectors)->current_func]] = j;
            (*answer_vectors)->count_vctrs[(*answer_vectors)->current_func]++;
        }
    }
    (*answer_vectors)->current_func++;
}

void matrix_norm(Vector* vectors, Indexes** answer_vectors, double eps, int count)
{
    double matrix[3][3] =
    {
        {1.0, 2.0, 3.0},
        {3.0, 2.0, 1.0},
        {2.0, 1.0, 3.0},
    };
    double new_elem = 0.0;
    double max_sum = 0.0;
    double sums[count];
    for (int i = 0; i < count; i++)
    {
        double sum_of_coord = 0.0;
        for (int j = 0; j < vectors[i].n; j++)
        {
            new_elem = 0.0;
            for (int k = 0; k < vectors[i].n; k++)
            {
                new_elem += matrix[j][k]*vectors[i].coord[k];
            }
            sum_of_coord += new_elem * vectors[i].coord[j];
        }
        sum_of_coord = sqrt(sum_of_coord);
        if ((max_sum - sum_of_coord) <= eps)
        {
            max_sum = sum_of_coord;
        }
        sums[i] = sum_of_coord;

    }
    for (int i = 0; i < count; i++)
    {
        if (fabs(max_sum - sums[i]) <= eps)
        {
            (*answer_vectors)->index[(*answer_vectors)->current_func][(*answer_vectors)->count_vctrs[(*answer_vectors)->current_func]] = i;
            (*answer_vectors)->count_vctrs[(*answer_vectors)->current_func]++;
        }
    }
    (*answer_vectors)->current_func++;

}

enum error_func
{
    error_no_vcr,
    error_no_norm,
    error_allocation_memory,
    no_error
};

enum error_func func (Indexes* arr, double eps, int n, int count_vcr, int count_norm, ...)
{
    if (count_norm < 1)
    {
        return error_no_norm;
    }
    if (count_vcr < 1)
    {
        return error_no_vcr;
    }
    va_list args;
    va_start(args,count_norm);
    Vector* vectors = (Vector*)malloc(sizeof(Vector) * count_vcr);
    if (vectors == NULL)
    {
        return error_allocation_memory;
    }
    void* func_ptr;
    for (int i = 0; i < count_vcr; i++)
    {
        vectors[i] = va_arg(args,Vector);
    }
    for (int i = 0; i < count_norm; i++)
    {
        func_ptr = va_arg(args,void*);
        if (func_ptr == max_norm)
        {
            ((void (*)(Vector*, Indexes**, double, int))func_ptr)(vectors,&arr,eps,count_vcr);
        }
        else if(func_ptr == evklidova_norm)
        {
            int p = 1;
            ((void (*)(Vector*, Indexes**, double, int, int))func_ptr)(vectors,&arr,eps,count_vcr, p);
        }
        else if(func_ptr == matrix_norm)
        {
            ((void (*)(Vector*, Indexes**, double, int))func_ptr)(vectors,&arr,eps,count_vcr);
        }
    }
    free(vectors);
    return no_error;
}

const char* get_error_message(enum error_func check) {
    switch (check) {
        case error_allocation_memory:
            return "Allocation memory\n";
        case error_no_norm:
            return "No functions are specified for vectors\n";
        case error_no_vcr:
            return "Not a single vector is set\n";
        default:
            return "Unknown error";
    }
}

int main()
{
    int count_vcr = 3;
    int count_norm = 3;
    int n = 3;
    double eps = 0.1;
    void* functions_max = max_norm;
    void* functions_evkild = evklidova_norm;
    void* functions_matrix = matrix_norm;

    Indexes arr;
    arr.current_func = 0;
    arr.index = (int**)malloc(sizeof(int*)*count_norm);
    if (arr.index == NULL)
    {
        return 2;
    }
    arr.count_vctrs = (int*)malloc(sizeof(int)*count_norm);
    if (arr.count_vctrs == NULL)
    {
        free(arr.index);
        return 2;
    }
    for (int i = 0; i < count_norm; i++)
    {
        arr.count_vctrs[i] = 0;
    }
    for (int i = 0; i < count_vcr; i++)
    {
        arr.index[i] = (int*)malloc(sizeof(int)*count_vcr);
        if (arr.index[i] == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                free(arr.index[j]);
            }
            free(arr.count_vctrs);
            free(arr.index);
            return 1;
        }
    }

    Vector vec1;
    vec1.coord = (double*)malloc(sizeof(double)*n);
    if (vec1.coord == NULL)
    {
        for (int i = 0; i < count_vcr; i++) 
        {
            free(arr.index[i]);
        }
        free(arr.index);
        free(arr.count_vctrs);
        return 2;
    }
    vec1.n = n;
    vec1.coord[0] = 2.95;
    vec1.coord[1] = 1.95;
    vec1.coord[2] = 6.95;

    Vector vec2;
    vec2.coord = (double*)malloc(sizeof(double)*n);
    if (vec2.coord == NULL)
    {
        free(vec1.coord);
        for (int i = 0; i < count_vcr; i++) 
        {
            free(arr.index[i]);
        }
        free(arr.index);
        free(arr.count_vctrs);
        return 2;
    }
    vec2.n = n;
    vec2.coord[0] = 7.0;
    vec2.coord[1] = 2.0;
    vec2.coord[2] = 3.0;
    
    Vector vec3;
    vec3.coord = (double*)malloc(sizeof(double)*n);
    if (vec3.coord == NULL)
    {
        free(vec1.coord);
        free(vec2.coord);
        for (int i = 0; i < count_vcr; i++) 
        {
            free(arr.index[i]);
        }
        free(arr.index);
        free(arr.count_vctrs);
        return 2;
    }
    vec3.n = n;
    vec3.coord[0] = -5.0;
    vec3.coord[1] = 2.0;
    vec3.coord[2] = 4.0;


    enum error_func check = func(&arr, eps, n, count_vcr, count_norm, vec1,vec2,vec3, functions_max, functions_evkild, functions_matrix);
    if (check == no_error)
    {
        for (int i = 0; i < arr.current_func; i++)
        {
            printf("In your %d input func: \n", i+1);
            for (int j = 0; j < arr.count_vctrs[i]; j++)
            {
                printf("Vector: %d\n", arr.index[i][j]+1);
            }
        }
    }
    else
    {
        printf("Error: %s\n", get_error_message(check));
    }

    for (int i = 0; i < count_vcr; i++) 
    {
        free(arr.index[i]);
    }
    free(arr.index);
    free(arr.count_vctrs);

    free(vec1.coord);
    free(vec2.coord);
    free(vec3.coord);
}