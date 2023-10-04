#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

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

enum errors {
    error_incorrect_flag,
    error_path_file1,
    error_path_file2,
    error_path_output_file,
    error_empty_or_incorrect_input,
    input_correct
};

enum errors check(int len, char* argv[]) {
    char correct_flags[][4] = { "-a", "/a", "-r", "/r" };
    if (len < 2){
       return error_empty_or_incorrect_input;
    }
    else{
       for (int i = 0; i < 4; i++){
          if (strcmp_f(argv[1], correct_flags[i]) == 0){
             if (len == 5){
                FILE *file1 = fopen(argv[2],"r");
                FILE *file2 = fopen(argv[3],"r");
                FILE *output_file = fopen(argv[4],"r");
                if (file1 == NULL){
                   return error_path_file1;
                }
                if (file2 == NULL){
                   fclose(file1);
                   return error_path_file2;
                }
                if (output_file == NULL){
                   fclose(file1);
                   fclose(file2);
                   return error_path_output_file;
                }
                fclose(file1);
                fclose(file2);
                fclose(output_file);
                return input_correct;
             }
             else if(len == 4 && argv[1][1] == 'a'){
                FILE *file1 = fopen(argv[2],"r");
                FILE *output_file = fopen(argv[3],"r");
                if (file1 == NULL){
                   return error_path_file1;
                }
                if (output_file == NULL){
                   fclose(file1);
                   return error_path_output_file;
                }
                fclose(file1);
                fclose(output_file);
                return input_correct;
             }
             else{
                return error_empty_or_incorrect_input;
             }
         }
         else if (i == 3){
            return error_incorrect_flag;
         }
     }
   }
                
}

void r_func(FILE* file1, FILE* file2, FILE* output_file) {
    int c;
    int odd_position = 1;
    int file1_finished = 0;
    int file2_finished = 0;

    while (1) {
        if (odd_position) {
            int word_started = 0;
            while ((c = fgetc(file1)) != EOF) {
                if (c != ' ' && c != '\n') {
                    fprintf(output_file, "%c", c);
                    word_started = 1;
                } 
                else if (word_started) {
                    fprintf(output_file, " ");
                    break;
                }
            }
            if (c == EOF) {
                file1_finished = 1;
            }
        } 
        else {
            int word_started = 0;
            while ((c = fgetc(file2)) != EOF) {
                if (c != ' ' && c != '\n') {
                    fprintf(output_file, "%c", c);
                    word_started = 1;
                } 
                else if (word_started) {
                    fprintf(output_file, " ");
                    break;
                }
            }
            if (c == EOF) {
                file2_finished = 1;
            }
        }
        if (file1_finished) {
            odd_position = 0;
        } 
        else if(file2_finished) {
            odd_position = 1;
        }
        else{
            odd_position = !odd_position;
        }
        if (file1_finished && file2_finished) {
            break;
        }
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

int ten_to_four(int num) {
    int res = 0;
    int multiplier = 1;

    while (num > 0) {
        int remainder = num % 4;
        res += remainder * multiplier;
        num /= 4;
        multiplier *= 10; 
    }

    return res;
}

int ten_to_eight(int num) {
    int res = 0;
    int multiplier = 1;

    while (num > 0) {
        int remainder = num % 8;
        res += remainder * multiplier;
        num /= 8;
        multiplier *= 10; 
    }

    return res;
}

void a_func(FILE* file1, FILE* output_file){
   int c;
   int flag_word = 0;
   size_t line_count = 1;
   while ((c = fgetc(file1)) != EOF) {
        if (c != ' ' && c != '\n') {
            if (line_count % 2 == 0 && line_count % 10 != 0) {
                fprintf(output_file, "%c", tollower(c));
            }
            else if (line_count % 5 == 0 && line_count % 10 != 0) {
                fprintf(output_file, "%d", ten_to_eight(c));
            }
            else if (line_count % 10 == 0) {
                c = tollower(c);
                fprintf(output_file, "%d", ten_to_four(c));
            }
            else if(line_count % 10 != 0 && line_count % 5 != 0 && line_count % 2 != 0){
                fprintf(output_file, "%c", c);
            } 
            flag_word = 1;
        }
        else if (flag_word) {
            fprintf(output_file, " ");
            line_count++;
        }
    }
}
         

int main(int argc, char* argv[]) {
    char correct_flags[][4] = { "-a", "/a", "-r", "/r" };
    switch (check(argc, argv) {
        case error_incorrect_flag:
            printf("The entered flag is incorrect\n");
            break;
        case error_path_file1:
            printf("This file1 path is incorrect or there is no such file\n");
            break;
        case error_path_file2:
            printf("This file2 path is incorrect or there is no such file\n");
            break;
        case error_empty_or_incorrect_input:
            printf("Input is empty or incorrect.\n");
            break;
        case error_path_output_file:
            printf("The paths for the output file turned out to be incorrect or such files do not exist\n");
            break;
        case input_correct:
            for (int i = 0; i < 4; i++) {
                if (strcmp_f(argv[1], correct_flags[i]) == 0) {

                   if (correct_flags[i][1] == 'r') {
                      FILE *file1 = fopen(argv[2],"r");
                      FILE *file2 = fopen(argv[3],"r");
                      FILE *output_file = fopen(argv[4],"w");
                      r_func(file1,file2,output_file);
                      fclose(file1);
                      fclose(file2);
                      fclose(output_file);
                   } 

                   else if (correct_flags[i][1] == 'a') {
                      FILE *file1 = fopen(argv[2],"r");
                      FILE *output_file = fopen(argv[3],"w");
                      a_func(file1,output_file);
                      fclose(file1);
                      fclose(output_file);  
                   }
                 }
            } 
            break;
         }
}
