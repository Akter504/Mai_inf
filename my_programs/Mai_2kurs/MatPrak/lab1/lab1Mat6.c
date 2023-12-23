#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

bool check_double(const char *str) {
    double num;
    int result = sscanf(str, "%lf", &num);
    return result == 1 && strchr(str, '.') != NULL;
}

enum errors {
    error_empty_or_incorrect_input,
    error_e_in_not_double,
    input_correct
};

enum errors check(int len, char* argv[]) {
    if (len == 2){
       if (check_double(argv[1])){
          return input_correct;
       }
       else{
          return error_e_in_not_double;
       }
   }
   else{
       return error_empty_or_incorrect_input;
   }
}

double counting_first_func(double x, int step, double h){
   double remember = x;
   double I = 0.0;
   for (int i = 1; i <= step - 1; i++) {
        x = x + h;
        I += log(x + 1) / x;
    }
    I += ((log(x + h + 1) / (x + h)) + (log(remember + 1) / remember)) / 2;
    I = I * h;
    return I;
}

double first_func(double x, double e) {
    x = 0.01;
    double n = 5.0;
    int step = 5;
    double h = 1.0 / n;
    double remember = x;
    double I1 = 0.0, I2 = 0.0, I3 = 0.0;
    I1 = counting_first_func(x, step, h);

    x = remember;
    step *= 2;
    n *= 2;
    h = 1.0 / n;
    I2 = counting_first_func(x, step, h);

    x = remember;
    step *= 2;
    n *= 2;
    h = 1.0 / n;
    while (fabs(I1 - I2) >= e) {
        I3 = counting_first_func(x, step, h);
        I1 = I2;
        I2 = I3;
        x = remember;
        step *= 2;
        n *= 2;
        h = 1.0 / n;
    }
    return I2;
}

double counting_second_func(double x, int step, double h){
   double remember = x;
   double I = 0.0;
   for (int i = 1; i <= step - 1; i++) {
        x = x + h;
        I += exp(-pow(x, 2) / 2);
    }
    I += ((exp(-pow(x + h, 2) / 2)) + (exp(-pow(remember, 2) / 2))) / 2;
    I = I * h;
    return I;
}

double second_func(double x, double e) {
    double n = 5.0;
    int step = 5;
    double h = 1.0 / n;
    double remember = x;
    double I1 = 0.0, I2 = 0.0, I3 = 0.0;
    I1 = counting_second_func(x, step, h);

    x = remember;
    step *= 2;
    n *= 2;
    h = 1.0 / n;
    I2 = counting_second_func(x, step, h);

    x = remember;
    step *= 2;
    n *= 2;
    h = 1.0 / n;
    while (fabs(I1 - I2) >= e) {
        I3 = counting_second_func(x, step, h);
        I1 = I2;
        I2 = I3;
        x = remember;
        step *= 2;
        n *= 2;
        h = 1.0 / n;
    }
    return I2;
}

double counting_third_func(double x, int step, double h){
   double remember = x;
   double I = 0.0;
   for (int i = 1; i <= step - 1; i++) {
        x = x + h;
        I += log(1 / (1 - x));
    }
    I += ((log(1 / (1 - x + h))) + (log(1 / (1 - remember)))) / 2;
    I = I * h;
    return I;
}

double third_func(double x, double e) {
    double n = 5.0;
    int step = 5;
    double h = 1.0 / n;
    double remember = x;
    double I1 = 0.0, I2 = 0.0, I3 = 0.0;
    I1 = counting_third_func(x, step, h);

    x = remember;
    step *= 2;
    n *= 2;
    h = 1.0 / n;
    I2 = counting_third_func(x, step, h);

    x = remember;
    step *= 2;
    n *= 2;
    h = 1.0 / n;
    while (fabs(I1 - I2) >= e) {
        I3 = counting_third_func(x, step, h);
        I1 = I2;
        I2 = I3;
        x = remember;
        step *= 2;
        n *= 2;
        h = 1.0 / n;
    }
    return I2;
}

double counting_fourty_func(double x, int step, double h){
   double remember = x;
   double I = 0.0;
   for (int i = 1; i <= step - 1; i++) {
        x = x + h;
        I += pow(x, x);
    }
    I += ((pow(x + h, x + h)) + (pow(remember, remember))) / 2;
    I = I * h;
    return I;
}


double fourty_func(double x, double e) {
    double n = 5.0;
    int step = 5;
    double h = 1.0 / n;
    double remember = x;
    double I1 = 0.0, I2 = 0.0, I3 = 0.0;
    I1 = counting_fourty_func(x, step, h);

    x = remember;
    step *= 2;
    n *= 2;
    h = 1.0 / n;
    I2 = counting_fourty_func(x, step, h);

    x = remember;
    step *= 2;
    n *= 2;
    h = 1.0 / n;
    while (fabs(I1 - I2) >= e) {
        I3 = counting_fourty_func(x, step, h);
        I1 = I2;
        I2 = I3;
        x = remember;
        step *= 2;
        n *= 2;
        h = 1.0 / n;
    }
    return I2;
}

      

int main(size_t argc, char* argv[]) {
    switch(check(argc,argv)){
       case error_empty_or_incorrect_input:
          printf("Input is empty or incorrect.\n");
          break;
       case error_e_in_not_double:
          printf("Input 'e' in not double.\n");
          break;
       case input_correct:
          double x = 0.0;
          double e = strtod(argv[1],NULL);
          if (e < 1e-7){
             printf("Enter eps greater than 1e-7\n");
             return 1;
          }
          double first_sum = first_func(x,e);
          printf("Integral of log(1 + x) / x,\ta = 0,\tb = 1:\t\n%.9lf\n", first_sum);
          double second_sum = second_func(x,e);
          printf("Integral of e^(-(x * x) / 2),\ta = 0\tb = 1:\t\n%.9lf\n" ,second_sum);
          double third_sum = third_func(x,e);
          printf("Integral of log(1 / 1 - x),\ta = 0\tb = 1:\t\n%.9lf\n", third_sum);
          double fourty_sum = fourty_func(x,e);
          printf("Integral of x^x,\ta = 0\tb = 1:\t\n%.9lf\n", fourty_sum);
          break;
    }
          
       
}
