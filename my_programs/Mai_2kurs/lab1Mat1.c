#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

size_t atoi_f(const char* arg) {
    size_t num = 0;
    int i = 0;
    while (arg[i] && (arg[i] >= '0' && arg[i] <= '9')) {
        num = num * 10 + (arg[i] - '0');
        i++;
    }
    return num;
}

int pow_f(int n, int d) {
    int res = 1;
    for (int i = 0; i < d; i++) {
        res *= n;
    }
    return res;
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

int strtol_f(const char* str) {
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

enum errors check_int(int len, char* argv[]) {
    char correct_flags[][12] = { "-h", "-p", "-s", "-e", "-a", "-f", "/h", "/p", "/s", "/e", "/a", "/f" };
    if (len > 3) {
        return error_empty_or_incorrect_input;
    } else {
        for (int i = 0; i < 12; i++) {
            if (strcmp_f(argv[1], correct_flags[i]) == 0) {
                if (strtol_f(argv[2])) {
                    return input_correct;
                } else {
                    return error_invalid_number_type;
                }
            } else {
                if (i != 11) {
                    continue;
                } else if (strtol_f(argv[2])) {
                    return error_incorrect_flag;
                } else {
                    return error_empty_or_incorrect_input;
                }
            }
        }
    }
}

int* h_func(size_t num) {
    int* arr = malloc(sizeof(int) * 100);
    if (arr == NULL){
       return NULL;
    }
    else{
    int l = 0;
      if (num <= 100) {
          for (size_t i = num; i <= 100; i = i + num) {
              if (i % num == 0) {
                  arr[l] = i;
                  l++;
              } else {
                  continue;
              }
          }
      }
      else{
          return arr;
      }
    }
    return arr;
}

bool p_func(size_t num) {
    if (num <= 1) {
        return false;
    }
    bool *sieve = (bool *)malloc((num + 1) * sizeof(bool));
    if (sieve == NULL) {
        fprintf(stderr, "Memory allocation failed in p_func.\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i <= num; ++i) {
        sieve[i] = true;
    }
    for (size_t p = 2; p * p <= num; ++p) {
        if (sieve[p]) {
            for (size_t i = p * p; i <= num; i += p) {
                sieve[i] = false;
            }
        }
    }
    bool result = sieve[num];
    free(sieve);
    return result;
}

char** e_func(size_t num) {
    char** arr_str = malloc(sizeof(char*) * 10 * num);
    if (arr_str == NULL) {
        return NULL;
    }
    char temp_str[25];
    int cnt = 0;
    int result;
    for (int base_num = 1; base_num <= 10; base_num++) {
        result = 1;
        for (int degree = 1; degree <= num; degree++) {
            result *=base_num;
            snprintf(temp_str, sizeof(temp_str), "Base %d: %d^%d=%d", base_num, base_num, degree, result);
            arr_str[cnt] = malloc(sizeof(char) * (strlen(temp_str) + 1));
            if (arr_str[cnt] == NULL) {
                for (int i = 0; i < cnt; i++) {
                    free(arr_str[i]);
                }
                free(arr_str);
                return NULL;
            }
            strcpy(arr_str[cnt], temp_str);
            cnt++;
        }
    }
    return arr_str;
}


size_t a_func(size_t num) {
    size_t res = (num * (num + 1)) / 2;
    return res;
}

size_t f_func(size_t num) {
    size_t res = 1;
    for (int i = 2; i <= num; i++) {
        res *= i;
    }
    return res;
}

int main(int argc, char* argv[]) {
    char correct_flags[][12] = { "-h", "-p", "-s", "-e", "-a", "-f", "/h", "/p", "/s", "/e", "/a", "/f" };
    switch (check_int(argc, argv)) {
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
            for (int i = 0; i < 12; i++) {
                if (strcmp_f(argv[1], correct_flags[i]) == 0) {

                    if (correct_flags[i][1] == 'h') {
                       int* natural = h_func(atoi_f(argv[2]));
                       if (natural == NULL) {
                          printf("Memory allocation failed in h_func.\n");
                       } 
                       else if (natural[0] == 0) {
                            printf("num > 100\n");
                       }
                       else {
                            printf("The numbers are multiples: ");
                            int cnt_n = 0;
                            while (natural[cnt_n] != 0) {
                                  printf("%d ", natural[cnt_n]);
                                  cnt_n++;
                            }
                            printf("\n");
                       }
                       free(natural);
                       break;
                   } 

                    else if (correct_flags[i][1] == 'p') {
                        if(p_func(atoi_f(argv[2]))){
                            printf("The entered number is simple.\n");
                        }
                        else {
                            printf("The entered number is composite.\n");
                        }
                        break;
                    }

                    else if (correct_flags[i][1] == 's') {
                        char* digits = argv[2];
                        size_t len = strlen(argv[2]);
                        int cnt_s = 0;
                        while (cnt_s != len) {
                            printf("%c ", digits[cnt_s]);
                            cnt_s++;
                        }
                        printf("\n");
                        break;
                    }

                    else if (correct_flags[i][1] == 'e') {
                        if (atoi_f(argv[2]) <= 10) {
                            char** base_str = e_func(atoi_f(argv[2]));
                            if (base_str == NULL){
                               printf("Memory allocation failed in e_func.\n");
                            }
                            else{
                               for (int i = 0; i < (10 * atoi_f(argv[2])); i++) {
                                   printf("%s\n", base_str[i]);
                                   free(base_str[i]);
                               }
                               free(base_str);
                            }
                        } 
                        else {
                            printf("To use this flag, the number must be no more than 10.\n");
                        }
                        break;
                    }

                    else if (correct_flags[i][1] == 'a') {
                        size_t res_a = a_func(atoi_f(argv[2]));
                        if (res_a == 0) {
                            printf("The resulting sum turned out to be too large'.\n");
                        } else {
                            printf("Result sum: %lu\n", res_a);
                        }
                        break;
                    }
                    else if (correct_flags[i][1] == 'f') {
                        size_t res_f = f_func(atoi_f(argv[2]));
                        if (res_f == 0) {
                            printf("The factorial of the entered number turned out to be too large.\n");
                        } else {
                            printf("Result factorial: %lu\n", res_f);
                        }
                        break;
                    }
                }
            }
            break;
    }
}
