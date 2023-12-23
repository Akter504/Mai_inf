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

double fact(double n) {
    double result = 1.0;
    while (n > 1) {
        result *= n;
        n -=1;
    }
    return result;
}

double fact2(double n) {
    double result = 1.0;
    while (n > 1) {
        result *= n;
        n -=2;
    }
    return result;
}

double first_func(double x, double e){
   double res = 0.0;
   double n = 0.0;
   double elem = pow(x,n) / fact(n);
   do{
      n +=1;
      elem =elem* (x/n);
      res += elem;
   } while (fabs(elem) >= e);
   return res;
}

double second_func(double x, double e){
   double res = 0.0;
   double n = 0.0;
   double elem = (pow(-1.0,n) * pow(x,2*n)) / (fact(2*n));
   do{
      n +=1;
      elem = elem * -(x / (2*n*(2*n-1)));
      res += elem;
   } while (fabs(elem) >= e);
   return res;
}

double third_func(double x, double e){
   double res = 0.0;
   double n = 0.0;
   double elem = (pow(3.0,n) * pow(fact(n),3.0) * pow(x, n)) / (fact(n));
   do{
      n +=1;
      elem = elem * ((9 * pow(n,2) * pow(x,2)) / ((3*n-2)*(3*n-1)));
      res += elem;
   } while (fabs(elem) >= e);
   return res;
}

double fourty_func(double x, double e){
   double res = 0.0;
   double n = 1.0;
   double elem = (pow(-1.0,n) * fact2(2*n-1) * pow(x, 2*n)) / (fact2(2*n));
   do{
      n +=1;
      elem = elem * -(((2*n-1)* pow(x,2)) / (2*n));
      res += elem;
   } while (fabs(elem) >= e);
   return res;
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
          double x = 0.990;
          double e = strtod(argv[1],NULL);
          double first_sum = first_func(x,e);
          printf("%s %f\n", "a_sum: ", first_sum);
          double second_sum = second_func(x,e);
          printf("%s %f\n", "b_sum: ", second_sum);
          if (x < 1.0){
             double third_sum = third_func(x,e);
             printf("%s %f\n", "c_sum: ", third_sum);
             double fourty_sum = fourty_func(x,e);
             printf("%s %f\n", "d_sum: ", fourty_sum);
          }
          else{
             printf("%s\n" , "It is impossible to calculate the amount c");
             printf("%s\n" , "It is impossible to calculate the amount d");
          }
          break;
    }
          
       
}
