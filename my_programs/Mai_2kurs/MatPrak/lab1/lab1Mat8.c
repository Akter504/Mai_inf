#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

enum errors {
    error_path_input,
    error_path_output,
    error_empty_or_incorrect_input,
    input_correct
};

enum errors check(int len, char* argv[]) {
    if (len == 3){
       FILE* input_file = fopen(argv[1],"r");
       FILE* output_file = fopen(argv[2],"r");
       if (input_file == NULL){
          return error_path_input;
       }
       if (output_file == NULL){
          fclose(input_file);
          return error_path_output;
       }
       fclose(input_file);
       fclose(output_file);
       return input_correct;
    }
    else{
       return error_empty_or_incorrect_input;
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

void find_notation(FILE* input, FILE* output){
   int c;
   int max_c = 0;
   int flag_word = 0;
   char* buffer = NULL;
   int buffer_size = 0;
   int buffer_index = 0;
   while ((c = fgetc(input)) != EOF){
      if (c != ' ' && c != '\n'){
         if (max_c < c){
            max_c = c;
         }
         buffer = (char*)realloc(buffer, (buffer_size + 1) * sizeof(char));
         if (buffer == NULL) {
             perror("Error allocation memory\n");
             exit(1);
         }
         buffer[buffer_index++] = c;
         buffer_size++;
         flag_word = 1;
      }
      else if (flag_word) {
          int base = 0;
          buffer = (char*)realloc(buffer, (buffer_size + 1) * sizeof(char));
          if (buffer == NULL) {
             perror("Error allocation memory\n");
             exit(1);
          }
          buffer[buffer_index] = '\0';
          
          int valid = 1;
          for (int i = 0; i < buffer_size; i++) {
              if ((buffer[i] < '0' || (buffer[i] > '9' && buffer[i] < 'A') || (buffer[i] > 'Z' && buffer[i] < 'a') || buffer[i] > 'z')) {
                  if (buffer[i] == '-' && i == 0){
                     continue;
                  }
                  else{
                     valid = 0;
                     break;
                  }
              }
          }

          if (!valid) {
             fprintf(output, " The entered number is incorrect\n");
          }
          else {
              fprintf(output, " ");
              if (max_c >= '0' && max_c <= '9') {
                  base = max_c - '0' + 1;
              }
              else if (max_c >= 'A' && max_c <= 'Z') {
                  base = max_c - 'A' + 11;
              }
              else if (max_c >= 'a' && max_c <= 'z') {
                  base = max_c - 'a' + 11;
              }
              else {
                  base = 0;
              }
              if (base <= 0 || base > 36) {
                  fprintf(output, "The entered number turned out to be incorrect\n");
              }
              else {
                  int result = all_to_ten(buffer, base);
                  int first_non_zero;
                  if (buffer[0] == '-'){
                     first_non_zero = 1;
                     int len_str = strlen(buffer);
                     while (buffer[first_non_zero] == '0' && first_non_zero != len_str-1) {
                        first_non_zero++;
                     }
                     fprintf(output, "%s%s %d %d\n","-", buffer + first_non_zero, base, result);
                  }
                  else{
                     first_non_zero = 0;
                     int len_str = strlen(buffer);
                     while (buffer[first_non_zero] == '0' && first_non_zero != len_str-1) {
                        first_non_zero++;
                     }
                     fprintf(output, "%s %d %d\n", buffer + first_non_zero, base, result);
                  }
              }
          }
          max_c = 0;
          free(buffer);
          buffer = NULL;
          buffer_size = 0;
          buffer_index = 0;
          flag_word = 0;
       }
    }
    if (buffer != NULL){
       free(buffer);
    }
}

int main(int argc, char* argv[]) {
    switch (check(argc, argv)) {
        case error_path_input:
            printf("This input path is incorrect or there is no such file\n");
            break;
        case error_path_output:
            printf("This output path is incorrect or there is no such file\n");
            break;
        case error_empty_or_incorrect_input:
            printf("Input is empty or incorrect.\n");
            break;
        case input_correct:
            FILE* input = fopen(argv[1],"r");
            FILE* output = fopen(argv[2],"w");
            find_notation(input, output);
            fclose(input);
            fclose(output);
                
            break;
         }
}
