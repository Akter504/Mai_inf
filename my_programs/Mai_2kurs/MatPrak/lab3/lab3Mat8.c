#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node {
    int coefficient;
    int degree;
    struct Node* next;
}Node;

void free_nodes(Node *head) {
    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        free(temp);
    }
}

void print_node(Node* head) {
    Node* current = head;
    while (current != NULL) {
        printf("Coef %d\n", current->coefficient);
        printf("degree %d\n", current->degree);
        printf("\n");
        current = current->next;
    }
}

enum errors_check_input_line
{
    error_input_polynomial,
    error_input_command,
    error_input_incorrect,
    error_incorrect_sequence,
    error_input_allocation_memory,
    error_input_command_eval,
    error_input_command_diff,
    no_input_error,
    multiline_comment
};

enum errors_check_input_line check_input_line(char* line, int* flag_check_multiline_comment,char** found_command, char** found_polyn)
{
    char command[5];
    char polyn[128];
    char comment[128];
    char* correct_command[8] = {"Add", "Div", "Sub","Mult","Mod","Eval","Diff","Cmps"};
    if (!(*flag_check_multiline_comment))
    {
        int result = sscanf(line, "%[^(](%[^);]);%s", command, polyn, comment);
        if (comment[0] == '[')
        {
            (*flag_check_multiline_comment) = 1;
        }
        if (result == 3 || result == 2)
        {
            int flag_find_command = 0;
            int flag_eval = 0;
            int flag_diff = 0;
            int flag_commas = 0;
            if (strcmp(command, "Eval") == 0)
                {
                    flag_eval = 1;
                }
            else if (strcmp(command, "Diff") == 0)
            {
                flag_diff = 1;
            }
            for (int i = 0; i < 8; i++)
            {
                if (strcmp(command,correct_command[i]) == 0)
                {
                    (*found_command) = correct_command[i];
                    flag_find_command = 1;
                    size_t len_polyn = strlen(polyn);
                    int j;
                    if (polyn[0] == '-')
                    {
                        j = 1;
                    }
                    else if (polyn[0] == 'x' || isdigit(polyn[0]))
                    {
                        j = 0;
                    }
                    else
                    {
                        return error_input_incorrect;
                    }
                    char remember[8] = "9999999\0";
                    while (j < len_polyn)
                    {
                        if (flag_diff && flag_commas)
                        {
                            return error_input_command_diff;
                        }
                        while (isdigit(polyn[j]))
                        {
                            j++;
                        }
                        if (polyn[j] == 'x')
                        {
                            j++;
                            if (polyn[j] == '^')
                            {
                                j++;
                                if (isdigit(polyn[j]))
                                {
                                    int count_digit = 0;
                                    char temp[8];
                                    while (isdigit(polyn[j]))
                                    {
                                        if (count_digit == 8)
                                        {
                                            return error_input_command;
                                        }
                                        temp[count_digit] = polyn[j];
                                        count_digit++;
                                        j++;
                                    }
                                    temp[count_digit] = '\0';

                                    if (polyn[j] == '+' || polyn[j] == '-')
                                    {
                                        if (atoi(remember) > atoi(temp))
                                        {
                                            strcpy(remember, temp);
                                        }
                                        else
                                        {
                                            return error_incorrect_sequence;
                                        }
                                        j++;
                                    }
                                    else if (polyn[j] == ',' && polyn[j+1] != '\0' && !flag_commas)
                                    {
                                        flag_commas = 1;
                                        if (flag_eval)
                                        {
                                            if (polyn[j+1] == '-')
                                            {
                                                j++;
                                            }
                                            while (isdigit(polyn[j+1]))
                                            {
                                                j++;
                                            }
                                            if (polyn[j+1] != '\0')
                                            {
                                                return error_input_command_eval;
                                            }
                                        }
                                        if (atoi(remember) > atoi(temp))
                                        {
                                            strcpy(remember, "9999999\0");
                                        }
                                        else
                                        {
                                            return error_incorrect_sequence;
                                        }
                                        j++;
                                        if (polyn[j] == '-')
                                        {
                                            j++;
                                        }
                                    }
                                    else if (polyn[j] == '\0')
                                    {
                                        if (atoi(remember) <= atoi(temp))
                                        {
                                            return error_incorrect_sequence;
                                        }
                                        if (flag_eval && !flag_commas)
                                        {
                                            return error_input_command_eval;
                                        }
                                    }
                                    else
                                    {
                                        return error_input_polynomial;
                                    }
                                }
                                else
                                {
                                    return error_input_polynomial;
                                }
                            }
                            else if (polyn[j-1] != '^' && (polyn[j] == '\0' || (polyn[j] == ',' && !flag_commas) || 
                            (polyn[j] == '+' && polyn[j+1] != '\0') || (polyn[j] == '-'&& polyn[j+1] != '\0')))
                            {
                                if (polyn[j] == '\0' && flag_eval && !flag_commas)
                                {
                                    return error_input_command_eval;
                                }
                                if (polyn[j] == ',')
                                {
                                    flag_commas = 1;
                                }
                                if (atoi(remember) <= 1)
                                {
                                    return error_incorrect_sequence;
                                }
                                else
                                {
                                    strcpy(remember,"1\0");
                                }
                                if (flag_eval && polyn[j] == ',')
                                {
                                    if (polyn[j+1] == '-')
                                    {
                                        j++;
                                    }
                                    while (isdigit(polyn[j+1]))
                                    {
                                        j++;
                                    }
                                    if (polyn[j+1] != '\0')
                                    {
                                        return error_input_command_eval;
                                    }
                                }
                                if (polyn[j] == ',' && polyn[j+1] == '-')
                                {
                                    strcpy(remember, "9999999\0");
                                    j = j+2;
                                }
                                else if (polyn[j] == ',' && polyn[j+1] == '\0')
                                {
                                    return error_input_polynomial;
                                }
                                else if (polyn[j] == ',' && polyn[j+1] == 'x')
                                {
                                    strcpy(remember, "9999999\0");
                                    j++;
                                }
                                else if((polyn[j] == '+' && polyn[j+1] != '\0') || (polyn[j] == '-'&& polyn[j+1] != '\0'))
                                {
                                    j++;
                                }
                            }
                            else
                            {
                                return error_input_polynomial;
                            }
                        }
                        else if (polyn[j-1] != 'x' && (polyn[j] == '\0' || (polyn[j] == ',' && !flag_commas) 
                        && (polyn[j-1] != '-' && polyn[j-1] != '+')))
                        {
                            if (polyn[j] == ',')
                            {
                                flag_commas = 1;
                            }
                            if (atoi(remember) <= 0 && flag_commas)
                            {
                                return error_incorrect_sequence;
                            }
                            else
                            {
                                strcpy(remember,"9999999\0");
                            }
                            if (flag_eval && polyn[j] == ',')
                            {
                                if (polyn[j+1] == '-')
                                {
                                    j++;
                                }
                                while (isdigit(polyn[j+1]))
                                {
                                    j++;
                                }
                                if (polyn[j+1] != '\0')
                                {
                                    return error_input_command_eval;
                                }
                            }
                            if (polyn[j] == ',' && polyn[j+1] == '-')
                            {
                                strcpy(remember, "9999999\0");
                                j = j+2;
                            }
                            else if (polyn[j] == ',' && polyn[j+1] != '-')
                            {
                                strcpy(remember, "9999999\0");
                                j++;
                            }
                            else if (polyn[j] == ',' && polyn[j+1] == '\0')
                            {
                                return error_input_polynomial;
                            }
                        }
                        else
                        {
                            return error_input_polynomial;
                        }
                    }
                }
            }
            if (!flag_find_command)
            {
                return error_input_command;
            }
            printf("%s %s %s\n", command, polyn, comment);
            (*found_polyn) = (char*)malloc(sizeof(char)*strlen(polyn));
            if (*found_polyn == NULL)
            {
                return error_input_allocation_memory;
            }
            strcpy(*found_polyn,polyn);
            return no_input_error;
        }
        else
        {
            return error_input_incorrect;
        }
    }
    else
    {
        char *ptr = strchr(line, ']');
        if (ptr != NULL && ptr == (line + strlen(line) - 2)) 
        {
            (*flag_check_multiline_comment) = 0;
            return multiline_comment;
        }
        return multiline_comment;
    }
}

char const* get_error_message_input(enum errors_check_input_line check)
{
    switch (check)
    {
    case error_input_command:
        return "The entered command does not correspond to any command for the polynomial\n";
    case error_input_incorrect:
        return "The command in the file is entered incorrectly\n";
    case error_input_polynomial:
        return "The polynomial is entered incorrectly\n";
    case error_incorrect_sequence:
        return "The sequence of degrees of the polynomial is incorrect\n";
    case error_input_command_eval:
        return "Error when using the eval command\n";
    case error_input_command_diff:
        return "Error when using the diff command\n";
    default:
        return "Another error\n";
    }
}

enum error_fill_nodes
{
    error_fill_allocation_memory,
    no_fill_error
};

void push_node(Node** polynomial, Node* new_polynomial)
{
    if ((*polynomial) == NULL)
    {
        (*polynomial) = new_polynomial;
    }
    else
    {
        Node* temp = (*polynomial);
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = new_polynomial;
    }
}

enum error_fill_nodes fill_nodes(Node** polynomial_before_comma, Node** polynomial_after_comma, char* polyn, int* flag_check_comma)
{
    (*flag_check_comma) = 0;
    char *start = polyn;
    char *end;
    while (*start) {
        Node* new_polynomial = (Node*)malloc(sizeof(Node));
        if (new_polynomial == NULL)
        {
            return error_fill_allocation_memory;
        }
        int coefficient = strtol(start, &end, 10);
        if (coefficient == 0 && end == start) {
            if (*start == '-')
            {
                end++;
                coefficient = -1;
            }
            else
            {
                if (*end == '+' || *end == '-')
                {
                    end++;
                }
                coefficient = 1;
            }
        }
        int degree = 0;
        if (*end == 'x') {
            if (*(end + 1) == '^') {
                degree = strtol(end +2, &end, 10);
            } else {
                degree = 1;
                end++;
            }
        }

        if (!(*flag_check_comma))
        {
            new_polynomial->coefficient = coefficient;
            new_polynomial->degree = degree;
            new_polynomial->next = NULL;
            push_node(polynomial_after_comma,new_polynomial);
        }
        else
        {
            new_polynomial->coefficient = coefficient;
            new_polynomial->degree = degree;
            new_polynomial->next = NULL;
            push_node(polynomial_before_comma,new_polynomial);
        }
        if (*end == '+' || *end == '-') 
        {
            start = end;
        }
        else if(*end == ',')
        {
            // new polynomial
            (*flag_check_comma) = 1;
            end++;
            start = end;
        }
        else 
        {
            break;
        }
    }

}

enum error_command
{
    error_command_allocation_memory,
    no_command_error
};

enum error_command add_or_sub_polynomial(Node* polynomial_before_comma, Node* polynomial_after_comma, Node** summator, int flag_check_comma, 
int flag_add_or_sub)
{
    Node* temp1;
    Node* temp2;
    if (flag_check_comma)
    {
        temp1 = polynomial_before_comma;
        temp2 = polynomial_after_comma; 
    }
    else
    {
        temp2 = polynomial_after_comma;
        temp1 = (Node*)malloc(sizeof(Node)); 
        if (temp1 == NULL)
        {
            return error_command_allocation_memory;
        }
        temp1->coefficient = polynomial_before_comma->coefficient;
        temp1->degree = polynomial_before_comma->degree;
        temp1->next = polynomial_before_comma->next;
    }
    if (*summator != NULL)
    {
        if (flag_check_comma)
        {
            Node* free_summator = (*summator);
            free_nodes(free_summator);
            (*summator) = NULL;
        }
        else
        {
            Node* free_summator = (*summator);
            free(free_summator);
            (*summator) = NULL;
        }
    }
    while (temp1 != NULL && temp2 != NULL)
    {
        Node* summator_part = (Node*)malloc(sizeof(Node));
        if (summator_part == NULL)
        {
            return error_command_allocation_memory;
        }
        if (temp2->degree > temp1->degree)
        {
            summator_part->degree = temp2->degree;
            if (flag_add_or_sub)
            {
                summator_part->coefficient = temp2->coefficient;
            }
            else
            {
                summator_part->coefficient = -temp2->coefficient;
            }
            summator_part->next = NULL;
            push_node(summator, summator_part);
            temp2 = temp2->next;
        }
        else if (temp2->degree < temp1->degree)
        {
            summator_part->degree = temp1->degree;
            summator_part->coefficient = temp1->coefficient;
            summator_part->next = NULL;
            push_node(summator, summator_part);
            temp1 = temp1->next;
        }
        else if (temp1->degree == temp2->degree)
        {
            summator_part->degree = temp1->degree;
            if (flag_add_or_sub)
            {
                summator_part->coefficient = temp1->coefficient + temp2->coefficient;
            }
            else
            {
                summator_part->coefficient = temp1->coefficient - temp2->coefficient;
            }
            summator_part->next = NULL;
            if (summator_part->coefficient == 0 && summator_part->degree != 0)
            {
                free(summator_part);
            }
            else if (summator_part->coefficient != 0 && summator_part->degree != 0)
            {
                push_node(summator, summator_part);
            }
            else if (summator_part->degree == 0 && summator_part->coefficient != 0)
            {
                push_node(summator, summator_part);
            }
            temp1 = temp1->next;
            temp2 = temp2->next;
        }
    }
    if (temp1 == NULL && temp2 != NULL)
    {
        while (temp2 != NULL)
        {
            Node* summator_part = (Node*)malloc(sizeof(Node));
            if (summator_part == NULL)
            {
                return error_command_allocation_memory;
            }
            summator_part->degree = temp2->degree;
            summator_part->coefficient = temp2->coefficient;
            summator_part->next = NULL;
            push_node(summator, summator_part);
            temp2 = temp2->next;
        }
    }
    else if (temp1 != NULL && temp2 == NULL)
    {
        while (temp1 != NULL)
        {
            Node* summator_part = (Node*)malloc(sizeof(Node));
            if (summator_part == NULL)
            {
                return error_command_allocation_memory;
            }
            summator_part->degree = temp1->degree;
            summator_part->coefficient = temp1->coefficient;
            summator_part->next = NULL;
            push_node(summator, summator_part);
            temp1 = temp1->next;
        }
    }
    return no_command_error;
}

enum error_command push_node_after_mult(Node** summator, Node* summator_part)
{
    if ((*summator) == NULL)
    {
        (*summator) = summator_part;
    }
    else
    {
        while (summator_part != NULL)
        {
            Node* current = (*summator);
            Node* prev = current;
            int flag_fill = 0;
            while (current != NULL)
            {
                if (current->degree > summator_part->degree)
                {
                    prev = current;
                    current = current->next;
                }
                else if (current->degree == summator_part->degree && !flag_fill)
                {
                    current->coefficient += summator_part->coefficient;
                    prev = current;
                    current = current->next;
                    flag_fill = 1;
                }
                else if (current->degree < summator_part->degree && !flag_fill)
                {
                    flag_fill = 1;
                    prev->next = summator_part;
                    summator_part->next = current;
                    prev = current;
                    current = current->next;
                }
                if (flag_fill)
                {
                    break;
                }
            }
            if (!flag_fill)
            {
                prev->next = summator_part;
            }
            summator_part = summator_part->next;
        }
    }
}

enum error_command mult_polynomial(Node* polynomial_before_comma, Node* polynomial_after_comma, Node** summator, int flag_check_comma)
{
    Node* temp1;
    Node* temp2;
    if (flag_check_comma)
    {
        temp2 = polynomial_before_comma;
        temp1 = polynomial_after_comma; 
    }
    else
    {
        temp1 = polynomial_after_comma;
        temp2 = (Node*)malloc(sizeof(Node)); 
        if (temp2 == NULL)
        {
            return error_command_allocation_memory;
        }
        temp2->coefficient = polynomial_before_comma->coefficient;
        temp2->degree = polynomial_before_comma->degree;
        temp2->next = polynomial_before_comma->next;
    }
    if (*summator != NULL)
    {
        if (flag_check_comma)
        {
            Node* free_summator = (*summator);
            free_nodes(free_summator);
            (*summator) = NULL;
        }
        else
        {
            Node* free_summator = (*summator);
            free(free_summator);
            (*summator) = NULL;
        }
    }
    int flag_check_mult_null = 0;
    if (temp1->degree == 0 || temp2->degree == 0)
    {
        flag_check_mult_null = 1;
        Node* summator_part = (Node*)malloc(sizeof(Node));
        if (summator_part == NULL)
        {
            return error_command_allocation_memory;
        }
        summator_part->degree = 0;
        summator_part->coefficient = 0;
        summator_part->next = NULL;
        push_node(summator,summator_part);
    }
    while (temp1 != NULL && temp2 != NULL && (!flag_check_mult_null))
    {
        Node* summator_part = (Node*)malloc(sizeof(Node));
        if (summator_part == NULL)
        {
            return error_command_allocation_memory;
        }
        summator_part->degree = temp1->degree + temp2->degree;
        summator_part->coefficient = temp1->coefficient * temp2->coefficient;
        summator_part->next = NULL;
        push_node_after_mult(summator, summator_part);
        temp1 = temp1->next;
        if (temp1 == NULL && temp2 != NULL)
        {
            temp1 = polynomial_after_comma;
            temp2 = temp2->next;
        }
    }
    if (!flag_check_comma)
    {
        free_nodes(temp2);
    }
    return no_command_error;

}

enum error_command div_or_mod_polynomial(Node* polynomial_before_comma, Node* polynomial_after_comma, Node** summator, int flag_check_comma,
int flag_check_div_or_mod) // реализовать деление на 0.
{
    Node* temp1;
    Node* temp2;
    if (flag_check_comma)
    {
        temp1 = polynomial_before_comma;
        temp2 = polynomial_after_comma; 
    }
    else
    {
        temp2 = polynomial_after_comma;
        temp1 = (Node*)malloc(sizeof(Node)); 
        if (temp1 == NULL)
        {
            return error_command_allocation_memory;
        }
        temp1->coefficient = polynomial_before_comma->coefficient;
        temp1->degree = polynomial_before_comma->degree;
        temp1->next = polynomial_before_comma->next;
    }
    if (*summator != NULL)
    {
        if (flag_check_comma)
        {
            Node* free_summator = (*summator);
            free_nodes(free_summator);
            (*summator) = NULL;
        }
        else
        {
            Node* free_summator = (*summator);
            free(free_summator);
            (*summator) = NULL;
        }
    }

    Node* current_temp1 = temp1;
    while(temp1->degree == temp2->degree && (temp1 != NULL && temp2 !=NULL))
    {
        temp1 = temp1->next;
        temp2 = temp2->next;
    }
    if (temp1 == NULL && temp2 ==NULL)
    {
        Node* summator_part = (Node*)malloc(sizeof(Node));
        if (summator_part == NULL)
        {
            return error_command_allocation_memory;
        }
        if (flag_check_div_or_mod)
        {
            summator_part->coefficient = 1;
            summator_part->degree = 0;
            summator_part->next = NULL;
            push_node(summator,summator_part);
        }
        else
        {
            summator_part->coefficient = 0;
            summator_part->degree = 0;
            summator_part->next = NULL;
            push_node(summator,summator_part);
        }
    }
    else if (temp1->degree < temp2->degree)
    {
        Node* summator_part = (Node*)malloc(sizeof(Node));
        if (summator_part == NULL)
        {
            return error_command_allocation_memory;
        }
        if (flag_check_div_or_mod)
        {
            summator_part->coefficient = 0;
            summator_part->degree = 0;
            summator_part->next = NULL;
            push_node(summator,summator_part);
        }
        else
        {
            while (temp1 != NULL)
            {
                summator_part->coefficient = temp1->coefficient;
                summator_part->degree = temp1->degree;
                summator_part->next = NULL;
                push_node(summator,summator_part);
                temp1 = temp1->next;
            }
        }
    }
    else if (temp1->degree > temp2->degree)
    {
        temp2 = polynomial_after_comma;
        if (flag_check_comma)
        {
            temp1 = polynomial_before_comma;
        }
        else
        {
            temp1 = current_temp1;
        }
        int coefficient;
        int degree;
        while (temp1->degree >= temp2->degree)
        {
            Node* substruction = NULL;
            coefficient = temp1->coefficient / temp2->coefficient;
            degree = temp1->degree - temp2->degree;
            if (flag_check_div_or_mod)
            {
                Node* summator_part = (Node*)malloc(sizeof(Node));
                if (summator_part == NULL)
                {
                    return error_command_allocation_memory;
                }
                summator_part->degree = degree;
                summator_part->coefficient = coefficient;
                summator_part->next = NULL; 
                push_node(summator,summator_part);
            }
            while (temp2 != NULL)
            {
                Node* substruction_part = (Node*)malloc(sizeof(Node));
                if (substruction_part == NULL)
                {
                    return error_command_allocation_memory;
                }
                substruction_part->degree = temp2->degree + degree;
                substruction_part->coefficient = temp2->coefficient * coefficient;
                substruction_part->next = NULL;
                push_node(&substruction,substruction_part);
                temp2 = temp2->next;
            }
            temp2 = polynomial_after_comma;
            add_or_sub_polynomial(temp1,substruction,&temp1,0,0);
            free_nodes(substruction);
        }
        if (!flag_check_div_or_mod)
        {
            Node* current = temp1;
            while (current != NULL)
            {
                Node* summator_part = (Node*)malloc(sizeof(Node));
                if (summator_part == NULL)
                {
                    return error_command_allocation_memory;
                }
                summator_part->coefficient = current->coefficient;
                summator_part->degree = current->degree;
                summator_part->next = NULL;
                push_node(summator,summator_part);
                current = current->next;
            }
            free_nodes(temp1);
        }

    }
    return no_command_error;

}

enum error_command eval_polynomial(Node* polynomial_before_comma, Node* polynomial_after_comma, Node** summator, int flag_check_comma)
{
    Node* temp1;
    Node* temp2;
    if (flag_check_comma)
    {
        temp1 = polynomial_before_comma;
        temp2 = polynomial_after_comma; 
    }
    else
    {
        temp2 = polynomial_after_comma;
        temp1 = (Node*)malloc(sizeof(Node)); 
        if (temp1 == NULL)
        {
            return error_command_allocation_memory;
        }
        temp1->coefficient = polynomial_before_comma->coefficient;
        temp1->degree = polynomial_before_comma->degree;
        temp1->next = polynomial_before_comma->next;
    }
    if (*summator != NULL)
    {
        if (flag_check_comma)
        {
            Node* free_summator = (*summator);
            free_nodes(free_summator);
            (*summator) = NULL;
        }
        else
        {
            Node* free_summator = (*summator);
            free(free_summator);
            (*summator) = NULL;
        }
    }
    int sum = 0;
    int num;
    while (temp1 != NULL)
    {
        num = 1;
        for (int i = 0; i < temp1->degree; i++)
        {
            num = num * temp2->coefficient;
        }
        num *= temp1->coefficient;
        sum += num;
        temp1 = temp1->next;
    }
    Node* summator_part = (Node*)malloc(sizeof(Node));
    if (summator_part == NULL)
    {
        return error_command_allocation_memory;
    }
    summator_part->degree = 0;
    summator_part->coefficient = sum;
    summator_part->next = NULL;
    push_node(summator,summator_part);
    return no_command_error;
}

enum error_command diff_polynomial(Node* polynomial, Node** summator)
{
    Node* temp = polynomial;
    if (*summator != NULL)
    {
        Node* free_summator = (*summator);
        free_nodes(free_summator);
        (*summator) = NULL;
    }
    while (temp != NULL)
    {
        Node* summator_part = (Node*)malloc(sizeof(Node));
        if (summator_part == NULL)
        {
            return error_command_allocation_memory;
        }
        summator_part->degree = temp->degree-1;
        if (summator_part->degree > -1)
        {
            summator_part->coefficient = temp->coefficient*temp->degree;
            summator_part->next = NULL;
            push_node(summator,summator_part);
        }
        else
        {
            if ((*summator) == NULL)
            {
                summator_part->coefficient = 0;
                summator_part->degree = 0;
                summator_part->next = NULL;
                push_node(summator,summator_part);
            }
            else
            {
               free(summator_part); 
            }
        }
        temp = temp->next;
    }
    return no_command_error;
}

enum error_command cmps_polynomial(Node* polynomial_before_comma, Node* polynomial_after_comma, Node** summator, int flag_check_comma)
{
    Node* temp1;
    Node* temp2;
    if (flag_check_comma)
    {
        temp1 = polynomial_before_comma;
        temp2 = polynomial_after_comma; 
    }
    else
    {
        temp2 = polynomial_after_comma;
        temp1 = (Node*)malloc(sizeof(Node)); 
        if (temp1 == NULL)
        {
            return error_command_allocation_memory;
        }
        temp1->coefficient = polynomial_before_comma->coefficient;
        temp1->degree = polynomial_before_comma->degree;
        temp1->next = polynomial_before_comma->next;
    }
    if (*summator != NULL)
    {
        if (flag_check_comma)
        {
            Node* free_summator = (*summator);
            free_nodes(free_summator);
            (*summator) = NULL;
        }
        else
        {
            Node* free_summator = (*summator);
            free(free_summator);
            (*summator) = NULL;
        }
    }

    int count;
    while (temp1 != NULL)
    {
        if (temp1->degree == 0)
        {
            Node* summator_part = (Node*)malloc(sizeof(Node));
            if (summator_part == NULL)
            {
                return error_command_allocation_memory;
            }
            summator_part->degree = 0;
            summator_part->coefficient = 1*temp1->coefficient;
            summator_part->next = NULL;
            push_node_after_mult(summator,summator_part);
        }
        else if (temp1->degree == 1)
        {
            while (temp2 != NULL)
            {
                Node* summator_part = (Node*)malloc(sizeof(Node));
                if (summator_part == NULL)
                {
                    return error_command_allocation_memory;
                }
                summator_part->degree = temp2->degree;
                summator_part->coefficient = temp2->coefficient*temp1->coefficient;
                summator_part->next = NULL;
                push_node_after_mult(summator,summator_part);
                temp2 = temp2->next;
            }
        }
        else
        {
            count = 1;
            Node* new_temp2 = (Node*)malloc(sizeof(Node));
            if (new_temp2 == NULL)
            {
                return error_command_allocation_memory;
            }
            new_temp2->degree = temp2->degree;
            new_temp2->coefficient = temp2->coefficient;
            new_temp2->next = temp2->next;
            while (count < temp1->degree)
            {
                mult_polynomial(new_temp2,temp2,&new_temp2,0);
                count++;
            }
            Node* current = new_temp2;
            while (current != NULL)
            {
                current->coefficient = current->coefficient*temp1->coefficient;
                current = current->next;
            }
            push_node_after_mult(summator,new_temp2);
        }
        temp1 = temp1->next;
    }
    return no_command_error;
}

enum error_command executing_the_command(Node* polynomial_before_comma,Node* polynomial_after_comma,Node** summator, 
const char* command, int flag_check_comma)
{
    if (strcmp(command, "Add") == 0)
    {
        if (flag_check_comma)
        {
            add_or_sub_polynomial(polynomial_before_comma,polynomial_after_comma,summator,flag_check_comma,1);
        }
        else
        {
            add_or_sub_polynomial((*summator),polynomial_after_comma,summator,flag_check_comma,1);
        }
    }
    else if (strcmp(command, "Sub") == 0)
    {
        if (flag_check_comma)
        {
            add_or_sub_polynomial(polynomial_after_comma,polynomial_before_comma,summator,flag_check_comma,0);
        }
        else
        {
            add_or_sub_polynomial((*summator),polynomial_after_comma,summator,flag_check_comma,0);
        }
    }
    else if (strcmp(command, "Mult") == 0)
    {
        if (flag_check_comma)
        {
            mult_polynomial(polynomial_after_comma,polynomial_before_comma,summator,flag_check_comma);
        }
        else
        {
            mult_polynomial((*summator),polynomial_after_comma,summator,flag_check_comma);
        }
    }
    else if (strcmp(command, "Div") == 0)
    {
        if (flag_check_comma)
        {
            if (div_or_mod_polynomial(polynomial_after_comma,polynomial_before_comma,summator,flag_check_comma,1) != no_command_error)
            {
                return error_command_allocation_memory;
            }
        }
        else
        {
            if (div_or_mod_polynomial((*summator),polynomial_after_comma,summator,flag_check_comma,1)!= no_command_error)
            {
                return error_command_allocation_memory;
            }
        }
    }
    else if (strcmp(command, "Mod") == 0)
    {
        if (flag_check_comma)
        {
            if (div_or_mod_polynomial(polynomial_after_comma,polynomial_before_comma,summator,flag_check_comma,0) != no_command_error)
            {
                return error_command_allocation_memory;
            }
        }
        else
        {
            if (div_or_mod_polynomial((*summator),polynomial_after_comma,summator,flag_check_comma,0)!= no_command_error)
            {
                return error_command_allocation_memory;
            }
        }
    }
    else if (strcmp(command, "Eval") == 0)
    {
        if (flag_check_comma)
        {
            if (eval_polynomial(polynomial_after_comma,polynomial_before_comma,summator,flag_check_comma) != no_command_error)
            {
                return error_command_allocation_memory;
            }
        }
        else
        {
            if (eval_polynomial((*summator),polynomial_after_comma,summator,flag_check_comma) != no_command_error)
            {
                return error_command_allocation_memory;
            }
        }
    }
    else if (strcmp(command, "Diff") == 0)
    {
        if (diff_polynomial(polynomial_after_comma,summator) != no_command_error)
        {
            return error_command_allocation_memory;
        }
    }
    else if (strcmp(command, "Cmps") == 0)
    {
        if (flag_check_comma)
        {
            if (cmps_polynomial(polynomial_after_comma,polynomial_before_comma,summator,flag_check_comma) != no_command_error)
            {
                return error_command_allocation_memory;
            }
        }
        else
        {
            if (cmps_polynomial((*summator),polynomial_after_comma,summator,flag_check_comma) != no_command_error)
            {
                return error_command_allocation_memory;
            }
        }
    }
    return no_command_error;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Error input\n");
        return 1;
    }
    FILE* input = fopen(argv[1], "r");
    Node* summator = (Node*)malloc(sizeof(Node));
    if (summator == NULL)
    {
        printf("Error allocation memory\n");
        return 2;
    }
    summator->coefficient = 0;
    summator->degree = 0;
    summator->next = NULL;
    char line[256];
    int flag_check_multiline_comment = 0;
    int flag_check_comma;
    char* found_command;
    char* found_polynomials;
    while (fgets(line,sizeof(line),input) != NULL)
    {
        Node* polynomial_before_comma = NULL;
        Node* polynomial_after_comma = NULL;
        enum errors_check_input_line check_line = check_input_line(line, &flag_check_multiline_comment,&found_command,&found_polynomials);
        if (check_line != no_input_error && check_line != multiline_comment)
        {
            printf("Error: %s\n", get_error_message_input(check_line));
        }
        else if (check_line == multiline_comment)
        {
            continue;
        }
        else
        {
            fill_nodes(&polynomial_before_comma, &polynomial_after_comma, found_polynomials, &flag_check_comma);
            enum error_command check_command = executing_the_command(polynomial_before_comma, polynomial_after_comma, &summator, found_command, flag_check_comma);
            if (check_command == error_command_allocation_memory)
            {
                printf("Error allocation memory\n");
                return 2;
            }
            if (flag_check_comma)
            {
                free_nodes(polynomial_before_comma);
                free_nodes(polynomial_after_comma);
            }
            else
            {
                free_nodes(polynomial_after_comma);
            }
            print_node(summator);
        }

    }

}
