#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

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
    for (int i = 0; i < strlen(str); i++) {
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
    return arr;
}

int p_func(size_t num) {
    for (size_t i = 2; i * i < num; i++) {
        if (num % i == 0) {
            return 1;
        } else {
            return 0;
        }
    }
}

char* s_func(size_t num) {
    int len = 0;
    size_t trash = num;
    while (trash > 0) {
        trash /= 10;
        len++;
    }
    char* arr = malloc(sizeof(char) * len);
    char* rev_arr = malloc(sizeof(char) * len);
    int i = 0, j = 0;
    while (num > 0) {
        arr[i] = (num % 10) + '0';
        num /= 10;
        i++;
    }
    i--;
    while (i != -1) {
        rev_arr[j] = arr[i];
        i--;
        j++;
    }
    free(arr);
    return rev_arr;
}

char** e_func(size_t num) {
    char** arr_str = malloc(sizeof(char*) * 10 * num);
    int result;
    for (int i = 0; i < (num * 10); i++) {
        arr_str[i] = malloc(sizeof(char) * 25);
    }
    int cnt = 0;
    for (int base_num = 1; base_num <= 10; base_num++) {
        for (int degree = 1; degree <= num; degree++) {
            result = pow_f(base_num, degree);
            sprintf(arr_str[cnt], "Base %d: %d^%d=%d", base_num, base_num, degree, result);
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
            printf("The number type is not 'long'.\n");
            break;
        case error_empty_or_incorrect_input:
            printf("Input is empty or incorrect.\n");
            break;
        case input_correct:
            for (int i = 0; i < 12; i++) {
                if (strcmp_f(argv[1], correct_flags[i]) == 0) {

                    if (correct_flags[i][1] == 'h') {
                        int* natural = h_func(atoi_f(argv[2]));
                        if (natural[0] == 0) {
                            printf("num > 100\n");
                        } else {
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
                        if (p_func(atoi_f(argv[2]))) {
                            printf("The entered number is simple.\n");
                        } else {
                            printf("The entered number is composite.\n");
                        }
                        break;
                    }

                    else if (correct_flags[i][1] == 's') {
                        char* digits = s_func(atoi_f(argv[2]));
                        size_t num = atoi_f(argv[2]);
                        int cnt_s = 0;
                        while (num > 0) {
                            num /= 10;
                            printf("%c ", digits[cnt_s]);
                            cnt_s++;
                        }
                        printf("\n");
                        free(digits);
                        break;
                    }

                    else if (correct_flags[i][1] == 'e') {
                        if (atoi_f(argv[2]) <= 10) {
                            char** base_str = e_func(atoi_f(argv[2]));
                            for (int i = 0; i < (10 * atoi_f(argv[2])); i++) {
                                printf("%s\n", base_str[i]);
                                free(base_str[i]);
                            }
                            free(base_str);
                        } else {
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

