#include <stdio.h>
#include <stdarg.h>
#include <math.h>

enum errors_repr
{
    error_base,
    input_correct
};

enum errors_repr has_right_representation(int* has_right,int base, int count, ...) {
    if (base <= 0 )
    {
        return error_base;
    }
    va_list args;
    va_start(args, count);

    (*has_right) = 1;

    for (int i = 0; i < count; i++) {
        double decimal = va_arg(args, double);

        if (decimal <= 0 || decimal >= 1) {
            (*has_right) = 0;
            break;
        }

        double fractional_part = decimal;
        int max_iterations = 20;

        while (fractional_part > 0 && max_iterations > 0) {
            fractional_part *= base;
            if (fractional_part >= 1) {
                fractional_part -= floor(fractional_part);
            }
            max_iterations--;
        }
        if (fractional_part != 0) {
            (*has_right) = 0;
            break;
        }
    }

    va_end(args);

    return input_correct;
}

int main() {
    int base = 5;
    int has_right;
    enum errors_repr check = has_right_representation(&has_right,base, 1, 0.25);
    if (check == error_base)
    {
        printf("Wrong base\n");
    }
    else
    {
        if (has_right) {
            printf("Все дроби имеют конечное представление.\n");
        } 
        else 
        {
            printf("Не все дроби имеют конечное представление.\n");
        }
    }

    return 0;
}
