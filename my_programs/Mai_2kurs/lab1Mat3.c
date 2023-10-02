#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>

size_t atoi_f(const char* arg) {
    size_t num = 0;
    int i = 0;
    while (arg[i] && (arg[i] >= '0' && arg[i] <= '9')) {
        num = num * 10 + (arg[i] - '0');
        i++;
    }
    return num;
}

int strcmp_f(const char* str1, const char* str2) {
    while (*str1) {
        if (*str1 != *str2) {
            break;
        }
        str1++;
        str2++;
    }
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

bool check_double(const char *str) {
    double num;
    int result = sscanf(str, "%lf", &num);
    return result == 1 && strchr(str, '.') != NULL;
}

int check_int(const char* str) {
    size_t len = strlen(str);
    for (int i = 0; i < len; i++) {
        if ((str[i] >= '0' && str[i] <= '9')) {
            continue;
        } else {
            return 0;
        }
        return 1;
    }
}

enum errors {
    error_incorrect_flag,
    error_invalid_number_type,
    error_empty_or_incorrect_input,
    error_over_unsigned_long,
    input_correct
};

enum errors check(int len, char* argv[]) {
    char correct_flags[][6] = { "-q", "-m", "-t", "/q", "/m", "/t" };
    for (int i = 0; i < 6; i++) {
        if (argv[1][1] == 'q') {
           if (len != 6){
              return error_empty_or_incorrect_input;
           }
           else{
              if (check_double(argv[2])&&check_double(argv[3])&&check_double(argv[4])&&check_double(argv[5])){
                 return input_correct;
              }
              else{
                 return error_invalid_number_type;
              } 
           }
        }
        else if(argv[1][1] == 'm'){
           if (len != 4){
              return error_empty_or_incorrect_input;
           }
           else{
              if (check_int(argv[2])&&check_int(argv[3])){
                 return input_correct;
              }
              else{
                 return error_invalid_number_type;
              }
           }
        
        }
        else if(argv[1][1] == 't'){
           if (len != 6){
              return error_empty_or_incorrect_input;
           }
           else{
              if (check_double(argv[2])&&check_double(argv[3])&&check_double(argv[4])&&check_double(argv[5])){
                 return input_correct;
              }
              else{
                 return error_invalid_number_type;
              } 
           }
        }
        else{
           return error_incorrect_flag;
        }
    }
}


typedef struct roots_quadration_equations{
     double** arr;
     int cnt_arr;
     int* count_el;
}roots;
          

void free_roots(roots** equation) {
    for (int i = 0; i < (*equation)->cnt_arr; i++) {
        free((*equation)->arr[i]);
    }
    free((*equation)->arr);
    free((*equation)->count_el);
    free(*equation);
    *equation = NULL;
}

roots* create_roots(int situation){
   roots* res = malloc(sizeof(roots));
   if (res == NULL){
      perror("Memory allocation failed for roots structure");
      exit(1);
    }
    res->cnt_arr = 0;
    res->arr = malloc(sizeof(double*) * situation);
    res->count_el = malloc(sizeof(int) * situation);
    if (res->arr == NULL || res->count_el == NULL) {
        free(res->arr);
        free(res->count_el);
        free(res);
        perror("Memory allocation failed for roots data");
        exit(1);
    }
    for (int i = 0; i < situation; i++){
        res->arr[i] = malloc(sizeof(double)*2);
        res->count_el[i] = 0;
        if (res->arr[i] == NULL){
           perror("Memory allocation failed for roots data");
           for (int j = 0; j < i; j++){
               free(res->arr[j]);
           }
           free(res->count_el);
           free(res->arr);
           free(res);
           exit(1);
       }
   }
   return res;
}
          
           

void discriminant(double a, double b, double c, double eps,roots** res){
     double d = b*b-4*a*c;
     if (fabs(d) <= eps) {
        (*res)->arr[(*res)->cnt_arr][0] = (-b)/(2*a);
        (*res)->arr[(*res)->cnt_arr][1] = 0.0;
        (*res)->count_el[(*res)->cnt_arr] = 1;
        (*res)->cnt_arr +=1; 
     } 
     else if (d > eps) {
        (*res)->arr[(*res)->cnt_arr][0] = (-b+sqrt(d))/(2*a);
        (*res)->arr[(*res)->cnt_arr][1] = (-b-sqrt(d))/(2*a);
        (*res)->count_el[(*res)->cnt_arr] = 2;
        (*res)->cnt_arr +=1;
     } 
     else if (d < -eps) {
        free((*res)->arr[(*res)->cnt_arr]);
        (*res)->count_el[(*res)->cnt_arr] = 0;
        (*res)->arr[(*res)->cnt_arr] = NULL;
        (*res)->cnt_arr++;
     }
}

enum q_errors{
     q_succes,
     q_allocation_memory,
     q_bad_situation
};

enum q_errors q_func(char * argv[], roots** equation){
    if (*equation != NULL) {
        free_roots(equation);
    }
    double eps = strtod(argv[2],NULL),arg1 = strtod(argv[3],NULL),arg2 = strtod(argv[4],NULL),arg3 = strtod(argv[5],NULL);
    if (fabs(arg1-arg2)<eps && fabs(arg2-arg3)<eps && fabs(arg1-arg3)<eps){
       (*equation) = create_roots(1);
       discriminant(arg1,arg2,arg3,eps,equation);
       return q_succes;  
    }   
    else if (fabs(arg1-arg2)>eps && fabs(arg2-arg3)<=eps && fabs(arg1-arg3)>eps){
       (*equation) = create_roots(3);
       discriminant(arg1,arg2,arg3,eps,equation);
       discriminant(arg2,arg1,arg3,eps,equation);
       discriminant(arg2,arg3,arg1,eps,equation);
       return q_succes;
          
    }
    
       
    else if (fabs(arg1-arg2)>eps && fabs(arg2-arg3)>eps && fabs(arg1-arg3)<=eps){
       (*equation) = create_roots(3);
       discriminant(arg1,arg2,arg3,eps,equation);
       discriminant(arg2,arg1,arg3,eps,equation);
       discriminant(arg1,arg3,arg2,eps,equation);
       return q_succes;
    }
    else if (fabs(arg1-arg2)<=eps && fabs(arg2-arg3)>eps && fabs(arg1-arg3)>eps){
       (*equation) = create_roots(3);
       discriminant(arg1,arg2,arg3,eps,equation);
       discriminant(arg3,arg2,arg1,eps,equation);
       discriminant(arg1,arg3,arg2,eps,equation);
       return q_succes;
    }
    else if (fabs(arg1-arg2)>eps && fabs(arg2-arg3)>eps && fabs(arg1-arg3)>eps){
       (*equation) = create_roots(6);
       discriminant(arg1,arg2,arg3,eps,equation);
       discriminant(arg2,arg1,arg3,eps,equation);
       discriminant(arg3,arg2,arg1,eps,equation);
       discriminant(arg1,arg3,arg2,eps,equation);
       discriminant(arg3,arg1,arg2,eps,equation);
       discriminant(arg2,arg3,arg1,eps,equation);
       return q_succes;
   }
   return q_bad_situation;
}
     
    
int m_func(char* a,char* b){
   size_t arg1 = atoi_f(a), arg2 = atoi_f(b);
   return arg1%arg2==0;
}

enum t_errors{
     t_succes,
     t_triangle_not_exist,
     t_not_right_triangle,
     t_negative_coefficients
};

enum t_errors t_func(char * argv[]){
   double eps = strtod(argv[2],NULL),a = strtod(argv[3],NULL),b = strtod(argv[4],NULL),c = strtod(argv[5],NULL);
   if ((a<-eps)||(b<-eps)||(c<-eps)){
      return t_negative_coefficients;
   }
   else{
      if (fabs((a+b)-c)>eps && fabs((a+c)-b)>eps && fabs((b+c)-a)>eps){
         if (fabs(c*c-(a*a+b*b))<eps || fabs(a*a-(c*c+b*b))<eps || fabs(b*b-(a*a+c*c))<eps){
            return t_succes;
         }
         else{
            return t_not_right_triangle;
         }
      }
      else{
         return t_triangle_not_exist;
      }  
   }    
}

int main(int argc, char* argv[]) {
    char correct_flags[][6] = { "-q", "-m", "-t", "/q", "/m", "/t" };
    switch (check(argc, argv)) {
        case error_incorrect_flag:
            printf("The flag enter incorrectly.\n");
            break;
        case error_invalid_number_type:
            printf("The number type is wrong.\n");
            break;
        case error_empty_or_incorrect_input:
            printf("Input is empty or incorrect.\n");
            break;
        case input_correct:
            for (int i = 0; i < 6; i++) {
                if (strcmp_f(argv[1], correct_flags[i]) == 0) {

                    if (correct_flags[i][1] == 'q') {
                       roots* result = NULL;
                       switch(q_func(argv, &result)){
                          case q_allocation_memory:
                             printf("Memory allocation failed in q_func.\n");
                             break;
                          case q_bad_situation:
                             printf("Bad\n");
                             break;
                          case q_succes:
                             printf("%d", result->cnt_arr);
                             for (int i = 0; i < result->cnt_arr; i++){
                                 if (result->arr[i] == NULL){
                                     printf("Roots not found.\n");
                                     continue;
                                 }
                                 else{
                                     if (result->count_el[i] == 2){
                                         printf("%s%lf %lf%s","Roots x1,x2: ",result->arr[i][0],result->arr[i][1],"\n");
                                     }
                                        
                                 }
                             }
                             free_roots(&result);
                             break;
                        }
                     }
                         
                    else if (correct_flags[i][1] == 'm') {
                        int check = m_func(argv[2],argv[3]);
                        if (check) {
                            printf("The first number is a multiple of the second.\n");
                        } else {
                            printf("The first number is not a multiple of the second\n");
                        }
                        break;
                    }
                    else if (correct_flags[i][1] == 't') {
                        switch(t_func(argv)){
                           case t_negative_coefficients:
                              printf("The coefficients of the sides of a triangle cannot be negative.\n");
                              break;
                           case t_triangle_not_exist:
                              printf("A triangle with such coefficients does not exist.\n");
                              break;
                           case t_not_right_triangle:
                              printf("The entered coefficients do not form a right triangle.\n");
                              break;
                           case t_succes:
                              printf("The entered coefficients form a right triangle.\n");
                              break;
                        }
                    }
                }
            }
            break;
    }
}
