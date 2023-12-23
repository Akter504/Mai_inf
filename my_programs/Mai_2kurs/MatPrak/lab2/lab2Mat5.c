#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

int len_int(int num)
{
    int count = 0;
    while(num > 0)
    {
        num /=10;
        count++;
    }
    return count;
}

int tollower(char c){
   if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    } 
    else {
        return c;
    }
}

const char* intToRoman(int num) {
    if (num < 1 || num > 3999) {
        return "Invalid";
    }

    const char* romanNumerals[] = {
        "M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"
    };
    const int romanValues[] = {
        1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1
    };

    char* romanNum = (char*)malloc(20); // Выделяем память для результата
    if (romanNum == NULL) {
        return "Memory allocation error";
    }
    romanNum[0] = '\0'; // Убеждаемся, что строка пуста

    for (int i = 0; i < 13; i++) {
        while (num >= romanValues[i]) {
            strcat(romanNum, romanNumerals[i]);
            num -= romanValues[i];
        }
    }

    return romanNum;
}

int findClosestFibonacci(unsigned int n) {
    int a = 0, b = 1, c = 1;
    while (c <= n) {
        a = b;
        b = c;
        c = a + b;
    }
    return b;
}

void reverseString(char *str) {
    int length = strlen(str);
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void zrRepresentation(int n, char **result) {
    int a = 1, b = 1;
    int i = 0;
    int remember_n = n;
    int remember_i;
    char *answer;
    while (n > 0) {
        if (a == n)
        {
            answer = (char*)malloc(3*sizeof(char));
            answer[0] = '1';
            answer[1] = '1';
            answer[2] = '\0';
            n -=a;
        }
        else if (b == n && a != b && i != 0)
        {
            answer = (char*)malloc((i+3)*sizeof(char));
            answer[i+2] = '\0';
            answer[i+1] = '1';
            answer[i] = '1';
            n -=b;
        }
        else if (b < n)
        {
            int next = a+b;
            a = b;
            b = next;
            i++;
        }
        else if (b > n || (remember_i - i) > 1)
        {
            if (remember_n == n)
            {
                answer = (char*)malloc((i+3)*sizeof(char));
                answer[i+2] = '\0';
                answer[i+1] = '1';
                answer[i] = '1';
                remember_i = i;
                a = 1;
                b = 1;
                n -=a;
                i = 0;
            }
            answer[i] = '1';
            a = 1;
            b = 1;
            remember_i = i;
            i = 0;
            n -=a;
        }
    }
    printf("%s\n",answer);
}

void ten_to_base(int decimal,int base,char* num_arr_base,int flag) {
    int index = 0; 
    while (decimal > 0) {
        int remainder = decimal % base;
        if (remainder < 10) {
            num_arr_base[index] = remainder + '0';
        } 
        else {
            if (flag)
            {
                num_arr_base[index] = remainder - 10 + 'A';
            }
            else
            {
                num_arr_base[index] = remainder - 10 + 'a';
            }
        }
        num_arr_base[index + 1] = '\0';
        decimal /= base;
        index++;
    }
    for (int i= 0, j = index - 1; i < j; i++, j--) {
        char temp = num_arr_base[i];
        num_arr_base[i] = num_arr_base[j];
        num_arr_base[j] = temp;
    }
}


int overfprintf(FILE *stream, const char *format, ...) {
    va_list args;
    va_start(args, format);

    int result = 0;
    const char* formatPtr = format;
    char ch;

    if (strstr(format, "%Ro") == NULL && strstr(format, "%Zr") == NULL && strstr(format, "%Cv") == NULL && strstr(format, "%CV") == NULL) {
        // Если флаг %Ro не найден в формате, используем vfprintf для обработки стандартных флагов
        vfprintf(stream, format, args);
        va_end(args);
        return result;
    }

    while ((ch = *formatPtr)) {
        if (ch == '%' && formatPtr[1] == 'R' && formatPtr[2] == 'o') {
            formatPtr += 3;
            int num = va_arg(args, int);
            const char* romanNum = intToRoman(num);
            overfprintf(stream, "%s", romanNum);
            result += strlen(romanNum);
        }
        else if (*formatPtr == '%' && formatPtr[1] == 'Z' && formatPtr[2] == 'r') {
            formatPtr += 3;
            unsigned int num = va_arg(args, unsigned int);
            char* zrNum = (char*)malloc(100);
            zrRepresentation(num, &zrNum);
            overfprintf(stream, "%s", zrNum);
            result += strlen(zrNum);
            free(zrNum); // Освобождаем выделенную память 
        }
        else if (*formatPtr == '%' && formatPtr[1] == 'C' && (formatPtr[2] == 'v' || formatPtr[2] == 'V'))
        {
            int flag_big_or_small;
            if (formatPtr[2] == 'V')
            {
                flag_big_or_small = 1;
            }
            else
            {
                flag_big_or_small = 0;
            }
            formatPtr +=3;
            int num = va_arg(args,int);
            int base = va_arg(args,int);
            if (base > 36 || base < 2)
            {
                base = 10;
                int len_num = len_int(num);
                result += len_num;
                char temp[len_num];
                overfprintf(stream, "%s", temp);
            }
            else
            {
                char* num_with_base = (char*)malloc((10*len_int(num)+1)*sizeof(char));
                ten_to_base(num,base,num_with_base,flag_big_or_small);
                overfprintf(stream, "%s", num_with_base);
                result += strlen(num_with_base);
                free(num_with_base);
            }
        }
        else {
            fputc(ch, stream);
            result++;
            formatPtr++;
        }
    }

    va_end(args);
    return result;
}

int oversprintf(char *str, const char *format, ...) {
    va_list args;
    va_start(args, format);

    int result = 0;
    char* strPtr = str;
    const char* formatPtr = format;
    char ch;

    if (strstr(format, "%Ro") == NULL && strstr(format, "%Zr") == NULL && strstr(format, "%Cv") == NULL && strstr(format, "%CV") == NULL) {
        // Если флаг %Ro не найден в формате, используем vsprintf для обработки стандартных флагов
        vsprintf(str, format, args);
        va_end(args);
        return strlen(str);
    }

    while ((ch = *formatPtr)) {
        if (ch == '%' && formatPtr[1] == 'R' && formatPtr[2] == 'o') {
            formatPtr += 3;
            int num = va_arg(args, int);
            const char* romanNum = intToRoman(num);
            strcpy(strPtr, romanNum);
            strPtr += strlen(romanNum);
            result += strlen(romanNum);
        }
        else if (*formatPtr == '%' && formatPtr[1] == 'Z' && formatPtr[2] == 'r') {
            formatPtr += 3;
            unsigned int num = va_arg(args, unsigned int);
            char* zrNum = (char*)malloc(100);
            zrRepresentation(num, &zrNum);
            strcpy(strPtr, zrNum);
            strPtr += strlen(zrNum);
            result += strlen(zrNum);
            free(zrNum); // Освобождаем выделенную память
        }
        else if (*formatPtr == '%' && formatPtr[1] == 'C' && (formatPtr[2] == 'v' || formatPtr[2] == 'V'))
        {
            int flag_big_or_small;
            if (formatPtr[2] == 'V')
            {
                flag_big_or_small = 1;
            }
            else
            {
                flag_big_or_small = 0;
            }
            formatPtr +=3;
            int num = va_arg(args,int);
            int base = va_arg(args,int);
            if (base > 36 || base < 2)
            {
                base = 10;
                int len_num = len_int(num);
                char temp[len_num];
                oversprintf(temp,"%d",num);
                strPtr += len_num;
                result += len_num;
            }
            else
            {
                char* num_with_base = (char*)malloc((10*len_int(num)+1)*sizeof(char));
                ten_to_base(num,base,num_with_base,flag_big_or_small);
                strcpy(strPtr,num_with_base);
                strPtr += strlen(num_with_base);
                result += strlen(num_with_base);
                free(num_with_base);
            }
        }
        else {
            *strPtr = ch;
            strPtr++;
            result++;
            formatPtr++;
        }
    }

    *strPtr = '\0';

    va_end(args);
    return result;
}

int main() {
    int number = 42;
    unsigned int zrNumber = 24;
    char romanNumber[50];
    char zrString[50];
    char cv_up_string[50];
    char cv_lower_string[50];
    int cv_up_number = 66075;
    int cv_lower_number = 45512;
    int base1 = 16;
    int base2 = 16;

    overfprintf(stdout, "The Roman numeral for 42 is %Ro\n", number);
    oversprintf(romanNumber, "The Roman numeral for 42 is %Ro", number);
    printf("%s\n", romanNumber);

    overfprintf(stdout, "The Zr representation of 24 is %Zr\n", zrNumber);
    oversprintf(zrString, "The Zr representation of 24 is %Zr", zrNumber);
    printf("%s\n", zrString);

    printf("\nNum:%d Base:%d\n",cv_lower_number,base1);
    overfprintf(stdout, "The Cv result: %Cv\n", cv_lower_number,base1);
    oversprintf(cv_lower_string, "The Cv result: %Cv", cv_lower_number,base1);
    printf("%s\n",cv_lower_string);

    printf("\nNum:%d Base:%d\n",cv_up_number, base2);
    overfprintf(stdout, "The CV result: %CV\n", cv_up_number,base2);
    oversprintf(cv_up_string, "The CV result: %CV", cv_up_number,base2);
    printf("%s\n",cv_up_string);

    return 0;
}
