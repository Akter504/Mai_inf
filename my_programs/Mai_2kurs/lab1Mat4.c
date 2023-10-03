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
    error_input_path,
    error_output_path,
    error_empty_or_incorrect_input,
    input_correct,
    error_inp_out
};

enum errors check(int len, char* argv[],int* n_in_flag) {
    char correct_flags3[][8] = { "-nd", "/nd", "-ni", "/ni", "-ns", "/ns", "-na", "/na" };
    char correct_flags2[][8] = { "-d", "/d", "-i", "/i", "-s", "/s", "-a", "/a" };
    FILE *path_input = fopen(argv[2], "r");
    if (len < 2){
       return error_empty_or_incorrect_input;
    }
    else{
       for (int i = 0; i < 8; i++){
          if (strcmp_f(argv[1], correct_flags3[i]) == 0){
             if (len == 4){
                FILE *path_output = fopen(argv[3],"r");
                if (path_input == NULL && path_output != NULL){
                   fclose(path_output);
                   return error_input_path;
                }
                else if(path_input != NULL && path_output == NULL){
                   fclose(path_input);
                   return error_output_path;
                }
                else if (path_input != NULL && path_output != NULL){
                   fclose(path_input);
                   fclose(path_output);
                   (*n_in_flag) = 1;
                   return input_correct;
                }
                else if(path_input == NULL && path_output == NULL){
                   return error_inp_out;
                }
             }
             else{
                return error_empty_or_incorrect_input;
             }
           }
         
           if (strcmp_f(argv[1], correct_flags2[i]) == 0){
              if (len == 3){
                 if (path_input == NULL){
                    return error_input_path;
                 }
                 else{
                    fclose(path_input);
                    (*n_in_flag) = 0;
                    return input_correct;
                 }
             }
             else{
                 return error_empty_or_incorrect_input;
             }
          }
        }
        return error_incorrect_flag;
     }           
}

char* create_out_path(char *input_path) {
    char prefix[] = "out_";
    char *last_slash = strrchr(input_path, '/');
    if (last_slash == NULL) {
        perror("Error: Unable to determine the directory for the file.c");
        exit(1);
    }
    size_t dir_length = last_slash - input_path + 1;
    char *out_path = malloc(dir_length + strlen(prefix) + strlen(last_slash + 1)+1);

    if (out_path == NULL) {
        perror("Memory allocation error");
        exit(1);
    }
    strncpy(out_path, input_path, dir_length);
    strcpy(out_path + dir_length, prefix);
    strcpy(out_path + dir_length + strlen(prefix), last_slash + 1);

    FILE *path_output = fopen(out_path, "w");
    if (path_output == NULL) {
        perror("Error creating the file");
        free(out_path);
        exit(1);
    }
    fclose(path_output);
    return out_path;
}

void d_func(FILE* path_input, FILE* path_output){
    int c;
    while ((c = fgetc(path_input)) != EOF){
       if ((c < '0') || (c > '9')){
          fputc(c,path_output);
       }
    }
}

void i_func(FILE* path_input, FILE* path_output){
    int c;
    size_t count = 0;
    while ((c = fgetc(path_input)) != EOF){
       if (c == '\n') {
            fprintf(path_output, "%ld\n", count);
            count = 0;
        }
        else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            count++;
        }
    }
    if (count > 0) {
        fprintf(path_output, "%ld\n", count);
    }
}

void s_func(FILE* path_input, FILE* path_output){
    int c;
    size_t count = 0;
    while ((c = fgetc(path_input)) != EOF){
       if (c == '\n') {
            fprintf(path_output, "%ld\n", count);
            count = 0;
        }
        else if ((c < 'A' || c > 'Z') && (c < 'a' || c > 'z') && (c < '0' || c > '9') && (c != ' ')) {
            count++;
        }
    }
}

void a_func(FILE* path_input, FILE* path_output){
   int c;
    while ((c = fgetc(path_input)) != EOF) {
        if (c >= '0' && c <= '9') {
            fputc(c, path_output);
        } else {
            fprintf(path_output, "%X", c);
        }
    }
}

int main(int argc, char* argv[]) {
    char correct_flags3[][8] = { "-nd", "/nd", "-ni", "/ni", "-ns", "/ns", "-na", "/na" };
    char correct_flags2[][8] = { "-d", "/d", "-i", "/i", "-s", "/s", "-a", "/a" };
    int n_in_flag;
    switch (check(argc, argv,&n_in_flag)) {
        case error_incorrect_flag:
            printf("The entered flag is incorrect\n");
            break;
        case error_input_path:
            printf("This input path is incorrect or there is no such file\n");
            break;
        case error_output_path:
            printf("This output path is incorrect or there is no such file\n");
            break;
        case error_empty_or_incorrect_input:
            printf("Input is empty or incorrect.\n");
            break;
        case error_inp_out:
            printf("The paths for the output and input file turned out to be incorrect or such files do not exist\n");
            break;
        case input_correct:
            for (int i = 0; i < 8; i++) {
                if (strcmp_f(argv[1], correct_flags3[i]) == 0 || strcmp_f(argv[1], correct_flags2[i]) == 0) {

                    if (correct_flags2[i][1] == 'd' || (correct_flags3[i][1] == 'n' && correct_flags3[i][2] == 'd')) {
                       if (n_in_flag){
                          FILE *path_input = fopen(argv[2], "r");
                          FILE *path_output = fopen(argv[3], "w");
                          d_func(path_input,path_output);
                          fclose(path_input);
                          fclose(path_output);
                       }
                       else{
                          FILE *path_input = fopen(argv[2], "r");
                          if (path_input == NULL) {
                             printf("Wrong input path\n");
                             return 1;
                          }
                          char* out_path = create_out_path(argv[2]);
                          FILE *path_output = fopen(out_path,"w");
                          if (path_output == NULL) {
                             perror("Error creating the file\n");
                             free(out_path);
                             return 1;
                          }
                          free(out_path);
                          d_func(path_input,path_output);
                          fclose(path_input);
                          fclose(path_output);
                      }
                          
                          
                   } 

                   else if (correct_flags2[i][1] == 'i' || (correct_flags3[i][1] == 'n' && correct_flags3[i][2] == 'i')) {
                        if (n_in_flag){
                          FILE *path_input = fopen(argv[2], "r");
                          FILE *path_output = fopen(argv[3], "w");
                          i_func(path_input,path_output);
                          fclose(path_input);
                          fclose(path_output);
                       }
                       else{
                          FILE *path_input = fopen(argv[2], "r");
                          if (path_input == NULL) {
                             printf("Wrong input path\n");
                             return 1;
                          }
                          char* out_path = create_out_path(argv[2]);
                          FILE *path_output = fopen(out_path,"w");
                          if (path_output == NULL) {
                             perror("Error creating the file\n");
                             free(out_path);
                             return 1;
                          }
                          free(out_path);
                          i_func(path_input,path_output);
                          fclose(path_input);
                          fclose(path_output);
                      }
                   }

                   else if (correct_flags2[i][1] == 's' || (correct_flags3[i][1] == 'n' && correct_flags3[i][2] == 's')) {
                        if (n_in_flag){
                          FILE *path_input = fopen(argv[2], "r");
                          FILE *path_output = fopen(argv[3], "w");
                          s_func(path_input,path_output);
                          fclose(path_input);
                          fclose(path_output);
                       }
                       else{
                          FILE *path_input = fopen(argv[2], "r");
                          if (path_input == NULL) {
                             printf("Wrong input path\n");
                             return 1;
                          }
                          char* out_path = create_out_path(argv[2]);
                          FILE *path_output = fopen(out_path,"w");
                          if (path_output == NULL) {
                             perror("Error creating the file\n");
                             free(out_path);
                             return 1;
                          }
                          free(out_path);
                          s_func(path_input,path_output);
                          fclose(path_input);
                          fclose(path_output);
                      }
                   }

                   else if (correct_flags2[i][1] == 'a' || (correct_flags3[i][1] == 'n' && correct_flags3[i][2] == 'a')) {
                        if (n_in_flag){
                          FILE *path_input = fopen(argv[2], "r");
                          FILE *path_output = fopen(argv[3], "w");
                          a_func(path_input,path_output);
                          fclose(path_input);
                          fclose(path_output);
                       }
                       else{
                          FILE *path_input = fopen(argv[2], "r");
                          if (path_input == NULL) {
                             printf("Wrong input path\n");
                             return 1;
                          }
                          char* out_path = create_out_path(argv[2]);
                          FILE *path_output = fopen(out_path,"w");
                          if (path_output == NULL) {
                             perror("Error creating the file\n");
                             free(out_path);
                             return 1;
                          }
                          free(out_path);
                          a_func(path_input,path_output);
                          fclose(path_input);
                          fclose(path_output);
                      }
                   }
                        
                   }
                } 
            break;
         }
}
