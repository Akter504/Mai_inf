#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_NAME_LENGTH 50

enum error {
    error_invalid_none,
    error_invalid_input,
    error_no_empty,
    error_invalid_last_name,
    error_invalid_first_name,
    error_invalid_middle_name,
    error_invalid_birth_day,
    error_invalid_birth_month,
    error_invalid_birth_year,
    error_invalid_gender,
    error_invalid_income,
    error_invalid_name_length,
    error_invalid_birth_date
};

struct Liver {
    char last_name[MAX_NAME_LENGTH + 1];
    char first_name[MAX_NAME_LENGTH + 1];
    char middle_name[MAX_NAME_LENGTH + 1];
    int birth_day;
    int birth_month;
    int birth_year;
    char gender;
    float income;
    struct Liver* next;
};

typedef struct Stack {
    struct Liver data;
    struct Liver* changed_liver;
    struct Stack* next;
} Stack;

void push(Stack** stack, struct Liver* data) {
    Stack* new_node = (Stack*)malloc(sizeof(Stack));
    if (new_node == NULL) {
        fprintf(stderr, "Error allocation memory\n");
        exit(EXIT_FAILURE); //
    }
    new_node->data = (*data);
    new_node->changed_liver = data;
    new_node->next = *stack;
    *stack = new_node;
}

struct Liver pop(Stack** stack) {
    if (*stack == NULL) {
        fprintf(stderr, "Stack empty\n");
        exit(EXIT_FAILURE);//
    }

    Stack* temp = *stack;
    struct Liver data = temp->data;
    *stack = temp->next;
    free(temp);

    return data;
}

enum error validate_data(const char* last_name, const char* first_name, const char* middle_name, const char* birth_day_str, const char* birth_month_str, const char* birth_year_str, const char* gender, const char* income) {
    size_t len_last = strlen(last_name);
    for (size_t i = 0; i < len_last; i++) {
        if (!isalpha(last_name[i])) {
            return error_invalid_last_name;
        }
    }

    size_t len_first = strlen(first_name);
    for (size_t i = 0; i < len_first; i++) {
        if (!isalpha(first_name[i])) {
            return error_invalid_first_name;
        }
    }

    size_t len_middle = strlen(middle_name);
    for (size_t i = 0; i < strlen(middle_name); i++) {
        if (!isalpha(middle_name[i])) {
            return error_invalid_middle_name;
        }
    }

    char* endptr;
    int birth_day = strtol(birth_day_str, &endptr, 10);
    if (*endptr != '\0') {
        return error_invalid_birth_day;
    }

    int birth_month = strtol(birth_month_str, &endptr, 10);
    if (*endptr != '\0') {
        return error_invalid_birth_month;
    }

    int birth_year = strtol(birth_year_str, NULL, 10);
    if (*endptr != '\0') {
        return error_invalid_birth_year;
    }
    strtof(income, &endptr);
    if (*endptr != '\0') {
        return error_invalid_income;
    }
    
    if (strlen(last_name) == 0 || strlen(first_name) == 0 || strlen(last_name) > MAX_NAME_LENGTH || strlen(first_name) > MAX_NAME_LENGTH || strlen(middle_name) > MAX_NAME_LENGTH) {
        return error_invalid_name_length;
    }

    if (birth_day < 1 || birth_day > 31 || birth_month < 1 || birth_month > 12 || birth_year < 1900) {
        return error_invalid_birth_date;
    }

    if (gender[0] != 'M' && gender[0] != 'W') {
        return error_invalid_gender;
    }

    return error_invalid_none;
}

void insert_sorted(struct Liver** head, struct Liver* new_liver) {
    struct Liver* current;
    if (*head == NULL || (*head)->birth_year > new_liver->birth_year || ((*head)->birth_year == new_liver->birth_year && (*head)->birth_month > new_liver->birth_month) || ((*head)->birth_year == new_liver->birth_year && (*head)->birth_month == new_liver->birth_month && (*head)->birth_day > new_liver->birth_day)) {
        new_liver->next = *head;
        *head = new_liver;
    } else {
        current = *head;
        while (current->next != NULL && (current->next->birth_year < new_liver->birth_year || (current->next->birth_year == new_liver->birth_year && current->next->birth_month < new_liver->birth_month) || (current->next->birth_year == new_liver->birth_year && current->next->birth_month == new_liver->birth_month && current->next->birth_day < new_liver->birth_day))) {
            current = current->next;
        }
        new_liver->next = current->next;
        current->next = new_liver;
    }
}

void print_list(struct Liver* head) {
    struct Liver* current = head;
    while (current != NULL) {
        printf("Name: %s %s %s\n", current->last_name, current->first_name, current->middle_name);
        printf("Birth Date: %d-%d-%d\n", current->birth_day, current->birth_month, current->birth_year);
        printf("Gender: %c\n", current->gender);
        printf("Income: %.2f\n", current->income);
        printf("\n");
        current = current->next;
    }
}

const char* get_error_message(enum error err) {
    switch (err) {
        case error_invalid_last_name:
            return "Invalid character in last name";
        case error_invalid_first_name:
            return "Invalid character in first name";
        case error_invalid_middle_name:
            return "Invalid character in middle name";
        case error_invalid_birth_day:
            return "Invalid birth day";
        case error_invalid_birth_month:
            return "Invalid birth month";
        case error_invalid_birth_year:
            return "Invalid birth year";
        case error_invalid_income:
            return "Invalid income";
        case error_invalid_name_length:
            return "Invalid name or last name length";
        case error_invalid_birth_date:
            return "Invalid birth date";
        case error_invalid_gender:
            return "Invalid gender";
        case error_no_empty:
            return "Are you trying to add without deleting the old data";
        case error_invalid_input:
            return "Input in incorrect, use an example";
        default:
            return "Unknown error";
    }
}

struct Liver* fill(FILE* file, struct Liver* head)
{
    char last_name[MAX_NAME_LENGTH + 1];
    char first_name[MAX_NAME_LENGTH + 1];
    char middle_name[MAX_NAME_LENGTH + 1];
    char birth_day_str[50];
    char birth_month_str[50];
    char birth_year_str[50];
    char gender[50];
    char income_str[50];
    struct Liver* new_liver;
    char line[256];

    int birth_day;
    int birth_month;
    int birth_year;
    float income;
    while (fgets(line, sizeof(line), file) != NULL) {
        int result = sscanf(line, "%49s %49s %49s %49s %49s %49s %49s %49s",
               last_name, first_name, middle_name,
               birth_day_str, birth_month_str, birth_year_str,
               gender, income_str);
        new_liver = (struct Liver*)malloc(sizeof(struct Liver));
        if (result != 8)
        {
            enum error validation_result = validate_data(last_name, first_name, "\0",
                                                middle_name, birth_day_str, birth_month_str,
                                                birth_year_str, gender);
            if (validation_result != error_invalid_none) {
                printf("Error: %s\n", get_error_message(validation_result));
                continue;
            }
            birth_day = strtol(middle_name, NULL, 10);
            birth_month = strtol(birth_day_str, NULL, 10);
            birth_year = strtol(birth_month_str, NULL, 10);
            income = strtof(gender, NULL);
            gender[0] = birth_year_str[0];
            strcpy(new_liver->middle_name, "");
        }
        else
        {
            enum error validation_result = validate_data(last_name, first_name, middle_name,
                                                birth_day_str, birth_month_str, birth_year_str,
                                                gender, income_str);
            if (validation_result != error_invalid_none) {
                printf("Error: %s\n", get_error_message(validation_result));
                continue;
            }
            birth_day = strtol(birth_day_str, NULL, 10);
            birth_month = strtol(birth_month_str, NULL, 10);
            birth_year = strtol(birth_year_str, NULL, 10);
            income = strtof(income_str, NULL);
            strcpy(new_liver->middle_name, middle_name);
        }

        strcpy(new_liver->last_name, last_name);
        strcpy(new_liver->first_name, first_name);
        new_liver->birth_day = birth_day;
        new_liver->birth_month = birth_month;
        new_liver->birth_year = birth_year;
        new_liver->gender = gender[0];
        new_liver->income = income;

        new_liver->next = NULL;
        insert_sorted(&head, new_liver);
    }
    return head;
}

enum error_find
{
    error_input,
    error_found,
    no_error
};

enum error_find find(struct Liver* head, char* line, struct Liver** result_head)
{
    char* words[17];
    int word_count = 0;
    int count_correct;

    char* token = strtok(line, " ");
    while (token != NULL && word_count < 17) {
        words[word_count] = token;
        word_count++;
        token = strtok(NULL, " ");
    }
    if (word_count < 2 || (word_count % 2) != 0)
    {
        free(line);
        return error_input;
    }
    while (head != NULL)
    {
        count_correct = 0;
        for (int i = 0; i < word_count; i = i+2)
        {
            if (strcmp(words[i], "name") == 0)
            {
                if (strcmp(head->first_name, words[i+1]) == 0)
                {
                    count_correct++;
                }
            }
            else if(strcmp(words[i], "surname") == 0)
            {
                if (strcmp(head->last_name, words[i+1])== 0)
                {
                    count_correct++;
                }
            }
            else if(strcmp(words[i], "partronymic") == 0)
            {
                if (strcmp(head->middle_name, words[i+1])== 0)
                {
                    count_correct++;
                }
            }
            else if(strcmp(words[i], "day_of_birth") == 0)
            {
                if (head->birth_day == strtol(words[i+1], NULL,10))
                {
                    count_correct++;
                }
            }
            else if(strcmp(words[i], "month_of_birth") == 0)
            {
                if (head->birth_month== strtol(words[i+1], NULL,10))
                {
                    count_correct++;
                }
            }
            else if(strcmp(words[i], "year_of_birth") == 0)
            {
                if (head->birth_year== strtol(words[i+1], NULL,10))
                {
                    count_correct++;
                }
            }
            else if(strcmp(words[i], "gender") == 0)
            {
                if (head->gender == words[i+1][0])
                {
                    count_correct++;
                }
            }
            else if(strcmp(words[i], "income") == 0)
            {
                double eps = 0.0001;
                if (fabs(head->income-strtof(words[i+1], NULL)) <= eps)
                {
                    count_correct++;
                }
            }
        }
        if (count_correct == (word_count / 2))
        {
            struct Liver* new_liver = (struct Liver*)malloc(sizeof(struct Liver));
            *new_liver = *(head);
            insert_sorted(&(*result_head), new_liver);
        }
        head = head->next;
    }
    free(line);
    return no_error;
}

enum error_find change_info(struct Liver* head, char* line, struct Liver** current)
{
    char* words[9];
    int word_count = 0;

    char* token = strtok(line, " ");
    while (token != NULL && word_count < 17) {
        words[word_count] = token;
        word_count++;
        token = strtok(NULL, " ");
    }
    int flag = 1;
    if (word_count < 8)
    {
        if (word_count == 7)
        {
            flag = 0;
        }
        free(line);
        return error_input;
    }
    if (strcmp(words[0], "find") == 0)
    {
        while(head != NULL)
        {

            if (strcmp(head->last_name, words[1]) == 0 && strcmp(head->first_name, words[2]) == 0 && strcmp(head->middle_name, words[3]) == 0
            && (head->birth_day == strtol(words[4],NULL,10)) && (head->birth_month == strtol(words[5],NULL,10)) && (head->birth_year == strtol(words[6],NULL,10))
            && (head->gender==words[7][0]) && flag)
            {
                (*current) = head;
                free(line);
                return no_error;
            }
            else if (strcmp(head->last_name, words[1]) == 0 && strcmp(head->first_name, words[2]) == 0 && strcmp(head->middle_name, "") == 0
            && (head->birth_day == strtol(words[3],NULL,10)) && (head->birth_month == strtol(words[4],NULL,10)) && (head->birth_year == strtol(words[5],NULL,10))
            && (head->gender==words[6][0]) && !flag)
            {
                (*current) = head;
                free(line);
                return no_error;
            }
            else
            {
                head = head->next;
            }
        }
        free(line);
        return error_found;
    }
    
}

int check_name_surname_middlename(char* name)
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


enum error add_or_remove_or_change(struct Liver** result_head, Stack** stack, char* info)
{
    push(stack, *result_head);
    char* words[4];
    int word_count = 0;
    int flag = 0;
    char* token = strtok(info, " ");
    while (token != NULL && word_count < 3) {
        words[word_count] = token;
        word_count++;
        token = strtok(NULL, " ");
    }
    if (strcmp(words[0], "add") == 0 && word_count == 3)
    {
        size_t len = strlen(words[2]);
        words[2][len-1] = '\0';
        if (strcmp(words[1], "name") == 0)
        {
            if (check_name_surname_middlename(words[2]))
            {
                if (strcmp((*result_head)->first_name,"\0") ==0)
                {
                    strcpy((*result_head)->first_name,words[2]);
                }
                else
                {
                    free(info);
                    return error_no_empty;
                }
            }
            else
            {
                free(info);
                return error_invalid_first_name;
            }
        }
        else if(strcmp(words[1], "surname") == 0)
        {
            if (check_name_surname_middlename(words[2]))
            {
                if (strlen((*result_head)->last_name) == 0)
                {
                    strcpy((*result_head)->last_name,words[2]);
                }
                else
                {
                    free(info);
                    return error_no_empty;
                }
            }
            else
            {
                free(info);
                return error_invalid_last_name;
            }
        }
        else if(strcmp(words[1], "partronymic") == 0)
        {
            if (check_name_surname_middlename(words[2]))
            {
                if (strcmp((*result_head)->middle_name,"\0") ==0)
                {
                    strcpy((*result_head)->middle_name,words[2]);
                }
                else
                {
                    free(info);
                    return error_no_empty;
                }

            }
            else
            {
                free(info);
                return error_invalid_middle_name;
            }
        }
        else if(strcmp(words[1], "day_of_birth") == 0)
        {
            char* endptr;
            int birth_day = strtol(words[2], &endptr, 10);
            if (*endptr != '\0') {
                free(info);
                return error_invalid_birth_day;
            }
            if ((*result_head)->birth_day == -1)
            {
                (*result_head)->birth_day = birth_day;
            }
            else
            {
                free(info);
                return error_no_empty;
            }
        }
        else if(strcmp(words[1], "month_of_birth") == 0)
        {
            char* endptr;
            int birth_month = strtol(words[2], &endptr, 10);
            if (*endptr != '\0') {
                free(info);
                return error_invalid_birth_month;
            }
            if ((*result_head)->birth_month == -1)
            {
                (*result_head)->birth_month = birth_month;
            }
            else
            {
                free(info);
                return error_no_empty;
            }
        }
        else if(strcmp(words[1], "year_of_birth") == 0)
        {
            char* endptr;
            int birth_year = strtol(words[2], &endptr, 10);
            if (*endptr != '\0') {
                free(info);
                return error_invalid_birth_year;
            }
            if ((*result_head)->birth_year == -1)
            {
                (*result_head)->birth_year = birth_year;
            }
            else
            {
                free(info);
                return error_no_empty;
            }
        }
        else if(strcmp(words[1], "gender") == 0)
        {
            if (words[2][0] == 'M' || words[2][0] == 'W')
            {
                if ((*result_head)->gender = '\0')
                {
                    (*result_head)->gender = words[2][0];
                }
                else
                {
                    free(info);
                    return error_no_empty;
                }
            }
            else
            {
                free(info);
                return error_invalid_gender;
            }
        }
        else if(strcmp(words[1], "income") == 0)
        {
            char* endptr;
            float income = strtof(words[2], &endptr);
            if (*endptr != '\0') {
                free(info);
                return error_invalid_income;
            }
            float eps = 0.0001;
            if (fabs(fabs((*result_head)->income) - 1.0) <= eps)
            {
                (*result_head)->income = income;
            }
        }
        else
        {
            free(info);
            return error_invalid_input;
        }
    }
    else if (strcmp(words[0], "remove") == 0 && word_count == 2) //
    {
        size_t len = strlen(words[1]);
        words[1][len-1] = '\0';
        if (strcmp(words[1], "name") == 0)
        {
            strcpy((*result_head)->first_name,"\0");
        }
        else if(strcmp(words[1], "surname") == 0)
        {
            strcpy((*result_head)->last_name,"\0");
        }
        else if(strcmp(words[1], "partronymic") == 0)
        {
            strcpy((*result_head)->middle_name,"\0");
        }
        else if(strcmp(words[1], "day_of_birth") == 0)
        {
            (*result_head)->birth_day = -1;
        }
        else if(strcmp(words[1], "month_of_birth") == 0)
        {
            (*result_head)->birth_month = -1;
        }
        else if(strcmp(words[1], "year_of_birth") == 0)
        {
            (*result_head)->birth_year = -1;
        }
        else if(strcmp(words[1], "gender") == 0)
        {
            (*result_head)->gender = '\0';
        }
        else if(strcmp(words[1], "income") == 0)
        {
            (*result_head)->income = -1.0;
        }
        else
        {
            free(info);
            return error_invalid_input;
        }
    }
    else if (strcmp(words[0], "change") == 0 && word_count == 3) //
    {
        size_t len = strlen(words[2]);
        words[2][len-1] = '\0';
        printf("\n%s %s %s\n", words[0], words[1], words[2]);
        if (strcmp(words[1], "name") == 0)
        {
            if (check_name_surname_middlename(words[2]))
            {
                strcpy((*result_head)->first_name, words[2]);
            }
            else
            {
                free(info);
                return error_invalid_first_name;
            }
        }
        else if(strcmp(words[1], "surname") == 0)
        {
            if (check_name_surname_middlename(words[2]))
            {
                strcpy((*result_head)->last_name, words[2]);
            }
            else
            {
                free(info);
                return error_invalid_last_name;
            }
        }
        else if(strcmp(words[1], "partronymic") == 0)
        {
            if (check_name_surname_middlename(words[2]))
            {
                strcpy((*result_head)->middle_name, words[2]);
            }
            else
            {
                free(info);
                return error_invalid_middle_name;
            }
        }
        else if(strcmp(words[1], "day_of_birth") == 0)
        {
            char* endptr;
            int birth_day = strtol(words[2], &endptr, 10);
            if (*endptr != '\0') {
                free(info);
                return error_invalid_birth_day;
            }
            (*result_head)->birth_day = birth_day;
            
        }
        else if(strcmp(words[1], "month_of_birth") == 0)
        {
            char* endptr;
            int birth_month = strtol(words[2], &endptr, 10);
            if (*endptr != '\0') {
                free(info);
                return error_invalid_birth_month;
            }
            (*result_head)->birth_month = birth_month;
        }
        else if(strcmp(words[1], "year_of_birth") == 0)
        {
            char* endptr;
            int birth_year = strtol(words[2], &endptr, 10);
            if (*endptr != '\0') {
                free(info);
                return error_invalid_birth_year;
            }
            (*result_head)->birth_year = birth_year;
        }
        else if(strcmp(words[1], "gender") == 0)
        {
            if (words[2][0] == 'M' || words[2][0] == 'W')
            {
                (*result_head)->gender = words[2][0];
            }
            else
            {
                free(info);
                return error_invalid_gender;
            }
        }
        else if(strcmp(words[1], "income") == 0)
        {
            char* endptr;
            float income = strtof(words[2], &endptr);
            if (*endptr != '\0') {
                free(info);
                return error_invalid_income;
            }
            (*result_head)->income = income;
        }
        else
        {
            free(info);
            return error_invalid_input;
        }
    }
    else
    {
        free(info);
        return error_invalid_input;
    }
    free(info);
    return error_invalid_none;

}

int main(int argc, char* argv[]) {
    if (argc != 2)
    {
        printf("Error\n");
        return 1;
    }
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    struct Liver* head = NULL;
    head = fill(file,head);
    fclose(file);
    int counter =  0;
    int count_turns = 0;
    Stack* stack_undo = NULL;
    while(1)
    {
        int counter = 0;
        char command[15];
        printf("Use command: find or change_info or exit or undo or print\n");
        scanf("%15s",command);
        if (strcmp(command, "find") == 0)
        {
            printf("Example1: name Alexander partronymic Olegovich surname Maryan income 500000\n");
            printf("Example2: day_of_birth 20 gender M\n");
            printf("Example3: day_of_birth 20 day_of_month 6 surname Zyk\n");
            char line[257];
            fflush(stdin);
            if (!fgets(line,sizeof(line),stdin))
            {
                printf("Error\n");
                return 1;
            }
            size_t len_line = strlen(line);
            line[len_line-1] = '\0';
            struct Liver* result_head = NULL;
            if (find(head,strdup(line),&result_head) == no_error)
            {
                print_list(result_head);
                while (result_head != NULL) {
                    struct Liver* temp = result_head;
                    result_head = result_head->next;
                    free(temp);
                }
            }
        }
        else if (strcmp(command, "change_info") == 0)
        {
            printf("To use this command enter: find Surname,Name,Partronymic(or no),day,month,year,gender\n");
            char line[257];
            fflush(stdin);
            struct Liver* result_head = NULL;
            if (!fgets(line,sizeof(line),stdin))
            {
                printf("Error\n");
                return 1;
            }
            enum error_find check_change_info = change_info(head,strdup(line), &result_head);
            if (check_change_info == error_input)
            {
                printf("Input is wrong\n");
            }
            else if (check_change_info == error_found)
            {
                printf("Not a single resident was found\n"); 
            }
            else if (check_change_info == no_error)
            {
                printf("\nA user with such data was found\n");
                printf("To add information, use: add partronymic Olegovich, add name Alexander\n");
                printf("To remove information, use: remove partronymic, remove name\n");
                printf("To change information, use: change name Alex, change surname Mar\n");
                printf("Information can be added if the old one is deleted\n");
                printf("Use 'cancel' to back\n");
                while(1)
                {
                    char info[257];
                    if (!fgets(info,sizeof(info),stdin))
                    {
                        printf("Error\n");
                        return 1;
                    }
                    if (strcmp(info,"cancel\n") == 0)
                    {
                        break;
                    }
                    enum error check_change_info = add_or_remove_or_change(&result_head, &stack_undo, strdup(info));
                    if (check_change_info != error_invalid_none)
                    {
                        printf("%s\n", get_error_message(check_change_info));
                    }
                    else
                    {
                        count_turns++;
                        print_list(head);
                    }
                }
            }
        }
        else if (strcmp(command, "undo") == 0)
        {
            if (stack_undo != NULL && (count_turns / 2) > 0)
            {
                (*stack_undo->changed_liver) = stack_undo->data;
                pop(&stack_undo);
                count_turns--;
                print_list(head);
            }
            else{
                printf("You can`t use undo, because your count modification < n/2\n");
            }
        }
        else if (strcmp(command, "print") == 0)
        {
            struct Liver* current = head;
            printf("Write the file name or path to the file where the response will be displayed\n");
            char name_of_file[257];
            fflush(stdin);
            if (!fgets(name_of_file,sizeof(name_of_file),stdin))
            {
                printf("Error\n");
                return 1;
            }
            size_t len_name = strlen(name_of_file);
            name_of_file[len_name-1] = '\0'; 
            FILE* output = fopen(name_of_file,"w");
            if (output == NULL)
            {
                return 1;
            }
            while (current != NULL) {
                fprintf(output,"Name: %s %s %s\n", current->last_name, current->first_name, current->middle_name);
                fprintf(output,"Birth Date: %d-%d-%d\n", current->birth_day, current->birth_month, current->birth_year);
                fprintf(output,"Gender: %c\n", current->gender);
                fprintf(output,"Income: %.2f\n", current->income);
                fprintf(output,"\n");
                current = current->next;
            }
            fclose(output);
        }
        else if (strcmp(command, "exit") == 0)
        {
            printf("Exit succesful\n");
            while (head != NULL) {
                struct Liver* temp = head;
                head = head->next;
                free(temp);
            }
            while (stack_undo->next != NULL)
            {
                Stack* temp;
                temp = stack_undo->next;
                free(stack_undo);
                stack_undo = temp;
            }
            free(stack_undo);
            return 0;
        }

    }

}
