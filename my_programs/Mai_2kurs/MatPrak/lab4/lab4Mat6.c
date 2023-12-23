#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

typedef struct Stack
{
    char elem[3];
    struct Stack* next;
}Stack;

typedef struct bynary_tree
{
    char elem_tree[3];
    struct bynary_tree* left;
    struct bynary_tree* right;
}Tree;

typedef enum error_postfix
{
    error_pref_allocation_memory,
    no_postfix_error
}CHECK_POSTFIX;

enum fill_error
{
    error_fill_allocation_memory,
    no_fill_error
};

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
        current = temp->next;
        free(temp);
        temp = current;        
    }
}

Tree* create_tree()
{
    Tree* node = (Tree*)malloc(sizeof(Tree));
    if (node == NULL)
    {
        return NULL;
    }
    node->left = NULL;
    node->right = NULL;
    return node;
}

void free_tree(Tree* node)
{
    if (node != NULL)
    {
        free_tree(node->left);
        free_tree(node->right);
        free(node); 
    }
}

int get_priority(const char* elem)
{
    if (strcmp(elem, "~") == 0)
    {
        return 3;
    }
    else if (strcmp(elem, "?") == 0 || strcmp(elem, "!") == 0 || strcmp(elem, "+>") == 0 
    || strcmp(elem, "&") == 0)
    {
        return 2;
    }
    else if (strcmp(elem, "|") == 0 || strcmp(elem, "->") == 0 || strcmp(elem, "<>") == 0 
    || strcmp(elem, "=") == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}

void printTree(Tree* node) {
    if (node != NULL) {
        printTree(node->left);
        printTree(node->right);
        printf("%s ", node->elem_tree);
    }
}

enum fill_error fill_tree(Tree** node, Stack* postfix_exp)
{
    Tree* temp = NULL;
    while (postfix_exp != NULL)
    {
        Tree* new_node = (Tree*)malloc(sizeof(Tree));
        if (new_node == NULL)
        {
            return error_fill_allocation_memory;
        }
        strcpy(new_node->elem_tree, postfix_exp->elem);
        new_node->left = NULL;
        new_node->right = NULL;
        if ((*node) == NULL)
        {
            (*node) = new_node;
            temp = (*node);
            pop_stack(&postfix_exp);
            continue;
        }
        if (!isalnum(postfix_exp->elem[0]))
        {
            int elem_tree_p;
            int elem_stack_p = get_priority(postfix_exp->elem);
            if (elem_stack_p == 3 && (temp != (*node) || temp != NULL))
            {
                if (temp->right == NULL)
                {
                    temp->right = new_node;
                    temp = temp->right;
                    pop_stack(&postfix_exp);
                    continue;
                }
            }
            else
            {
                temp = (*node);
            }
            while (temp != NULL)
            {
                elem_tree_p = get_priority(temp->elem_tree);
                if (elem_tree_p > elem_stack_p)
                {
                    if (temp->left == NULL)
                    {
                        temp->left = new_node;
                        temp = temp->left;
                        break;
                    }
                    else
                    {
                        if (temp->right == NULL)
                        {
                            temp->right = new_node;
                            temp = temp->right;
                            break;
                        }
                        else
                        {
                            temp = temp->left;
                        }
                    }
                }
                else
                {
                    if (temp->right == NULL)
                    {
                        temp->right = new_node;
                        temp = temp->right;
                        break;
                    }
                    else
                    {
                        if (temp->left == NULL && (elem_tree_p == elem_stack_p))
                        {
                            temp->left = new_node;
                            temp = temp->left;
                            break;
                        }
                        else
                        {
                            temp = temp->right;
                        }
                    }
                }
            }
        }
        else
        {
            if (temp != NULL)
            {
                if (temp->left == NULL)
                {
                    temp->left = new_node;
                }
                else if(temp->right == NULL)
                {
                    temp->right = new_node;
                }
                else
                {
                    temp = (*node);
                    while (temp != NULL)
                    {
                        if (temp->right != NULL && temp->left == NULL)
                        {
                            temp->left = new_node;
                            break;
                        }
                        else if (temp->right == NULL && temp->left != NULL)
                        {
                            temp->right = new_node;
                            break;
                        }
                        else if (temp->right != NULL && temp->left != NULL)
                        {
                            temp = temp->left;
                        }//var
                    }
                    temp = new_node;
                }
                if (temp->elem_tree[0] == '~')
                {
                    temp = (*node);
                }
            }
        }
        pop_stack(&postfix_exp);
    }
    return no_fill_error;
}

void take_path(char* path, char* find_path, int* name_size)
{
    size_t len_path = strlen(path);
    size_t i;
    size_t j;
    for (i = len_path-1; path[i] != '/' && i != 0; i--)
    ;
    (*name_size) = len_path-i;
    for (j = 0; j <= i && i != 0; j++)
    {
        find_path[j] = path[j];
    }
    find_path[j] = '\0';
}

bool isBinaryOperator(char c) {
    switch (c) {
        case '&':
        case '|':
        case '~':
        case '=':
        case '!':
        case '?':
            return true;
        default:
            return false;
    }
}

// Функция для валидации логического выражения
bool isValidExpression(char expression[]) {
    int openParentheses = 0;
    int flag_sign = 0;
    int flag_alnum = 0;
    int flag_open_bracket = 0;
    for (int i = 0; expression[i] != '\0'; i++) {
        switch (expression[i]) {
            case '(':
                openParentheses++;
                if (flag_alnum)
                {
                    return false;
                }
                flag_open_bracket = 1;
                flag_sign = 0;
                break;
            case ')':
                if (openParentheses <= 0) {
                    return false; // Неверное количество закрывающих скобок
                }
                if (flag_sign)
                {
                    return false;
                }
                openParentheses--;
                break;
            default:
                if (isalnum(expression[i]) && flag_alnum)
                {
                    return false;
                }

                if (!(isBinaryOperator(expression[i])) && !(isalpha(expression[i])) 
                && expression[i] != '0' && expression[i] != '1') 
                {
                    if ((i == 0 || expression[i+1] == '\0' 
                    || !( (expression[i] == '<' && expression[i+1] == '>')
                    || (expression[i] == '+' && expression[i+1] == '>') 
                    || (expression[i] == '-' && expression[i+1] == '>') )))
                    {
                        return false;
                    }
                    if ((expression[i] == '<' && expression[i+1] == '>')
                    || (expression[i] == '+' && expression[i+1] == '>') 
                    || (expression[i] == '-' && expression[i+1] == '>'))
                    {
                        if (flag_sign)
                        {
                            return false;
                        }
                        i++;
                        flag_sign = 1;
                        flag_alnum = 0;
                        flag_open_bracket = 0;
                    }
                }
                else
                {
                    if (isalnum(expression[i]))
                    {
                        flag_sign = 0;
                        flag_open_bracket = 0;
                        flag_alnum = 1;
                    }
                }
                // Проверка на дублирование бинарных операторов
                if (isBinaryOperator(expression[i])) {
                    if ((isBinaryOperator(expression[i + 1]) && expression[i + 1] != '~') || (i == 0 && expression[i] != '~')) {
                        return false; // Дублирование бинарного оператора
                    }
                    if (flag_alnum && expression[i+1] == '~')
                    {
                        return false;
                    }
                    if (flag_open_bracket && (expression[i] != '~'))
                    {
                        return false;
                    }
                    if (flag_sign && (expression[i] != '~'))
                    {
                        return false;
                    }
                    flag_sign = 1;
                    flag_alnum = 0;
                }
        }
    }

    return openParentheses == 0 && !isBinaryOperator(expression[strlen(expression) - 1]) && !flag_sign; // Проверка на равное количество открывающих и закрывающих скобок, а также на последний символ
}

void print_stack(Stack* stack)
{
    Stack* temp = stack;
    int el = 0;
    while (temp != NULL)
    {
        printf("El %d: %s\n", ++el, temp->elem);
        temp = temp->next;
    }
}

CHECK_POSTFIX prefix_notation(Stack** result_stack, const char* expression)
{
    int i = 0;
    Stack* temp_stack = NULL;
    while (expression[i] != '\0')
    {
        char temp_elem[3];
        if (expression[i] == '<' || expression[i] == '+' || expression[i] == '-')
        {
            temp_elem[0] = expression[i];
            temp_elem[1] = expression[i+1];
            temp_elem[2] = '\0';
        }
        else if (!isalnum(expression[i]) && expression[i] != '(' && expression[i] != ')')
        {
            temp_elem[0] = expression[i];
            temp_elem[1] = '\0';
        }
        int priority = get_priority(temp_elem);
        if (expression[i] == '(')
        {
            Stack* temp_part = (Stack*)malloc(sizeof(Stack));
            if (temp_part == NULL)
            {
                free_stack(&temp_stack);
                return error_pref_allocation_memory;
            }
            temp_part->next = NULL;
            temp_part->elem[0] = '(';
            temp_part->elem[1] = '\0';
            i++;
            push_stack(&temp_stack,temp_part);
        }
        else if (isalnum(expression[i]))
        {
            Stack* result_part = (Stack*)malloc(sizeof(Stack));
            if (result_part == NULL)
            {
                free_stack(&temp_stack);
                return error_pref_allocation_memory;
            }
            result_part->next = NULL;
            result_part->elem[0] = expression[i];
            result_part->elem[1] = '\0';
            i++;
            push_stack(result_stack,result_part); 
        }
        else if (expression[i] == ')')
        {
            while(temp_stack != NULL && temp_stack->elem[0] != '(')
            {
                Stack* result_part = (Stack*)malloc(sizeof(Stack));
                if (result_part == NULL)
                {
                    free_stack(&temp_stack);
                    return error_pref_allocation_memory;
                }
                result_part->next = NULL;
                if (temp_stack->elem[0] == '<' || temp_stack->elem[0] == '-' || temp_stack->elem[0] == '+')
                {
                    result_part->elem[0] = temp_stack->elem[0];
                    result_part->elem[1] = temp_stack->elem[1];
                    result_part->elem[2] = '\0';
                }
                else
                {
                    result_part->elem[0] = temp_stack->elem[0];
                    result_part->elem[1] = '\0';
                }
                push_stack(result_stack,result_part);
                pop_stack(&temp_stack);
            }
            if (temp_stack != NULL)
            {
                pop_stack(&temp_stack);
            }
            i++;
        } 
        else if (priority == 3)
        {
            Stack* temp_part = (Stack*)malloc(sizeof(Stack));
            if (temp_part == NULL)
            {
                free_stack(&temp_stack);
                return error_pref_allocation_memory;
            }
            temp_part->next = NULL;
            temp_part->elem[0] = '~';
            temp_part->elem[1] = '\0';
            if (temp_stack == NULL || temp_stack->elem[0] == '(' || temp_stack->elem[0] != '~')
            {
                push_stack(&temp_stack,temp_part);
            }
            else if (temp_stack->elem[0] == '~')
            {
                while (temp_stack != NULL && temp_stack->elem[0] == '~')
                {
                    Stack* result_part = (Stack*)malloc(sizeof(Stack));
                    if (result_part == NULL)
                    {
                        free_stack(&temp_stack);
                        return error_pref_allocation_memory;
                    }
                    result_part->next = NULL;
                    result_part->elem[0] = '~';
                    result_part->elem[1] = '\0';
                    push_stack(result_stack,result_part);
                    pop_stack(&temp_stack);
                }
                push_stack(&temp_stack, temp_part);
                
            }
            i++;
        }
        else if (priority == 2)
        {
            Stack* temp_part = (Stack*)malloc(sizeof(Stack));
            if (temp_part == NULL)
            {
                free_stack(&temp_stack);
                return error_pref_allocation_memory;
            }
            temp_part->next = NULL;
            strcpy(temp_part->elem, temp_elem);
            if (temp_stack == NULL || temp_stack->elem[0] == '(')
            {
                push_stack(&temp_stack,temp_part);
            }
            else
            {
                while (temp_stack != NULL 
                && temp_stack->elem[0] != '(' 
                && temp_stack->elem[0] != '|' && temp_stack->elem[0] != '-' 
                && temp_stack->elem[0] != '<' && temp_stack->elem[0] != '=')
                {
                    Stack* result_part = (Stack*)malloc(sizeof(Stack));
                    if (result_part == NULL)
                    {
                        free_stack(&temp_stack);
                        return error_pref_allocation_memory;
                    }
                    result_part->next = NULL;
                    strcpy(result_part->elem,temp_stack->elem);
                    push_stack(result_stack,result_part);
                    pop_stack(&temp_stack);
                }
                push_stack(&temp_stack,temp_part);
            }
            if (strlen(temp_elem) == 2)
            {
                i +=2;
            }
            else
            {
                i++;
            }
        }
        else if (priority == 1)
        {
            Stack* temp_part = (Stack*)malloc(sizeof(Stack));
            if (temp_part == NULL)
            {
                free_stack(&temp_stack);
                return error_pref_allocation_memory;
            }
            temp_part->next = NULL;
            strcpy(temp_part->elem, temp_elem);
            if (temp_stack == NULL || temp_stack->elem[0] == '(')
            {
                push_stack(&temp_stack,temp_part);
            }
            else
            {
                while (temp_stack != NULL && temp_stack->elem[0] != '(' )
                {
                    Stack* result_part = (Stack*)malloc(sizeof(Stack));
                    if (result_part == NULL)
                    {
                        free_stack(&temp_stack);
                        return error_pref_allocation_memory;
                    }
                    result_part->next = NULL;
                    strcpy(result_part->elem,temp_stack->elem);
                    push_stack(result_stack,result_part);
                    pop_stack(&temp_stack);
                }
                push_stack(&temp_stack,temp_part);
            }
            if (strlen(temp_elem) == 2)
            {
                i +=2;
            }
            else
            {
                i++;
            }
        }
    }
    if (temp_stack != NULL)
    {
        while (temp_stack != NULL)
        {
            Stack* result_part = (Stack*)malloc(sizeof(Stack));
            if (result_part == NULL)
            {
                free_stack(&temp_stack);
                return error_pref_allocation_memory;
            }
            result_part->next = NULL;
            strcpy(result_part->elem,temp_stack->elem);
            push_stack(result_stack,result_part);
            pop_stack(&temp_stack);
        }
    }
    return no_postfix_error;
}

void counting_letters(char* exp, int* count, char* letters)
{
    while (*exp != '\0')
    {
        if (isalpha((*exp)))
        {
            letters[(*count)] = (*exp);
            (*count)++;
        }
        exp++;
    }
    letters[(*count)] = '\0';
}

int evaluateExpression(Tree* node, int variables[], int* j) {
    if (node == NULL) {
        return -1;
    }
    if (node->left == NULL && node->right == NULL) {
        if (isalpha(node->elem_tree[0]))
        {
            (*j)--;
            return variables[(*j)];
        }
        else if (node->elem_tree[0] == '0' || node->elem_tree[0] == '1')
        {
            return node->elem_tree[0] - '0';
        }
        
    }

    // Рекурсивно вычисляем значения левого и правого поддеревьев
    int leftValue = evaluateExpression(node->left, variables, j);
    int rightValue = evaluateExpression(node->right, variables, j);
    int temp;
    // Выполняем операцию в зависимости от оператора
    switch (node->elem_tree[0]) {
        case '&':
            return leftValue & rightValue;
        case '|':
            return leftValue | rightValue;
        case '!':
            return (leftValue & rightValue) ^ 1;
        case '-':
            return (rightValue ^ 1) | leftValue;
        case '~':
            if (leftValue == -1)
            {
                return rightValue ^ 1;
            }
            else
            {
                return leftValue ^ 1;
            }
        case '+':
            return ~(~rightValue | leftValue);
        case '<':
            return leftValue ^ rightValue;
        case '=':
            temp = ((leftValue ^ 1) & (rightValue ^ 1)) | (leftValue & rightValue);
            return temp;
        case '?':
            return (leftValue | rightValue) ^ 1;
        default:
            printf("Ошибка: недопустимый оператор\n");
    }
}

void create_table(Tree* node, int count_letters, char* letters, FILE* output)
{
    int variables[count_letters];
    int res_exp[count_letters];
    for (int i = 0; i <= count_letters; i++)
    {
        fprintf(output,"%c",letters[i]);
        if (i < count_letters)
        {
            fprintf(output,"\t");
        }
    }
    fprintf(output,"Result\n");

    for (int i = 0; i < (1 << count_letters); ++i) {
        // Заполняем массив переменных значениями битов текущего значения счетчика
        for (int j = 0; j < count_letters; ++j) {
            variables[j] = (i >> j) & 1;
            fprintf(output,"%d\t", variables[j]);
        }
        int temp = count_letters;
        int answer = evaluateExpression(node,variables,&(temp));
        fprintf(output,"   %d\n",answer);
    }
}

void generate_random_filename(char* filename, int length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    srand((unsigned int)time(NULL));

    for (int i = 0; i < length; ++i) {
        int index = rand() % (sizeof(charset) - 1);
        filename[i] = charset[index];
    }

    filename[length] = '\0';
}

int hasDuplicateLetters(const char* str) {
    int charSet[256] = {0};
    int len = strlen(str);

    for (int i = 0; i < len; ++i) {
        char currentChar = tolower(str[i]);
        if (isalpha(currentChar)) {
            if (charSet[currentChar] == 1) {
                return 1;
            }
            charSet[currentChar] = 1;
        }
    }

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 2)
    {
        printf("Error input\n");
        return 1;
    }
    FILE* input = fopen(argv[1],"r");
    if (input == NULL)
    {
        printf("Error open input file\n");
        return 2;
    }

    char expression[257];
    fflush(stdin);
    if (fgets(expression,sizeof(expression), input) == NULL)
    {
        printf("Can`t get str in file\n");
        return 1;
    }
    int name_size;
    char* find_path = (char*)malloc(sizeof(argv[1])*(strlen(argv[1])+1));
    if (find_path == NULL)
    {
        printf("Error allocation memory\n");
        return 1;
    }
    take_path(argv[1],find_path,&name_size);
    
    char* random_name = (char*)malloc(sizeof(char)*(name_size+2));
    if (random_name == NULL)
    {
        printf("Error allocation memory\n");
        return 1;
    }
    generate_random_filename(random_name, name_size);
    char* output_path = (char*)malloc(sizeof(char)*(strlen(argv[1])+5));
    sprintf(output_path, "%s%s.txt", find_path, random_name);
    FILE* output = fopen(output_path,"w");
    free(random_name);
    free(find_path);
    if (output == NULL || output == input)
    {
        fclose(input);
        printf("Error open in file or output = input\n");
        return 2;
    }

    size_t len_exp = strlen(expression);
    char* letters = (char*)malloc(sizeof(char)*len_exp);
    if (letters == NULL)
    {
        printf("Error allocation memory\n");
        return 1;
    }

    Stack* result_stack = NULL;
    Tree* node = NULL;
    int count = 0;
    if (isValidExpression(expression)) {
        printf("Expression correct.\n");
        prefix_notation(&result_stack,expression);
        counting_letters(expression,&count,letters);
        if (hasDuplicateLetters(letters))
        {
            printf("The same variables were found in the formula.");
            free_stack(&result_stack);
            free(letters);
            free(output_path);
            fclose(output);
            return 2;
        }
        fill_tree(&node, result_stack);
        create_table(node,count,letters, output);
        printf("Create correct, your output file is: %s\n", output_path);
        free_tree(node);
        free(letters);
        free(output_path);
        fclose(output);
    } else {
        free(letters);
        free(output_path);
        fclose(output);
        printf("Expression uncorrect.\n");
    }

    return 0;
}
