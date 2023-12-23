#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

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

int check_int(const char* str) {
    size_t len = strlen(str);
    for (int i = 0; i < len; i++) {
        if ((str[i] >= '0' && str[i] <= '9')) {
            continue;
        } 
        else {
           return 0;
        }
        return 1;
    }
}

int tollower(char c){
   if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    } 
    else {
        return c;
    }
}

int char_to_int(char elem){
    if ((elem >= '0') && (elem <= '9')){
       return elem - '0';
    }
    else if((tollower(elem) >= 'a') && (tollower(elem) <= 'z')){
       return tollower(elem) - 'a' + 10;
    }
}

int all_to_ten(char *buffer, int base) {
    size_t len = strlen(buffer);
    int res = 0;
    int sign = 1;
    if (buffer[0] == '-') {
        sign = -1;
        buffer++; 
        len--;
    }

    for (size_t i = 0; i < len; i++) {
        res = res * base + char_to_int(buffer[i]);
    }
    return sign * res;
}

int check_num_numeration(size_t len, char* argv[]) {
    int base = atoi(argv[1]);
    if (base < 2 || base > 36) {
        return 0;
    }

    for (size_t i = 2; i < len - 1; i++) {
        char* args = argv[i];
        for (size_t j = 0; args[j] != '\0'; j++) {
            char arg = tolower(args[j]);
            if (isdigit(arg)) {
                if (arg - '0' >= base) {
                    return 0;
                }
            } 
            else if (islower(arg)) {
                if (arg - 'a' + 10 >= base) {
                    return 0;
                }
            } 
            else {
                return 0;
            }
        }
    }
    return 1;
}


int max_num(size_t len, char* argv[],int* index){
   int base = atoi(argv[1]);
   int max_res = fabs(all_to_ten(argv[2],base));
   (*index) = 2;
   for (int i = 3; i < len-1; i++){
      int res = fabs(all_to_ten(argv[i],base));
      if (max_res < res){
          max_res = res;
          (*index) = i;
      }
   }
   return max_res;
}
      
enum errors {
    error_numeration_system,
    error_num_numeration,
    error_end_not_stop,
    error_numeration_out_of_range,
    error_empty_or_incorrect_input,
    input_correct
};

enum errors check(size_t len, char* argv[]) {
    if (len < 4){
       return error_empty_or_incorrect_input;
    }
    else{
       if (!check_int(argv[1])){
          return error_numeration_system;
       }
       else{
          if (strcmp_f(argv[len-1],"Stop") == 0){
             if ((atoi(argv[1]) >= 2) && (atoi(argv[1]) <= 36)){
                if (check_num_numeration(len,argv)){
                   return input_correct;
                }
                else{
                   return error_num_numeration;
                }
             }
             else{
                return error_numeration_out_of_range;
             }
          }
          else{
             return error_end_not_stop;
          }
       }  
    }              
}

int ten_to_nine(int num) {
    int res = 0;
    int multiplier = 1;

    while (num > 0) {
        int remainder = num % 9;
        res += remainder * multiplier;
        num /= 9;
        multiplier *= 10; 
    }

    return res;
}

char* ten_to_eighteen(int decimal) {
    char* base18 = NULL;
    int index = 0; 
    while (decimal > 0) {
        int remainder = decimal % 18;
        if (index == 0) {
            base18 = (char*)malloc(2 * sizeof(char));
        } 
        else {
            base18 = (char*)realloc(base18, (index + 2) * sizeof(char));
        }
        if (base18 == NULL) {
            perror("Memory allocation error.\n");
            exit(1);
        }
        if (remainder < 10) {
            base18[index] = remainder + '0';
        } 
        else {
            base18[index] = remainder - 10 + 'A';
        }
        base18[index + 1] = '\0';
        decimal /= 18;
        index++;
    }
    for (int i= 0, j = index - 1; i < j; i++, j--) {
        char temp = base18[i];
        base18[i] = base18[j];
        base18[j] = temp;
    }
    return base18;
}

char* ten_to_twentyseventeen(int decimal) {
    char* base27 = NULL;
    int index = 0; 
    while (decimal > 0) {
        int remainder = decimal % 27;
        if (index == 0) {
            base27 = (char*)malloc(2 * sizeof(char));
        } 
        else {
            base27 = (char*)realloc(base27, (index + 2) * sizeof(char));
        }
        if (base27 == NULL) {
            perror("Memory allocation error.\n");
            exit(1);
        }
        if (remainder < 10) {
            base27[index] = remainder + '0';
        } 
        else {
            base27[index] = remainder - 10 + 'A';
        }
        base27[index + 1] = '\0';
        decimal /= 27;
        index++;
    }
    for (int i= 0, j = index - 1; i < j; i++, j--) {
        char temp = base27[i];
        base27[i] = base27[j];
        base27[j] = temp;
    }
    return base27;
}

char* ten_to_thirtysixteen(int decimal) {
    char* base36 = NULL;
    int index = 0; 
    while (decimal > 0) {
        int remainder = decimal % 36;
        if (index == 0) {
            base36 = (char*)malloc(2 * sizeof(char));
        } 
        else {
            base36 = (char*)realloc(base36, (index + 2) * sizeof(char));
        }
        if (base36 == NULL) {
            perror("Memory allocation error.\n");
            exit(1);
        }
        if (remainder < 10) {
            base36[index] = remainder + '0';
        } 
        else {
            base36[index] = remainder - 10 + 'A';
        }
        base36[index + 1] = '\0';
        decimal /= 36;
        index++;
    }
    for (int i= 0, j = index - 1; i < j; i++, j--) {
        char temp = base36[i];
        base36[i] = base36[j];
        base36[j] = temp;
    }
    return base36;
}

int main(size_t argc, char* argv[]) {
    switch (check(argc, argv)) {
        case error_numeration_system:
            printf("The numeration system is entered incorrectly\n");
            break;
        case error_num_numeration:
            printf("One of the entered numbers cannot be represented in this numeration system.\n");
            break;
        case error_numeration_out_of_range:
            printf("The number system is limited [2,36]\n");
            break;
        case error_end_not_stop:
            printf("The line 'Stop' at the end of the input was not found\n");
            break;
        case error_empty_or_incorrect_input:
            printf("Input is empty or incorrect.\n");
            break;
        case input_correct:
            int res,index;
            res = max_num(argc,argv,&index);
            printf("%s %s\n", "The maximum number modulo:", argv[index]);
            int res9 = ten_to_nine(res);
            printf("%s %d\n","Representation of this number in the 9 numeral system: ",res9);
            char* res18 = ten_to_eighteen(res);
            printf("%s %s\n","Representation of this number in the 18 numeral system: ",res18);
            char* res27 = ten_to_twentyseventeen(res);
            printf("%s %s\n","Representation of this number in the 27 numeral system: ",res27);
            char* res36 = ten_to_thirtysixteen(res);
            printf("%s %s\n","Representation of this number in the 36 numeral system: ",res36);
            free(res18);
            free(res27);
            free(res36);
            
            break;
         }
}
