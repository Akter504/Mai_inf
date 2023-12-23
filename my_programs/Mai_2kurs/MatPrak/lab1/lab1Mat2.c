#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define M_PI 3.14159265358979323846

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
    double res = 1.0;
    for (long long i = 1; i <= n; i++)
    {
        res *= (double)i;
    }
    return res;
}

double lim_exp(double e) {
   double n = 1.0;
   double prev_arg = 100.0;
   double current_arg = pow(1 + 1/n, n);
   while (fabs(prev_arg - current_arg) >= e){
      prev_arg = current_arg;
      n +=1;
      current_arg = pow(1 + 1/n, n);
   }
   return current_arg;
}

double sum_exp(double e){
   double n = 0.0;
   double elem = 1.0;
   double sum = 0.0;
   while (fabs(elem) >= e){
      n +=1;
      sum += elem;
      elem = elem * (1/n);
   }
   return sum;
}

double func_exp(double e){
   double x = 2.1;
   double next_elem;
   while (fabs(log(x)- 1.0) >= e){
      x = x - ((log(x) - 1.0) / (1.0/x));
   }
   return x;
}

double lim_pi(double e) {
   double n = 0.012;
   double prev_arg = 10000.0;
   double current_arg = ( pow(pow(2,n)*fact(n),4) / ( n*pow(fact(2*n),2) ) );
   while (fabs(prev_arg - current_arg) >= e){
      prev_arg = current_arg;
      n +=1;
      current_arg = current_arg * ( (16.0*pow(n,4)*(n-1.0)) / (n*pow(2*n-1.0,2)*pow(2*n,2)) );
   }
   return current_arg;
}

double sum_pi(double e){
   double n = 1.0;
   double elem = 1.0;
   double sum = 0.0;
   while (fabs(elem) >= e){
      n +=1;
      sum += elem;
      elem = elem * (-(2*n-3.0)/(2*n-1.0));
   }
   return sum*4.0;
}

double func_pi(double e){
   double x = 1.1;
   while (fabs(cos(x)+ 1.0) >= e){
      x = x - ((cos(x) + 1.0) / (-sin(x)));
   }
   return x;
}

double lim_ln2(double e) {
   double n = 0.012;
   double prev_arg = 10000.0;
   double current_arg = ( n*(pow(2,1/n)-1.0) );
   while (fabs(prev_arg - current_arg) >= e){
      prev_arg = current_arg;
      n +=1;
      current_arg = ( n*(pow(2,1/n)-1.0) );
   }
   return current_arg;
}

double sum_ln2(double e){
   double n = 1.0;
   double elem = 1.0;
   double sum = 0.0;
   while (fabs(elem) >= e){
      n +=1;
      sum += elem;
      elem = elem * (-(n-1.0)/(n));
   }
   return sum;
}

double func_ln2(double e){
   double x = 1.1;
   while (fabs(exp(x) - 2.0) >= e){
      x = x - ((exp(x) - 2.0) / (x*exp(x)));
   }
   return x;
}


double lim_sqrt2(double e) {
   double n = 0.012;
   double prev_arg = -0.5;
   double current_arg = ( prev_arg - pow(prev_arg,2)/2 + 1.0 );
   while (fabs(prev_arg - current_arg) >= e){
      prev_arg = current_arg;
      n +=1;
      current_arg = ( prev_arg - pow(prev_arg,2)/2 + 1.0 );
   }
   return current_arg;
}

double comp_sqrt2(double e){
   double k = 2.0;
   double elem = pow(2,1/pow(2,k));
   double comp = 1.0;
   while (fabs(elem) >= e && k < 100){
      k += 1;
      comp *= elem;
      elem = pow(2,1/pow(2,k));
   }
   return comp;
}

double func_sqrt2(double e){
   double x = 1.1;
   while (fabs(x*x - 2.0) >= e){
      x = x - ((x*x - 2.0) / (2*x));
   }
   return x;
}

double compact(int k, int m){
   double res = 1.0;
   for (int i = k + 1; i <= m; i++){
      res *= (double)i;
   }
   double k_res = fact((double)m-(double)k);
   return res / k_res;
}

double lim_secret(double e) {
   if (e <= 1e-4){
      e = 1e-3;
   }
   int m = 1; 
   double current = 1.0;
   double prev = 0.0;
   double fact_k = 1.0;
   double comp;
   int sign = -1;
   while (fabsl(current-prev) >= e){
      prev = current;
      current = 0.0;
      fact_k = 1.0;
      sign = -1;
      for (int k = 1; k <= m; k++){ 
         fact_k = fact_k * (double)k;
         comp = compact(k,m);
         current += ( comp * sign * log(fact_k) )/ (double)k;
         sign = -sign;
      }
      m++;
   }
   return current;
}

double sum_secret(double e){
   double e_find = 1e-15;
   double k = 2.0;
   double elem = 1.0;
   double sum_prev = 100.0;
   double sum = 0.0;
   while (fabsl(sum - sum_prev) >= e){
      sum_prev = sum;
      elem = pow(floor(sqrt(k)),2);
      if (fabsl(elem-k) >= e_find){
         sum += 1.0 / elem - 1.0 / k;
      }
      else{
         sum_prev = 100000.0;
      }
      k+=1;
  }
  return sum + (-pow(M_PI,2) / 6);
}

bool check_prime(int n)
{
    if (n < 2)
    {
        return false;
    }
    if (n == 2)
    {
        return true;
    }
    if (n % 2 == 0) 
    {
        return false;
    }
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0)
        {
            return false;
        }
    }
    return true;
}

double func_secret(double e){
   if (e <= 1e-6){
      e = 1e-5;
   }
   double x = 1.0;
   int t = 2;
   double elem = 0.0;
   double prev_elem = 100.0;
   double mult = 1.0;
   while (fabs(prev_elem - elem) >= e){
      prev_elem = elem;
      mult = 1.0;
      for (int p = 2; p <= t; p++){
         if (check_prime(p)){
            mult *= ((double)p-1.0) / (double)p;
         }
      }
      elem = log(t) * mult;
      t++;
   }
   while (fabs(exp(-x)-elem) >= e){
      x = x - ((exp(-x)-elem) / (-x*exp(-x)));
   }
   return x;
}


int main(size_t argc, char* argv[]) {
   double x = 0.0;
   double e;
    switch(check(argc,argv)){
       case error_empty_or_incorrect_input:
          printf("Input is empty or incorrect.\n");
          break;
       case error_e_in_not_double:
          printf("Input 'e' in not double.\n");
          break;
       case input_correct:
          e = strtod(argv[1],NULL);
          if (e < 1e-9){
             printf("Enter eps greater than 1e-9\n");
             return 1;
          }
          double lim_e = lim_exp(e);
          printf("%s %.10f\n", "exp_lim: ", lim_e);
          double sum_e = sum_exp(e);
          printf("%s %.10f\n", "exp_sum: ", sum_e);
          double func_e = func_exp(e);
          printf("%s %.10f\n\n", "exp_func: ", func_e);
          double lim_p = lim_pi(e);
          printf("%s %.10f\n", "pi_lim: ", lim_p);
          double sum_p = sum_pi(e);
          printf("%s %.10f\n", "pi_sum: ", sum_p);
          double func_p = func_pi(e);
          printf("%s %.10f\n\n", "pi_func: ", func_p);
          double lim_ln = lim_ln2(e);
          printf("%s %.10f\n", "ln2_lim: ", lim_ln);
          double sum_ln = sum_ln2(e);
          printf("%s %.10f\n", "ln2_sum: ", sum_ln);
          double func_ln = func_ln2(e);
          printf("%s %.10f\n\n", "ln2_func: ", func_ln);
          double lim_sqrt = lim_sqrt2(e);
          printf("%s %.10f\n", "sqrt2_lim: ", lim_sqrt);
          double sum_sqrt = comp_sqrt2(e);
          printf("%s %.10f\n", "sqrt2_sum: ", sum_sqrt);
          double func_sqrt = func_sqrt2(e);
          printf("%s %.10f\n\n", "sqrt2_func: ", func_sqrt);
          double lim_sec = lim_secret(e);
          printf("%s %.10f\n", "secter_lim: ", lim_sec);
          double sum_sec = sum_secret(e);
          printf("%s %.10f\n", "secter_sum: ", sum_sec);
          double func_sec = func_secret(e);
          printf("%s %.10f\n", "func_sum: ", func_sec);
          break;
    }
          
       
}
