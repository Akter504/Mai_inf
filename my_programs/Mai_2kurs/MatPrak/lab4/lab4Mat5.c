#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#define DEFOLT_LEN_STR 64

typedef struct Stack
{
    char* elem_stack;
    struct Stack* next;
}Stack;

typedef struct List
{
    char* elem_list;
    struct List* next;
}List;

typedef enum
{
    SIGNS_ERROR,
    NUM_ERROR,
    BRACKET_ERROR,
    INPUT_ERROR,
    no_error
}infix_check;

typedef enum create
{
    error_allocation_memory,
    create_no_error
}create_check;

typedef enum result
{
    error_type_overflow,
    error_result_allocation_memory,
    error_division_by_zero,
    error_exponentiation,
    result_no_error
}result_check;

typedef enum file_p
{
    error_file_allocation_memory,
    error_output_file,
    processing_no_error
}file_check;

void push_stack(Stack** stack, Stack* stack_part)
{
    if ((*stack) == NULL)
    {
        (*stack) = stack_part;
    }
    else
    {
        stack_part->next = (*stack);
        (*stack) = stack_part;
    }
}

void pop_stack(Stack** stack)
{
    Stack* temp = (*stack);
    (*stack) = (*stack)->next;
    free(temp); 
}

void free_stack(Stack** stack)
{
    Stack* temp = (*stack);
    Stack* current;
    while (temp != NULL)
    {
        if (temp->elem_stack != NULL)
        {
            free(temp->elem_stack);
        }
        current = temp->next;
        free(temp);
        temp = current;        
    }
}

List* push_list(List** list, List* part_list)
{
    if (*list == NULL)
    {
        (*list) = part_list;
    }
    else
    {
        List* temp = (*list);
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = part_list;
    }
}

void free_list(List** list)
{
    List* temp = (*list);
    List* current;
    while (temp != NULL)
    {
        if (temp->elem_list != NULL)
        {
            free(temp->elem_list);
        }
        current = temp->next;
        free(temp);
        temp = current;
    }
}

infix_check checking_infix_expression(char* infix_exp, int size_str)
{
    
    int i = 0;
    int count_open_brakets = 0;
    int count_close_brakets = 0;
    while (infix_exp[i+1] != '\0')
    {
        if (infix_exp[i] == ')')
        {
            i++;
            count_close_brakets++;
            if (!(infix_exp[i] == '-' || infix_exp[i] == '*' 
            ||infix_exp[i] == '%' || infix_exp[i] == '/' || infix_exp[i] == '+'
            ||infix_exp[i] == '^'))
            {
                return SIGNS_ERROR;
            }
            continue;
        }
        if (count_close_brakets > count_open_brakets)
        {
            return BRACKET_ERROR;
        }
        if (infix_exp[i] == '(')
        {
            i++;
            count_open_brakets++;
            if (infix_exp[i] != '-' && !isdigit(infix_exp[i]))
            {
                return NUM_ERROR;
            }
            continue;
        }
        if (isdigit(infix_exp[i]))
        {
            if ((!isdigit(infix_exp[i-1]) && infix_exp[i] == '0' && isdigit(infix_exp[i+1])) && ((i > 1) && (i < size_str)))
            {
                return NUM_ERROR;
            }
            else if (i == 0 && infix_exp[i] == '0' && isdigit(infix_exp[i+1]))
            {
                return NUM_ERROR;
            }
            i++;
            if (!isdigit(infix_exp[i]) && infix_exp[i] != ')' && 
            !(infix_exp[i] == '-' || infix_exp[i] == '*' || infix_exp[i] == '%' 
            || infix_exp[i] == '/' || infix_exp[i] == '+'
            ||infix_exp[i] == '^' || infix_exp[i] == ')'))
            {
                return SIGNS_ERROR;
            }
            if (infix_exp[i+1] == '\0' && (infix_exp[i] == '-' || infix_exp[i] == '*' || infix_exp[i] == '%' 
            || infix_exp[i] == '/' || infix_exp[i] == '+'
            ||infix_exp[i] == '^'))
            {
                return SIGNS_ERROR;
            }
            continue;
        }
        if (infix_exp[i] == '-' || infix_exp[i] == '*' 
        ||infix_exp[i] == '%' || infix_exp[i] == '/' || infix_exp[i] == '+'
        ||infix_exp[i] == '^')
        {
            if ((i == 0) && (infix_exp[i+1] == '(' || infix_exp[i] != '-'))
            {
                return SIGNS_ERROR;
            }
            i++;
            if (infix_exp[i] != '(' && !isdigit(infix_exp[i]))
            {
                if (i != 1 && infix_exp[i] == '-' && isdigit(infix_exp[i+1]))
                {
                    continue;
                }
                else if (infix_exp[i] == ')')
                {
                    return BRACKET_ERROR;
                }
                else
                {
                    return NUM_ERROR;
                }
            }
            continue;
        }
        if (infix_exp[i] != ')' && infix_exp[i] != '(' && infix_exp[i] != '+' && infix_exp[i] != '*' 
        && infix_exp[i] != '/' && infix_exp[i] != '-' && infix_exp[i] != '%' && infix_exp[i] != '^' && !isdigit(infix_exp[i]))
        {
            return INPUT_ERROR;
        }
    }
    if ((i == 0) && infix_exp[i] != ')' && infix_exp[i] != '(' && infix_exp[i] != '+' 
    && infix_exp[i] != '*' && infix_exp[i] != '/' && infix_exp[i] != '-' && infix_exp[i] != '%' 
    && infix_exp[i] != '^' && !isdigit(infix_exp[i]))
    {
        return INPUT_ERROR;
    }
    if (infix_exp[i] == ')')
    {
        count_close_brakets++;
    }
    if (count_close_brakets != count_open_brakets)
    {
        return BRACKET_ERROR;
    }
    return no_error;
}

void print_list(List* postfix)
{
    List* temp = postfix;
    while (temp != NULL)
    {
        printf("elem_list: %s\n", temp->elem_list);
        temp = temp->next;
    }
}

create_check postfix_create(char* infix_expression, List** postfix_expression)
{
    Stack* temp_stack = NULL;
    Stack* stack_part;
    List* postfix_part;
    size_t len_infix = strlen(infix_expression);
    int i = 0;
    while (infix_expression[i] != '\0')
    {
        if (isdigit(infix_expression[i]) || (i == 0 && infix_expression[i] == '-') 
        || ((infix_expression[i-1] == '*' || infix_expression[i-1] == '+' || infix_expression[i-1] == '-' || infix_expression[i-1] == '/' 
        || infix_expression[i-1] == '^' || infix_expression[i-1] == '%') && infix_expression[i] == '-'))
        {
            postfix_part = (List*)malloc(sizeof(List));
            if (postfix_part == NULL)
            {
                free_list(postfix_expression);
                free_stack(&temp_stack);
                return error_allocation_memory;
            }
            postfix_part->elem_list = (char*)malloc(sizeof(char)*len_infix);
            if (postfix_part->elem_list == NULL)
            {
                free_list(postfix_expression);
                free_stack(&temp_stack);
                return error_allocation_memory;
            }
            postfix_part->next = NULL;
            int j = 0;
            if (((infix_expression[i-1] == '*' || infix_expression[i-1] == '+' || infix_expression[i-1] == '-' 
            || infix_expression[i-1] == '/' || infix_expression[i-1] == '^' || infix_expression[i-1] == '%') 
            && infix_expression[i] == '-') || (i == 0 && infix_expression[i] == '-'))
            {
                postfix_part->elem_list[j] = '-';
                j++;
                i++;
            }
            while (isdigit(infix_expression[i]))
            {
                postfix_part->elem_list[j] = infix_expression[i];
                j++;
                i++;
            }
            postfix_part->elem_list[j] = '\0';
            push_list(postfix_expression,postfix_part);
        }
        else if (infix_expression[i] == '(')
        {
            stack_part = (Stack*)malloc(sizeof(Stack));
            if (stack_part == NULL)
            {
                free_list(postfix_expression);
                free_stack(&temp_stack);
                return error_allocation_memory;
            }
            stack_part->elem_stack = (char*)malloc(sizeof(char)*2);
            if (stack_part->elem_stack == NULL)
            {
                free_list(postfix_expression);
                free_stack(&temp_stack);
                return error_allocation_memory;
            }
            stack_part->elem_stack[0] = '(';
            stack_part->elem_stack[1] = '\0';
            stack_part->next = NULL;
            push_stack(&temp_stack, stack_part);
            i++;
        }
        else if (infix_expression[i] == '+' || infix_expression[i] == '-')
        {
            stack_part = (Stack*)malloc(sizeof(Stack));
            if (stack_part == NULL)
            {
                free_list(postfix_expression);
                free_stack(&temp_stack);
                return error_allocation_memory;
            }
            stack_part->elem_stack = (char*)malloc(sizeof(char)*2);
            if (stack_part->elem_stack == NULL)
            {
                free_list(postfix_expression);
                free_stack(&temp_stack);
                return error_allocation_memory;
            }
            stack_part->elem_stack[1] = '\0';
            if (infix_expression[i] == '+')
            {
                stack_part->elem_stack[0] = '+';
            }
            else
            {
                stack_part->elem_stack[0] = '-';
            }
            stack_part->next = NULL;
            if (temp_stack != NULL)
            {
                if (temp_stack->elem_stack[0] == '(')
                {
                    push_stack(&temp_stack,stack_part);
                }
                else
                {
                    while(temp_stack != NULL && temp_stack->elem_stack[0] != '(')
                    {
                        postfix_part = (List*)malloc(sizeof(List));
                        if (postfix_part == NULL)
                        {
                            free_list(postfix_expression);
                            free_stack(&temp_stack);
                            return error_allocation_memory;
                        }
                        postfix_part->elem_list = (char*)malloc(sizeof(char)*2);
                        if (postfix_part->elem_list == NULL)
                        {
                            free_list(postfix_expression);
                            free_stack(&temp_stack);
                            return error_allocation_memory;
                        }
                        postfix_part->elem_list[0] = temp_stack->elem_stack[0];
                        postfix_part->elem_list[1] = '\0';
                        postfix_part->next = NULL;
                        push_list(postfix_expression,postfix_part);
                        pop_stack(&temp_stack);
                    }
                    push_stack(&temp_stack,stack_part);
                }
            }
            else
            {
                push_stack(&temp_stack,stack_part);
            }
            i++;
        }
        else if (infix_expression[i] == '%')
        {
            stack_part = (Stack*)malloc(sizeof(Stack));
            if (stack_part == NULL)
            {
                free_list(postfix_expression);
                free_stack(&temp_stack);
                return error_allocation_memory;
            }
            stack_part->elem_stack = (char*)malloc(sizeof(char)*2);
            if (stack_part->elem_stack == NULL)
            {
                free_list(postfix_expression);
                free_stack(&temp_stack);
                return error_allocation_memory;
            }
            stack_part->elem_stack[0] = '%';
            stack_part->elem_stack[1] = '\0';
            stack_part->next = NULL;
            if (temp_stack != NULL)
            {
                if (temp_stack->elem_stack[0] == '(')
                {
                    push_stack(&temp_stack,stack_part);
                }
                else
                {
                    while(temp_stack != NULL
                    && temp_stack->elem_stack[0] != '(' && temp_stack->elem_stack[0] != '+' && temp_stack->elem_stack[0] != '-')
                    {
                        postfix_part = (List*)malloc(sizeof(List));
                        if (postfix_part == NULL)
                        {
                            free_list(postfix_expression);
                            free_stack(&temp_stack);
                            return error_allocation_memory;
                        }
                        postfix_part->elem_list = (char*)malloc(sizeof(char)*2);
                        if (postfix_part->elem_list == NULL)
                        {
                            free_list(postfix_expression);
                            free_stack(&temp_stack);
                            return error_allocation_memory;
                        }
                        postfix_part->elem_list[0] = temp_stack->elem_stack[0];
                        postfix_part->elem_list[1] = '\0';
                        postfix_part->next = NULL;
                        push_list(postfix_expression,postfix_part);
                        pop_stack(&temp_stack);
                    }
                    push_stack(&temp_stack,stack_part);
                }
            }
            else
            {
                push_stack(&temp_stack,stack_part);
            }
            i++;
        }
        else if (infix_expression[i] == '*' || infix_expression[i] == '/')
        {
            stack_part = (Stack*)malloc(sizeof(Stack));
            if (stack_part == NULL)
            {
                free_list(postfix_expression);
                free_stack(&temp_stack);
                return error_allocation_memory;
            }
            stack_part->elem_stack = (char*)malloc(sizeof(char)*2);
            if (stack_part->elem_stack == NULL)
            {
                free_list(postfix_expression);
                free_stack(&temp_stack);
                return error_allocation_memory;
            }
            stack_part->elem_stack[1] = '\0';
            if (infix_expression[i] == '*')
            {
                stack_part->elem_stack[0] = '*';
            }
            else
            {
                stack_part->elem_stack[0] = '/';
            }
            stack_part->next = NULL;
            if (temp_stack != NULL)
            {
                if (temp_stack->elem_stack[0] == '(')
                {
                    push_stack(&temp_stack,stack_part);
                }
                else
                {
                    while(temp_stack != NULL
                    && temp_stack->elem_stack[0] != '(' && temp_stack->elem_stack[0] != '+' && temp_stack->elem_stack[0] != '-' 
                    && temp_stack->elem_stack[0] != '%')
                    {
                        postfix_part = (List*)malloc(sizeof(List));
                        if (postfix_part == NULL)
                        {
                            free_list(postfix_expression);
                            free_stack(&temp_stack);
                            return error_allocation_memory;
                        }
                        postfix_part->elem_list = (char*)malloc(sizeof(char)*2);
                        if (postfix_part->elem_list == NULL)
                        {
                            free_list(postfix_expression);
                            free_stack(&temp_stack);
                            return error_allocation_memory;
                        }
                        postfix_part->elem_list[0] = temp_stack->elem_stack[0];
                        postfix_part->elem_list[1] = '\0';
                        postfix_part->next = NULL;
                        push_list(postfix_expression,postfix_part);
                        pop_stack(&temp_stack);
                    }
                    push_stack(&temp_stack,stack_part);
                }
            }
            else
            {
                push_stack(&temp_stack,stack_part);
            }
            i++;
        }
        else if (infix_expression[i] == '^')
        {
            stack_part = (Stack*)malloc(sizeof(Stack));
            if (stack_part == NULL)
            {
                free_list(postfix_expression);
                free_stack(&temp_stack);
                return error_allocation_memory;
            }
            stack_part->elem_stack = (char*)malloc(sizeof(char)*2);
            if (stack_part->elem_stack == NULL)
            {
                free_list(postfix_expression);
                free_stack(&temp_stack);
                return error_allocation_memory;
            }
            stack_part->elem_stack[0] = '^';
            stack_part->elem_stack[1] = '\0';
            stack_part->next = NULL;
            if (temp_stack != NULL)
            {
                if (temp_stack->elem_stack[0] == '(')
                {
                    push_stack(&temp_stack,stack_part);
                }
                else
                {
                    while(temp_stack != NULL
                    && temp_stack->elem_stack[0] != '(' && temp_stack->elem_stack[0] != '+' && temp_stack->elem_stack[0] != '-' 
                    && temp_stack->elem_stack[0] != '%' && temp_stack->elem_stack[0] != '*' && temp_stack->elem_stack[0] != '/')
                    {
                        postfix_part = (List*)malloc(sizeof(List));
                        if (postfix_part == NULL)
                        {
                            free_list(postfix_expression);
                            free_stack(&temp_stack);
                            return error_allocation_memory;
                        }
                        postfix_part->elem_list = (char*)malloc(sizeof(char)*2);
                        if (postfix_part->elem_list == NULL)
                        {
                            free_list(postfix_expression);
                            free_stack(&temp_stack);
                            return error_allocation_memory;
                        }
                        postfix_part->elem_list[0] = temp_stack->elem_stack[0];
                        postfix_part->elem_list[1] = '\0';
                        postfix_part->next = NULL;
                        push_list(postfix_expression,postfix_part);
                        pop_stack(&temp_stack);
                    }
                    push_stack(&temp_stack,stack_part);
                }
            }
            else
            {
                push_stack(&temp_stack,stack_part);
            }
            i++;
        }
        else if (infix_expression[i] == ')')
        {
            while(temp_stack != NULL && temp_stack->elem_stack[0] != '(')
            {
                postfix_part = (List*)malloc(sizeof(List));
                if (postfix_part == NULL)
                {
                    free_list(postfix_expression);
                    free_stack(&temp_stack);
                    return error_allocation_memory;
                }
                postfix_part->elem_list = (char*)malloc(sizeof(char)*2);
                if (postfix_part->elem_list == NULL)
                {
                    free_list(postfix_expression);
                    free_stack(&temp_stack);
                    return error_allocation_memory;
                }
                postfix_part->elem_list[0] = temp_stack->elem_stack[0];
                postfix_part->elem_list[1] = '\0';
                postfix_part->next = NULL;
                push_list(postfix_expression,postfix_part);
                pop_stack(&temp_stack);
            }
            if (temp_stack != NULL && temp_stack->elem_stack[0] == '(')
            {
                pop_stack(&temp_stack);
            }
            i++;
        }
    }
    while (temp_stack != NULL)
    {
        postfix_part = (List*)malloc(sizeof(List));
        if (postfix_part == NULL)
        {
            free_list(postfix_expression);
            free_stack(&temp_stack);
            return error_allocation_memory;
        }
        postfix_part->elem_list = (char*)malloc(sizeof(char)*2);
        if (postfix_part->elem_list == NULL)
        {
            free_list(postfix_expression);
            free_stack(&temp_stack);
            return error_allocation_memory;
        }
        postfix_part->elem_list[0] = temp_stack->elem_stack[0];
        postfix_part->elem_list[1] = '\0';
        postfix_part->next = NULL;
        push_list(postfix_expression,postfix_part);
        pop_stack(&temp_stack);
    }
    return create_no_error;
}

int pow_f(int num1, int num2)
{
    int res = 1;
    if (num1 == 0)
    {
        return 0;
    }
    for (int i = 0; i < num2 ; i++)
    {
        res *= num1;
    }
    return res;
}

result_check result_of_postfix_exp(List* postfix_expression, char** result)
{
    Stack* result_stack = NULL;
    List* temp = postfix_expression;
    while (temp != NULL)
    {
        size_t len_list_elem = strlen(temp->elem_list);
        Stack* part_stack = (Stack*)malloc(sizeof(Stack));
        if (part_stack == NULL)
        {
            free_stack(&result_stack);
            return error_result_allocation_memory;
        }
        part_stack->next = NULL;
        part_stack->elem_stack = (char*)malloc(sizeof(char)*len_list_elem+1);
        if (part_stack->elem_stack == NULL)
        {
            free_stack(&result_stack);
            return error_result_allocation_memory;
        }
        int j = 0;
        if (isdigit(temp->elem_list[0]) || (temp->elem_list[0] == '-' && isdigit(temp->elem_list[1])))
        {
            if (temp->elem_list[0] == '-' && isdigit(temp->elem_list[1]))
            {
                part_stack->elem_stack[j] = temp->elem_list[j];
                j++;
            }
            while (isdigit(temp->elem_list[j]))
            {
                part_stack->elem_stack[j] = temp->elem_list[j];
                j++;
            }
            part_stack->elem_stack[j] = '\0';
            push_stack(&result_stack,part_stack);
        }
        else
        {
            char* endptr;
            long long num1 = strtoll(result_stack->elem_stack,&endptr,10);
            if ((errno == ERANGE && (num1 == LLONG_MAX || num1 == LLONG_MIN)) || (errno != 0 && num1 == 0)) {
                free_stack(&result_stack);
                return error_type_overflow;
            }
            pop_stack(&result_stack);
            long long num2 = strtoll(result_stack->elem_stack,&endptr,10);
            if ((errno == ERANGE && (num2 == LLONG_MAX || num2 == LLONG_MIN)) || (errno != 0 && num2 == 0)) {
                free_stack(&result_stack);
                return error_type_overflow;
            }
            free(result_stack->elem_stack);
            if (temp->elem_list[0] == '+')
            {
                num2 +=num1;
            }
            else if (temp->elem_list[0] == '-')
            {
                num2 -= num1;
            }
            else if (temp->elem_list[0] == '%')
            {
                if (num1 != 0)
                {
                    num2 %= num1;
                }
            }
            else if (temp->elem_list[0] == '/')
            {
                if (num1 != 0)
                {
                   num2 /= num1; 
                }
                else
                {
                    free_stack(&result_stack);
                    return error_division_by_zero;
                }
            }
            else if (temp->elem_list[0] == '*')
            {
                num2 *= num1;
            }
            else if (temp->elem_list[0] == '^')
            {
                if (((num1 == 0 && num2 != 0) || (num1 != 0 && num2 == 0) || (num1 != 0 && num2 != 0)) && (num1 >= 0))
                {
                    num2 = pow_f(num2,num1);
                }
                else
                {
                    free_stack(&result_stack);
                    return error_exponentiation;
                }
                
            }
            if ((errno == ERANGE && (num2 == LLONG_MAX || num2 == LLONG_MIN)) || (errno != 0 && num2 == 0)) {
                free_stack(&result_stack);
                return error_type_overflow;
            }
            size_t length_num = snprintf(NULL, 0, "%lld", num2);
            result_stack->elem_stack = (char*)malloc((length_num + 1) * sizeof(char));
            if (result_stack->elem_stack == NULL)
            {
                free_stack(&result_stack);
                return error_result_allocation_memory;
            }
            snprintf(result_stack->elem_stack, length_num + 1, "%lld", num2);
        }
        temp = temp->next;
    }
    (*result) = (char*)malloc(sizeof(char)*strlen(result_stack->elem_stack));
    if ((*result) == NULL)
    {
        free_stack(&result_stack);
        return error_result_allocation_memory;
    }
    strcpy((*result),result_stack->elem_stack);
    free_stack(&result_stack);
    return result_no_error;
}

const char* get_error_infix_exp(infix_check error)
{
    switch (error)
    {
        case SIGNS_ERROR:
            return "In this expression, if there is a problem with the signs(sign)\n";
        case NUM_ERROR:
            return "In this expression, if there is a problem with the nums(num)\n";
        case BRACKET_ERROR:
            return "In this expression, if there is a problem with the brackets(bracket)\n";
        case INPUT_ERROR:
            return "The expression was entered incorrectly\n";
        default:
            return "Undefined error\n";
    }
}

const char* get_error_result_pfx(result_check error)
{
    switch(error)
    {
        case error_result_allocation_memory:
            return "Allocation memory in result postfix.\n";
        case error_division_by_zero:
            return "You can't divide by zero.\n";
        case error_exponentiation:
            return "0 to the power of 0 is undefined, the degree must be positive and integer.\n";
        case error_type_overflow:
            return "There was an overflow of the long long data type, enter smaller numbers.\n";
        default:
            return "Undefined error\n";
    }
}

void output_processing(FILE* output, infix_check check_inf, char* infix_exp, char* name_of_file, int current_str)
{
    List* postfix_expression = NULL;
    char* result;
    fprintf(output, "Current string[%d]\n", current_str);
    fprintf(output, "Infix expression: %s\n", infix_exp);
    if (check_inf == no_error)
    {
        create_check check_postfix = postfix_create(infix_exp,&postfix_expression);
        if (check_postfix == create_no_error)
        {
            fprintf(output, "Postfix expression: ");
            List* current;
            result_check check_result_postfix = result_of_postfix_exp(postfix_expression, &result);
            while (postfix_expression != NULL)
            {
                fprintf(output, "%s", postfix_expression->elem_list);
                current = postfix_expression->next;
                free(postfix_expression->elem_list);
                postfix_expression = current;
            }
            free(postfix_expression);
            fprintf(output, "\n");
            if (check_result_postfix == result_no_error)
            {
                fprintf(output, "Postfix result: %s\n\n", result);
            }
            else
            {
                fprintf(output, "Error: %s\n\n", get_error_result_pfx(check_result_postfix));
            }
        }
        else
        {
            fprintf(output, "Error: allocation memory in create postfix\n\n");
        }
    }
    else
    {
        fprintf(output, "Error: %s\n\n", get_error_infix_exp(check_inf));
    }
}

file_check file_processing(FILE* input, char* name_of_file, FILE* output)
{
    int c;
    int count_str_in_file = 0;
    char* infix_expression = (char*)malloc(sizeof(char)*DEFOLT_LEN_STR);
    if (infix_expression == NULL)
    {
        return error_file_allocation_memory;
    }
    int current_len_infix_exp = 0;
    char* temp;
    int max_len_str = DEFOLT_LEN_STR;
    fprintf(output, "!Name of file: %s\n", name_of_file);
    while (1)
    {
        if ((c = fgetc(input)) != EOF)
        {
            infix_expression[current_len_infix_exp] = c;
            current_len_infix_exp++;
            if (current_len_infix_exp == (max_len_str - 1))
            {
                max_len_str = max_len_str*2+1;
                temp = (char*)realloc(infix_expression,sizeof(char)*max_len_str);
                if (temp == NULL)
                {
                    free(infix_expression);
                    return error_file_allocation_memory;
                }
                infix_expression = temp;
            }
        }




        if (c == '\n' || c == EOF)
        {
            count_str_in_file++;
            if (c == EOF)
            {
                infix_expression[current_len_infix_exp] = '\0';
            }
            else
            {
                infix_expression[current_len_infix_exp-1] = '\0';
            }
            infix_check checker = checking_infix_expression(infix_expression,current_len_infix_exp);

            output_processing(output,checker,infix_expression,name_of_file,count_str_in_file);

            free(infix_expression);
            infix_expression = (char*)malloc(sizeof(char)*DEFOLT_LEN_STR);
            if (infix_expression == NULL)
            {
                return error_file_allocation_memory;
            }
            max_len_str = DEFOLT_LEN_STR;
            current_len_infix_exp = 0;
            if (c == EOF)
            {
                break;
            }
        }
    }
    free(infix_expression);
    return processing_no_error;
}

int main(int argc, char* argv[])
{
    int num_of_file = 1;
    int flag_first_open = 0;
    for (int i = 1; i < argc; i++)
    {
        FILE* input = fopen(argv[i], "r");
        if (input == NULL)
        {
            printf("Can`t open file number: %d", num_of_file);
            num_of_file++;
        }
        else
        {
            FILE* output;
            if (!flag_first_open)
            {
                output = fopen("Answers.txt","w");
                if (output == NULL)
                {
                    printf("Can`t open output file\n");
                    return 1;
                }
                if (output == input)
                {
                    printf("The output and input files are the same\n");
                    return 1;
                }
                flag_first_open = 1;
            }
            else
            {
                output = fopen("Answers.txt","a");
                if (output == NULL)
                {
                    printf("Can`t open output file\n");
                    return 1;
                }
                if (output == input)
                {
                    printf("The output and input files are the same\n");
                    return 1;
                }
            }
            file_check check = file_processing(input, argv[i], output);
            if (check == error_file_allocation_memory)
            {
                printf("Error: allocation memory in processing file\n");
                return 1;
            }
            fclose(input);
            fclose(output);
        }
    }
}
