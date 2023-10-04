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

size_t all_to_ten(char *buffer, int base){
   size_t len = strlen(buffer);
   size_t res = 0;
   for (size_t i = 0; i < len; i++){
       res = res*base + char_to_int(buffer[i]);
   }
   return res;
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
         fprintf(output, "%c" , c);
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
          
          fprintf(output, " ");
          if (max_c >= '0' && max_c <= '9') {
              base = max_c - '0' + 1;
          } 
          else if (tollower(max_c) >= 'a' && tollower(max_c) <= 'z') {
              base = tollower(max_c) - 'a' + 11;
          }
          else{
              base = 0;
          }
          if (base <= 0 || base > 36){
             fprintf(output, "The entered number turned out to be incorrect\n");
          }
          else{   
             size_t result = all_to_ten(buffer,base);
             fprintf(output, "%d %ld\n", base,result);
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
