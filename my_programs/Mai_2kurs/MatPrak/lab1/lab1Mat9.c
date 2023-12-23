#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int check_int(const char* str) {
    size_t len = strlen(str);
    for (int i = 0; i < len; i++) {
        if ((str[i] >= '0' && str[i] <= '9')) {
            continue;
        } 
        else {
           if ((i == 0) && (str[i] == '-')){
              continue;
           }
           else{
              return 0;
           }
        }
        return 1;
    }
}

enum errors {
    error_nums,
    error_empty_or_incorrect_input,
    error_a_more_b,
    input_correct
};

enum errors check(int len, char* argv[]) {
    if (len == 3){
       if (check_int(argv[1]) && check_int(argv[2])){
          if (atoi(argv[1]) <= atoi(argv[2])){
             return input_correct;
          }
          else{
             return error_a_more_b;
          }
       }
       else{
          return error_nums;
       }
    }
    else{
       return error_empty_or_incorrect_input;
    }                      
}

void swap_min_max(int arr[],size_t size){
   int maxind = 0;
   int minind = 0;
   for (int i = 0; i < size; i++){
      if (arr[i] > arr[maxind]){
         maxind = i;
      }
      if (arr[i] < arr[minind]){
         minind = i;
      }
   }
   int temp;
   temp = arr[maxind];
   arr[maxind] = arr[minind];
   arr[minind] = temp;
}

int generate_random_int(int min, int max) {
    return rand() % (max - min + 1) + min;
}      

void fill(int* arr, int size){
   for (int i = 0; i < size; i++){
      arr[i] = generate_random_int(-1000, 1000);
   }
}
      

int closest_num(int value, int* arr, size_t size){
   int min_diff = fabs(value - arr[0]);
   int diff = 0;
   int idx = 0;
   for (int i = 1; i < size; i++){
      diff = fabs(value - arr[i]);
      if (min_diff > diff){
         min_diff = diff;
         idx = i;
      }
   }
   return arr[idx];
}

int main(int argc, char* argv[]) {
    switch (check(argc, argv)) {
        case error_nums:
            printf("Intered nums is not int\n");
            break;
        case error_empty_or_incorrect_input:
            printf("Input is empty or incorrect.\n");
            break;
        case error_a_more_b:
            printf("Error: a > b\n");
            break;
        case input_correct:
            size_t size = 50;
            int arr[size];
            int a = atoi(argv[1]);
            int b = atoi(argv[2]);
            srand(time(NULL));
            printf("(1) Array:\n");
            for (int i = 0; i < size; i++) {
                arr[i] = generate_random_int(a,b);
                printf("%d ", arr[i]);
            }
            printf("\n");
            swap_min_max(arr,size);
            printf("Array after swap min and max elem:\n");
            for (int i = 0; i < size; i++) {
                printf("%d ", arr[i]);
            }
            printf("\n");
            
            int min_size = 10;
            int max_size = 10000;
            int size_a = generate_random_int(min_size, max_size);
            int *a_arr = malloc(size_a * sizeof(int));
            if (a_arr == NULL){
               printf("Memory allocation failed for arr A\n");
               return 1;
            }
            fill(a_arr,size_a);
            int size_b = generate_random_int(min_size, max_size);
            int *b_arr = malloc(size_b * sizeof(int));
            if (b_arr == NULL){
               printf("Memory allocation failed for arr B\n");
               return 1;
            }
            fill(b_arr,size_b);
            if (b_arr == NULL){
               printf("Memory allocation failed for arr A\n");
               return 1;
            }
            int *c_arr = malloc(size_a * sizeof(int));
            if (c_arr == NULL){
               printf("Memory allocation failed for arr B\n");
               return 1;
            }
            else{
               printf("(2) Array C:\n");
               for (int i = 0; i < size_a; i++){
                  c_arr[i] = a_arr[i] + closest_num(a_arr[i],b_arr,size_a);
                  printf("%d ", c_arr[i]);
               }
               printf("\n");
            }
            free(a_arr);
            free(b_arr);
            free(c_arr);
            break;
         }
}
