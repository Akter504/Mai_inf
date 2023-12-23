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
    if (len < 2){
       return error_empty_or_incorrect_input;
    }
    else{
            if (len == 3)
            {
                FILE *file1 = fopen(argv[1],"r");
                FILE *output_file = fopen(argv[2],"r");
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
            else
            {
                return error_empty_or_incorrect_input;
            }
                
     }
}

typedef struct Members
{
    unsigned int id;
    char* name;
    char* surname;
    char* group;
    unsigned char* evaluations;
}Student;

int countWordsInLine(const char* line) {
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

int check_id_or_eval(char* id)
{
    size_t len = strlen(id);
    for(size_t i = 0; i < len; i++)
    {
        if (!isdigit(id[i]))
        {
            if (id[i] == '\n' && i == len-1)
            {
                continue;
            }
            else
            {
                return 0;
            }
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

int check_group(char* group)
{
    size_t len = strlen(group);
    int count = 0;
    for(size_t i = 0; i < len; i++)
    {
        if (!isalpha(group[i]) && !isdigit(group[i]))
        {
            if (group[i] = '-' && (i != 0 || i!= len-1))
            {
                count++;
                if (count == 2)
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            count = 0;
        }
    }
    return 1;
}

enum error_process
{
    error_process_id,
    error_process_eval,
    error_process_name,
    error_process_surname,
    error_process_group,
    error_process_allocation_memory,
    no_error_process
};

enum error_process process_file(Student** guests,FILE* input, int n , double* all_sum) {
    char line[256];
    int count = 0;
    while (fgets(line, sizeof(line), input)) {

        char* words[10];
        int word_count = 0;

        char* token = strtok(line, " "); // Разбиваем строку на слова

        while (token != NULL && word_count < n) {
            words[word_count] = token;
            word_count++;
            token = strtok(NULL, " ");
        }
        double sum = 0.0;
        if (check_id_or_eval(words[0]) && word_count == n)
        {
            (*guests)[count].id = strtol(words[0],NULL,10);
            if (check_name(words[1]))
            {
                (*guests)[count].name = strdup(words[1]);
                if (check_surname(words[2]))
                {
                    (*guests)[count].surname = strdup(words[2]);
                    if (check_group(words[3]))
                    {
                        (*guests)[count].group = strdup(words[3]); 
                        (*guests)[count].evaluations = (char*)malloc(5*sizeof(char));
                        if ((*guests)[count].evaluations == NULL)
                        {
                            return error_process_allocation_memory;
                        }
                        for (int i = 0; i < 5; i++)
                        {
                            if (check_id_or_eval(words[i+4]))
                            {
                                (*guests)[count].evaluations[i] = words[i+4][0];
                                sum += words[i+4][0] - '0';
                            }
                            else
                            {
                                return error_process_eval;
                            }
                        }
                        sum = (double)sum / 5.0;
                        (*all_sum) += sum;
                        count++;     
                    }
                    else
                    {
                        return error_process_group;
                    }
                    
                }
                else
                {
                    return error_process_surname;
                }
            }
            else
            {
                return error_process_name;
            }
        }
        else
        {
            return error_process_id;
        }
    }
    (*all_sum) = (*all_sum) / (double)(count);
    return no_error_process;

}

const char* take_error_message(enum error_process check)
{
    switch (check)
    {
    case error_process_allocation_memory:
        return "Allocation memory error\n";
    case error_process_eval:
        return "Incorrect evalutions\n";
    case error_process_group:
        return "Incorrect group\n";
    case error_process_id:
        return "Incorrect id\n";
    case error_process_name:
        return "Incorrect name\n";
    case error_process_surname:
        return "Incorrect surname\n";
    default:
        return "Incorrect Error\n";
    }
}

int compare_id(Student* guest1, Student* guest2)
{
    if (guest1->id < guest2->id) 
    {
        return 1;
    } 
    else if (guest1->id > guest2->id) 
    {
        return -1;
    }
}

int compare_name(Student* guest1, Student* guest2)
{
    int name_compare = strcmp(guest1->name, guest2->name);
    if  (name_compare != 0) 
    {
        return name_compare;
    }
}

int compare_surname(Student* guest1, Student* guest2)
{
    int surname_compare = strcmp(guest1->surname, guest2->surname);
    if (surname_compare != 0) 
    {
        return surname_compare;
    }
}

int compare_group(Student* guest1, Student* guest2)
{
    int group_compare = strcmp(guest1->group, guest2->group);
    if (group_compare != 0) 
    {
        return group_compare;
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

enum error_find
{
    error_find_id,
    error_find_name,
    error_find_surname,
    error_find_group,
    no_error_find
};

enum error_find find_student(char* key, int** number_of_student, Student* guests, int count_student)
{
    char* words[3];
    int word_count = 0;

    char* token = strtok(key, " ");
    while (token != NULL && word_count < 2) {
        words[word_count] = token;
        word_count++;
        token = strtok(NULL, " ");
    }
    if (strcmp(words[0],"id") == 0)
    {
        if (check_id_or_eval(words[1]))
        {
            int num = strtol(words[1],NULL,10);
            for (int i = 0; i < count_student; i++)
            {
                if (guests[i].id == num)
                {
                    (*number_of_student)[i] = 1;
                }
                else
                {
                    (*number_of_student)[i] = 0;
                }
            }
        }
        else
        {
            return error_find_id;
        }
    }
    else if (strcmp(words[0],"name") == 0)
    {
        size_t len = strlen(words[1]);
        words[1][len-1] = '\0';
        if (check_name(words[1]))
        {
            for (int i = 0; i < count_student; i++)
            {
                if (strcmp(guests[i].name,words[1])== 0)
                {
                    (*number_of_student)[i] = 1;
                }
                else
                {
                    (*number_of_student)[i] = 0;
                }
            }
        }
        else
        {
            return error_find_name;
        }
    }
    else if (strcmp(words[0],"surname") == 0)
    {
        size_t len = strlen(words[1]);
        words[1][len-1] = '\0';
        if (check_surname(words[1]))
        {
            for (int i = 0; i < count_student; i++)
            {
                if (strcmp(guests[i].surname,words[1])== 0)
                {
                    (*number_of_student)[i] = 1;
                }
                else
                {
                    (*number_of_student)[i] = 0;
                }
            }
        }
        else
        {
            return error_find_surname;
        }
    }
    else if (strcmp(words[0],"group") == 0)
    {
        size_t len = strlen(words[1]);
        words[1][len-1] = '\0';
        if (check_group(words[1]))
        {
            for (int i = 0; i < count_student; i++)
            {
                if (strcmp(guests[i].group,words[1]) == 0)
                {
                    (*number_of_student)[i] = 1;
                }
                else
                {
                    (*number_of_student)[i] = 0;
                }
            }
        }
        else
        {
            return error_find_group;
        }
    }
    return no_error_find;
}

const char* take_error_message_find(enum error_find check)
{
    switch(check)
    {
        case error_find_group:
            return "Your intered group is incorrect\n";
        case error_find_id:
            return "Your intered id is incorrect\n";
        case error_find_name:
            return "Your intered name is incorrect\n";
        case error_find_surname:
            return "Your intered surname is incorrect\n";
        default:
            return "Incorrect error\n";
    }
}

int sort_student(char* key, Student** guests, int count_student)
{
    size_t len = strlen(key);
    if (strcmp(key,"id") == 0)
    {
        qsort((*guests), count_student, sizeof(Student), (int (*)(const void *, const void *))compare_id);
    }
    else if (strcmp(key,"name") == 0)
    {
        qsort((*guests), count_student, sizeof(Student), (int (*)(const void *, const void *))compare_name);
    }
    else if (strcmp(key,"surname") == 0)
    {
        qsort((*guests), count_student, sizeof(Student), (int (*)(const void *, const void *))compare_surname);
    }
    else if (strcmp(key,"group") == 0)
    {
        qsort((*guests), count_student, sizeof(Student), (int (*)(const void *, const void *))compare_group);
    }
    else
    {
        return 0;
    }
    return 1;
}

double average (unsigned char* score)
{
    double sum = 0;
    for (int i = 0; i < 5; i++)
    {
        sum += score[i] - '0';
    }
    return (double)sum / 5.0;
}

int main(int argc, char* argv[]) {
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
            input = fopen(argv[1],"r");
            output = fopen(argv[2],"w");
            int line_count = 0;
            double all_sum = 0.0;
            count_lines_in_file(input, &line_count);
            Student* guests = (Student*)malloc(sizeof(Student)*line_count);
            if (guests == NULL)
            {
                printf("Error allocation memory\n");
            }
            enum error_process check_p =process_file(&guests,input,9, &all_sum);
            if (check_p != no_error_process)
            {
                printf("Error mesage: %s", take_error_message(check_p));
                return 1;
            }
            while(1)
            {
                printf("Enter one of the commands: find or sort or exit or best_students\n");
                char input_str[257];
                input_str[256] = '\0';
                scanf("%257s", input_str);
                if (input_str[256] != '\0')
                {
                    printf("Error\n");
                    return 1;
                }
                if (strcmp(input_str, "find") == 0)
                {
                    int* number_of_student = (int*)malloc(sizeof(int)*line_count);
                    if (number_of_student == NULL)
                    {
                        printf("Error allocation memory\n");
                        for (int i = 0; i < line_count; i++)
                        {
                            free(guests[i].evaluations);
                        }
                        free(guests);
                        return 2;
                    }
                    printf("Enter the key to find by: id, name, surname, group\n");
                    printf("Example: 'id 1'; 'name John'; 'surname Naumov'; 'group M80-213B-22 or group 213';\n");
                    fflush(stdin);
                    char key[256];
                    if (!fgets(key, sizeof(key), stdin))
                    {
                        printf("Error\n");
                        return 1;
                    }
                    enum error_find check_f = find_student(strdup(key),&number_of_student,guests,line_count);
                    if (check_f != no_error_find)
                    {
                        printf("Error mesage: %s", take_error_message_find(check_f));
                        continue;
                    }
                    int temp_count = 0;
                    fprintf(output, "After use command 'find' %s", key);
                    for (int i = 0; i < line_count; i++)
                    {
                        if (number_of_student[i] == 1)
                        {
                            fprintf(output, "Guest %d: %d, %s, %s, %s, Average student score for exams: %f\n", i+1, guests[i].id, guests[i].name, guests[i].surname, guests[i].group, average(guests[i].evaluations));
                            temp_count++;
                        }
                        else if (i == line_count-1 && temp_count == 0)
                        {
                            fprintf(output, "Not a single student matching the key was found.\n");
                        }
                    }
                }
                else if (strcmp(input_str, "best_students") == 0)
                {
                    double eps = 0.0001;
                    fprintf(output, "After use command 'best_students' (Average result among all students: %f)\n", all_sum);
                    int temp_count = 0;
                    for (int i = 0; i < line_count; i++)
                    {
                        if ((average(guests[i].evaluations) - all_sum) >= eps)
                        {
                            fprintf(output, "Guest %d: %d, %s, %s, %s, Average student score for exams: %f\n", i+1, 
                            guests[i].id, guests[i].name, guests[i].surname, guests[i].group, average(guests[i].evaluations));
                            temp_count++;
                        }
                        else if (i == line_count-1 && temp_count == 0)
                        {
                            fprintf(output, "In the provided file, there were no students whose average score would be higher than the overall average score.\n");
                        }

                    }
                }
                else if (strcmp(input_str, "sort") == 0)
                {
                    fflush(stdin);
                    char key[257];
                    printf("Enter the key to sort: id, name, surname, group\n");
                    printf("Example: 'id'; 'name';...\n");
                    scanf("%257s",key);
                    if (strlen(key) == 257) 
                    {
                        printf("Error: Max alphas in key 256\n");
                        return 1;
                    }
                    if (sort_student(key, &guests, line_count))
                    {
                        fprintf(output, "After use command 'sort' %s\n", key);
                        for (int i = 0; i < line_count; i++)
                        {
                            fprintf(output, "Guest %d: %d, %s, %s, %s, Average student score for exams: %f\n", i+1, guests[i].id, guests[i].name, guests[i].surname, guests[i].group, average(guests[i].evaluations));
                        }
                    }
                    else
                    {
                        printf("Your key for sort is incorrect\n");
                    }
                }
                else if (strcmp(input_str, "exit") == 0)
                {
                    fclose(input);
                    fclose(output);
                    for (int i = 0; i < line_count; i++)
                    {
                        free(guests[i].evaluations);
                    }
                    free(guests);
                    printf("The program ended successfully\n");
                    return 0;
                }
            }
            break;
        }
}