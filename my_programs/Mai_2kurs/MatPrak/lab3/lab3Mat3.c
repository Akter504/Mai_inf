#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
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
    char correct_flags[][4] = { "-a", "/a", "-d", "/d" };
    if (len < 2){
       return error_empty_or_incorrect_input;
    }
    else{
       for (int i = 0; i < 4; i++){
          if (strcmp_f(argv[1], correct_flags[i]) == 0){
             if (len == 4){
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

typedef struct Members
{
    unsigned int id;
    char* name;
    char* surname;
    double salary;
}Employee;

int count_words_in_line(const char* line) {
    int count = 0;
    char* temp = strdup(line);
    char* token = strtok(temp, " ");

    while (token != NULL) {
        count++;
        token = strtok(NULL, " ");
    }

    free(temp);
    return count;
}

int check_id(char* id)
{
    size_t len = strlen(id);
    for(size_t i = 0; i < len; i++)
    {
        if (!isdigit(id[i]))
        {
            return 0;
        }
    }
    return 1;
}

int check_name(char* name)
{
    size_t len = strlen(name);
    for(size_t i = 0; i < len; i++)
    {
        if (!isalpha(name[i]))
        {
            return 0;
        }
    }
    return 1;
}

int check_surname(char* surname)
{
    size_t len = strlen(surname);
    for(size_t i = 0; i < len; i++)
    {
        if (!isalpha(surname[i]))
        {
            return 0;
        }
    }
    return 1;
}

int check_salary(char* salary)
{
    size_t len = strlen(salary);
    int count = 0;
    for(size_t i = 0; i < len; i++)
    {
        if (!isdigit(salary[i]))
        {
            if (salary[i] == '.' && i != 0 && i != len-1 && count != 1) //check
            {
                count++;
            }
            else
            {
                return 0;
            }
        }
    }
    return 1;
}

enum error_process
{
    error_id,
    error_name,
    error_surname,
    error_salary,
    no_error_process
};

enum error_process process_file(Employee** guests,FILE* input, int n) {
    char line[256];
    int count = 0;
    while (fgets(line, sizeof(line), input)) {

        char* words[5];
        int word_count = 0;

        char* token = strtok(line, " "); // Разбиваем строку на слова

        while (token != NULL && word_count < n) {
            words[word_count] = token;
            word_count++;
            token = strtok(NULL, " ");
        }
        if (check_id)
        {
            (*guests)[count].id = strtol(words[0],NULL,10);
            if (check_name)
            {
                (*guests)[count].name = strdup(words[1]);
                if (check_surname)
                {
                    (*guests)[count].surname = strdup(words[2]);
                    if (check_salary)
                    {
                        (*guests)[count].salary = strtod(words[3], NULL); 
                        count++;       
                    }
                    else
                    {
                        return error_salary;
                    }
                }
                else
                {
                    return error_surname;
                }
            }
            else
            {
                return error_name;
            }
        }
        else
        {
            return error_id;
        }
    }
    return no_error_process;

}

int compare_records_increasing(Employee *guest1, Employee *guest2) {
    double eps = 0.00001;
    if ((guest1->salary-guest2->salary) < eps) 
    {
        return -1;
    } else if (guest1->salary > guest2->salary) 
    {
        return 1;
    }

    int surname_compare = strcmp(guest1->surname, guest2->surname);
    if (surname_compare != 0) 
    {
        return surname_compare;
    }

    int name_compare = strcmp(guest1->name, guest2->name);
    if  (name_compare != 0) 
    {
        return name_compare;
    }

    if (guest1->id < guest2->id) 
    {
        return -1;
    } 
    else if (guest1->id > guest2->id) 
    {
        return 1;
    }
}

int compare_records_decreasing(Employee *guest1, Employee *guest2) {
    double eps = 0.00001;
    if ((guest1->salary-guest2->salary) < eps) 
    {
        return 1;
    } 
    else if (guest1->salary > guest2->salary) 
    {
        return -1;
    }

    int surname_compare = strcmp(guest1->surname, guest2->surname);
    if (surname_compare != 0) 
    {
        return -surname_compare;
    }

    int name_compare = strcmp(guest1->name, guest2->name);
    if  (name_compare != 0) 
    {
        return -name_compare;
    }

    if (guest1->id < guest2->id) 
    {
        return 1;
    } 
    else if (guest1->id > guest2->id) 
    {
        return -1;
    }
}

void count_lines_in_file(FILE* file, int* line_count) 
{
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL) 
    {
        (*line_count)++;
    }
    rewind(file);
}

int main(int argc, char* argv[]) {
    char correct_flags[][4] = { "-a", "/a", "-d", "/d" };
    FILE *input;
    FILE *output;
    switch (check(argc, argv)) {
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
            input = fopen(argv[2],"r");
            output = fopen(argv[3],"w");
            int line_count = 0;
            count_lines_in_file(input, &line_count);
            Employee* guests = (Employee*)malloc(sizeof(Employee)*line_count);
            if (guests == NULL)
            {
                printf("Error: allocation memory\n");
                return 2;
            }
            enum error_process check_p = process_file(&guests,input,4);
            fclose(input);
            if (check_p == error_id)
            {
                printf("Incorrect id\n");
                return 1;
            }
            else if (check_p == error_name)
            {
                printf("Incorrect name\n");
                return 1;
            }
            else if (check_p == error_surname)
            {
                printf("Incorrect surname\n");
                return 1;
            }
            else if (check_p == error_salary)
            {
                printf("Incorrect salary\n");
                return 1;
            }
            else if (check_p == no_error_process)
            {
                if (argv[1][1] == 'a')
                {
                    qsort(guests, line_count, sizeof(Employee), (int (*)(const void *, const void *))compare_records_increasing);
                }
                else if (argv[1][1] == 'd')
                {
                    qsort(guests, line_count, sizeof(Employee), (int (*)(const void *, const void *))compare_records_decreasing);
                }
                for (int i = 0; i < line_count; i++)
                {
                    fprintf(output, "Guest %d: %d, %s, %s, %f\n", i, guests[i].id, guests[i].name, guests[i].surname, guests[i].salary);
                    free(guests[i].name);
                    free(guests[i].surname);
                }
                free(guests);
                fclose(output);
            }
            break;
         }
}