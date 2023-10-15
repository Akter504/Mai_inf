#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#define MAX_RECORDS 100

void tollower(char* str)
{
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++)
    {
        if ((str[i] >= 'A') && (str[i] <='Z'))
        {
            str[i] = str[i] - 'A' + 'a';
        }
    }
}

int check_pin_int(const char* str) {
    char* endptr;
    strtol(str, &endptr, 10);
    return *endptr == '\0';
}

int find_user_pin(FILE* users, char* login, char* pin_code)
{
    char line[256];
    char stored_login[15];
    char stored_pin[15];
    int found = 0;

    while (fgets(line, sizeof(line), users) != NULL) {
        if (sscanf(line, "%s %s", stored_login, stored_pin) == 2) {
            if (strcmp(stored_login, login) == 0 && strcmp(stored_pin, pin_code) == 0) {
                found = 1;
                return found;
            }
        }
    }

    return found;
}

int correct_login(char* login)
{
    size_t len = strlen(login);
    for (size_t i = 0; i < len; i++)
    {
        if (!isdigit(login[i]) && !isalpha(login[i]))
        {
            return 0;
        }
    }
    return 1;
}

int is_login_taken(FILE* users, const char* login)
{
    char line[256];
    while (fgets(line, sizeof(line), users) != NULL) {
        char stored_login[7];
        if (sscanf(line, "%s", stored_login) == 1) {
            if (strcmp(stored_login, login) == 0) {
                return 1;
            }
        }
    }

    return 0;
}

int authorization(char *login)
{
    int find = 0;
    int count_errors = 3;
    char pin_code[7];
    while(find == 0 && count_errors != 0){
            printf("Authorization:\n");
            printf("You have %d attempts to write your login and pincode correctly\n", count_errors);
            printf("Login: ");
            scanf("%7s",login);
            fflush(stdin);
            if (strlen(login) > 6) 
            {
                printf("Error: Login is too long! The login can contain no more than 6 letters.\n");
            }
            else
            {
                if (correct_login(login))
                {
                    FILE* users = fopen("users.txt","r");
                    if (users == NULL)
                    {
                    printf("Error: No users.\n");
                    exit(1);
                    }
                    printf("PIN-code: ");
                    scanf("%7s",pin_code);
                    fflush(stdin);
                    if (strlen(pin_code) > 6) 
                    {
                        printf("Error: PIN-code error! The PIN code can contain a number of not more than 100000 and not less than 0.\n");
                    }
                    else{    
                        if (check_pin_int(pin_code))
                        {
                            find = find_user_pin(users, login, pin_code);
                            fclose(users);
                            if (!find)
                            {
                                printf("Your login or PIN-code are incorrect try again\n");
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            fclose(users);
                            printf("Error: The PIN code contains incorrect characters.\n");
                        }
                    }
                }
            }
            count_errors--;
    }
    if (find)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int push_user_pin(FILE* users, char* login, char* pin_code)
{
    fseek(users, 0, SEEK_END);
    if (ftell(users) > 0) 
    {
        fprintf(users, "\n");
    }
    fprintf(users, "%s %s", login, pin_code);

    return 1;
}

int registration()
{
    int find = 0;
    char login[7];
    char pin_code[7];
    int check_login = 1;
    while (check_login == 1){
        printf("Registration\n");
        printf("Login: ");
        scanf("%7s",login);
        fflush(stdin);
        if (strlen(login) > 6) 
        {
            printf("Error: Login is too long! The login can contain no more than 6 letters.\n");
        }
        else
        {
            if (correct_login(login))
            {
                FILE* users = fopen("users.txt","a+");
                if (users == NULL)
                {
                printf("Error: Failed to create file.\n");
                exit(1);
                }
                check_login = is_login_taken(users,login);
                if (check_login)
                {
                    printf("The username you entered is already occupied, try another one.\n");
                    fclose(users);
                }
                else
                {
                    printf("PIN-code: ");
                    scanf("%7s",pin_code);
                    fflush(stdin);
                    if (strlen(pin_code) > 6) 
                    {
                        printf("Error: PIN-code error! The PIN code can contain a number of not more than 100000 and not less than 0.\n");
                    }
                    else{    
                        if (check_pin_int(pin_code))
                        {
                            find = push_user_pin(users, login, pin_code);
                            fclose(users);
                            if (!find)
                            {
                                printf("Your login or PIN-code are incorrect try again\n");
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            fclose(users);
                            printf("Error: The PIN code contains incorrect characters.\n");
                        }
                    }
                }
            }
            else
            {
                printf("Write correct login\n");
            }
        }
    }
}

void time_func()
{
   long int ttime = time (NULL);
   char times[40];

   strcpy(times,ctime(&ttime));
   for (int i = 11; i < 19; i++){
       printf("%c",times[i]);
   }
   printf("\n");
}

void date_func()
{
    time_t now;
    time(&now);
    int day, month, year;
    struct tm *local = localtime(&now);
    day = local->tm_mday;
    month = local->tm_mon + 1;
    year = local->tm_year + 1900;
    printf("%d:%d:%d", day, month, year);
    printf("\n");

    
}

int is_leap_year(int year) 
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int days_in_month(int month, int year) 
{
    int days[] = {0 , 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && is_leap_year(year)) {
        return 29;
    }
    return days[month];
}

int calculate_date_difference(int day, int month, int year, int day_input, int month_input, int year_input, char flag) 
{
    int day_difference = 0;
    int sign = 1;

    if (year_input < year || (year_input == year && (month_input < month || (month_input == month && day_input < day)))) 
    {
        int temp_year = year_input;
        year_input = year;
        year = temp_year;

        int temp_day = day_input;
        day_input = day;
        day = temp_day;

        int temp_month = month_input;
        month_input = month;
        month = temp_month;

        sign = -1;
    }

    while (year != year_input || month != month_input || day != day_input) 
    {
        day_difference++;
        day++;

        if (day > days_in_month(month, year)) 
        {
            day = 1;
            month++;

            if (month > 12) {
                month = 1;
                year++;
            }
        }
    }

    int leap_year_difference = (year_input - year) / 4;
    day_difference -= leap_year_difference;

    if (flag == 's')
    {
        return sign*day_difference*86400;
    }
    else if (flag == 'm')
    {
        return sign*day_difference*1440;
    }
    else if (flag == 'h')
    {
        return sign*day_difference*24;
    }
    else if (flag == 'y')
    {
        return (sign*day_difference) / 365;
    }
}

int howmuch_ans(char *flag, int day_input, int month_input, int year_input)
{
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    int day = local->tm_mday;
    int month = local->tm_mon + 1;
    int year = local->tm_year + 1900;
    if (flag[0] == '-' && (flag[1] == 's' || flag[1] == 'm' || flag[1] == 'h' || flag[1] == 'y') && strlen(flag) == 3)
    {
        return calculate_date_difference(day, month, year, day_input, month_input, year_input, flag[1]);
    }
    else
    {
        return 0;
    }
}

void howmuch_func(char* buffer)
{
    char buffer_copy[256];
    strcpy(buffer_copy,buffer);
    char *strings[3];
    char *token = strtok(buffer_copy, " ");
    int i = 0;
    for (i = 0; i < 3; i++) 
    {
        if (token == NULL) 
        {
            break;
        }
        strings[i] = (char *)malloc(strlen(token) + 1);
        strcpy(strings[i], token); 
        token = strtok(NULL, " "); 
    }
    if (i != 3)
    {
        printf("Incorrect input format: Howmuch <time> flag.\n");
    }
    else if (strlen(strings[1]) > 11)
    {
        printf("Incorrect len data: dd:mm:yy.\n");
    }
    else
    {
        size_t len = strlen(strings[1]);
        int count_point = 0;
        int count_digit = 0;
        int day_inp = 0;
        int month_inp = 0;
        int year_inp = 0;
        for (size_t i = 0; i < len; i++)
        {
            if (isdigit(strings[1][i]))
            {
                if (i < 2)
                {
                    day_inp = day_inp*10 + (strings[1][i] - '0');
                }
                else if(i > 2 && i < 5)
                {
                    month_inp = month_inp*10 + (strings[1][i] - '0');
                }
                else if(i > 5 && i < 10)
                {
                    year_inp = year_inp*10 + (strings[1][i] - '0');
                }
                count_digit++;
            }
            else
            {
                if (strings[1][i] == ':' && (i == 2 || i == 5))
                {
                    count_point++;
                }
            }
        }
        if (count_digit == 8 && count_point == 2)
        {
            int res;
            res = howmuch_ans(strings[2],day_inp,month_inp,year_inp);
            if (!res)
            {
                printf("Incorrect flag\n");
            }
            else
            {
                printf("%d\n", res);
            }
        }
        else
        {
            printf("Incorrect data. Date input format: dd:mm:yy.\n");
        }
    }
    for (int j = 0; j < i; j++)
    {
        free(strings[j]);
    }
}

struct UserRecord {
    char login[7];
    int number;
    int totalRecords;
};

struct UserRecord userRecords[MAX_RECORDS];

void push_sanctions(struct UserRecord *records, const char *login, const char* number) {
    int i;
    int num = atoi(number);
    for (i = 0; i < records->totalRecords; i++) 
    {
        if (strcmp(records[i].login, login) == 0) 
        {
            records[i].number = num;
            return;
        }
    }

    if (records->totalRecords < MAX_RECORDS)
    {
        strcpy(records[records->totalRecords].login, login);
        records[records->totalRecords].number = num;
        records->totalRecords++;
    } 
    else 
    {
        printf("Достигнут максимальный лимит записей.\n");
    }
}

void sanctions_func(char* buffer)
{
    char buffer_copy[256];
    strcpy(buffer_copy,buffer);
    char *strings[3];
    char *token = strtok(buffer_copy, " ");
    int i = 0;
    for (i = 0; i < 3; i++) 
    {
        if (token == NULL) 
        {
            break;
        }
        strings[i] = (char *)malloc(strlen(token) + 1);
        strcpy(strings[i], token); 
        token = strtok(NULL, " "); 
    }
    FILE* users = fopen("users.txt","r");
    if (users == NULL)
    {
        perror("Error open file in sanctions_func\n");
        exit(1);
    }
    else
    {
        if (i != 3)
        {
            printf("Incorrect input format: Sanctions username <number>.\n");
            fclose(users);
        }
        else if (is_login_taken(users, strings[1]))
        {
            fclose(users);
            size_t len_num = strlen(strings[2]);
            strings[2][len_num-1] = '\0';
            if (check_pin_int(strings[2]))
            {
                if (atoi(strings[2]) >= 0)
                {
                    printf("Enter the administrator password to use this command:\n");
                    int password;
                    scanf("%d",&password);
                    if (password == 12345)
                    {
                        printf("Succes\n");
                        push_sanctions(userRecords, strings[1], strings[2]);
                    }
                    else
                    {
                        printf("The passport you entered turned out to be incorrect\n");
                    }
                }
                else
                {
                    printf("The number must not be negative\n");
                }
            }
            else
            {
                printf("Error not int number.\n");
            }
        }
        else
        {
            printf("User not found.\n");
        }
    }
}

void commands(int *logout_flag,const char *login) {
    int flag_exit = 0;
    int user_limit = -1;
    int count = 0;

    for (int i = 0; i < MAX_RECORDS; i++)
    {
        if (strcmp(userRecords[i].login, login) == 0)
        {
            user_limit = userRecords[i].number;
            break;
        }
    }
    printf("Available command:\nTime.\nDate.\nHowmuch <time(dd:mm:yy)> flag.\nSanctions username <number>.\nLogout.\nExit.\n");


    if (user_limit > -1) 
    {
        printf("You are a limited user with a limit of %d commands.\n", user_limit);
    }
    while ((*logout_flag) != 1 && flag_exit != 1) 
    {
        if (user_limit > -1 && count >= user_limit) 
        {
            printf("You have reached the command usage limit (%d commands).\n", user_limit);
            printf("Bye-bye\n");
            (*logout_flag) = 1;
            break;
        }
        printf("Inter the function:\n");
        char buffer[256];
        fflush(stdin);
        fgets(buffer, sizeof(buffer), stdin);
        fflush(stdin);
        char current[256];
        if (sscanf(buffer, "%255s", current) == 1) 
        {
            tollower(current);
            if (strlen(current) == 256) 
            {
                printf("Input incorrect\n");
            } 
            else 
            {
                if (strcmp(current, "time") == 0) 
                {
                    time_func();
                    count++;
                } 
                else if (strcmp(current, "date") == 0) 
                {
                    date_func();
                    count++;
                } 
                else if (strcmp(current, "logout") == 0) 
                {
                    (*logout_flag) = 1;
                } 
                else if (strcmp(current, "exit") == 0) 
                {
                    (*logout_flag) = 0;
                    flag_exit = 1;
                } 
                else if (strcmp(current, "howmuch") == 0) 
                {
                    howmuch_func(buffer);
                    count++;
                } 
                else if (strcmp(current, "sanctions") == 0) 
                {
                    sanctions_func(buffer);
                    count++;
                } 
                else 
                {
                    printf("Unknown command.\n");
                }
            }
        }
    }
}

int main() 
{
    int logout_flag = 1;
    while(1)
    {
        if (logout_flag == 1)
        {
            printf("Hello, do you have an account?\n");
            printf("print: Yes or No or Exit\n");
            char ans[5];
            int find = 0;
            scanf("%4s", ans);
            fflush(stdin);
            tollower(ans);
            if (strcmp(ans,"yes") == 0)
            {
                char login[7];
                find = authorization(login);
                if (find)
                {
                    logout_flag = 0;
                    printf("Succes\n");
                    commands(&logout_flag, login);
                }
                else{
                    printf("Lose\n");
                }
            }
            else if (strcmp(ans,"no") == 0)
            {
                find = registration();
                if (find)
                {
                    printf("Succes\n");
                }
                else
                {
                    printf("Lose\n");
                }

            }
            else if(strcmp(ans,"exit") == 0)
            {
                return 0;
            }
        }
        else
        {
            break;
        }
    }
}
