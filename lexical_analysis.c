#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to check if a character is a valid letter
int isLetter(char ch) {
    return isalpha(ch) || ch == '_';
}

// Function to check if a character is a valid digit
int isDigit(char ch) {
    return isdigit(ch);
}

// Function to check if a character is a valid operator
int isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

// Function to check if a character is a special symbol
int isSpecialSymbol(char ch) {
    return ch == '!' || ch == '@' || ch == '#' || ch == '$' || ch == '%' || ch == '^' || ch == '=';
}

// Function to check if a string is a keyword
int isKeyword(char *str) {
    char keywords[12][20] = {
        "if",
        "else",
        "while",
        "for",
        "int",
        "double",
        "float",
        "printf",
        "long",
        "short",
        "void",
        "char"
    };
    
    int i;
    for (i = 0; i < 12; i++) {
        if (strcmp(str, keywords[i]) == 0)
            return 1; // Keyword found
    }
    
    return 0; // Keyword not found
}

// Function to perform lexical analysis and print tokens with their categories
void lexicalAnalysis(char *input) {
    int len = strlen(input);
    int i = 0;
    
    printf("Token\t\tCategory\n");
    printf("------------------------------\n");
    
    while (i < len) {
        // Check if character is a letter (identifier or keyword)
        if (isLetter(input[i])) {
            int j = i;
            while (isLetter(input[j]) || isDigit(input[j]))
                j++;
            
            char token[20];
            strncpy(token, &input[i], j - i);
            token[j - i] = '\0';
            
            if (isKeyword(token))
                printf("%s\t\tKeyword\n", token);
            else
                printf("%s\t\tIdentifier\n", token);
            
            i = j;
        }
        
        // Check if character is a digit (integer)
        else if (isDigit(input[i])) {
            int j = i;
            while (isDigit(input[j]))
                j++;
            
            printf("%.*s\t\tInteger\n", j - i, &input[i]);
            i = j;
        }
        
        // Check if character is an operator
        else if (isOperator(input[i])) {
            printf("%c\t\tOperator\n", input[i]);
            i++;
        }
        
        // Check if character is a special symbol
        else if (isSpecialSymbol(input[i])) {
            printf("%c\t\tSpecial Symbol\n", input[i]);
            i++;
        }
        
        // Check if character is a whitespace (delimiter)
        else if (isspace(input[i])) {
            printf("%c\t\tDelimiter\n", input[i]);
            i++;
        }
        
        // Invalid character
        else {
            printf("%c\t\tInvalid\n", input[i]);
            i++;
        }
    }
    
    printf("------------------------------\n");
}

int main() {
    char input[100];
    
    printf("Enter the input string: ");
    fgets(input, sizeof(input), stdin);
    
    // Remove the trailing newline character
    if (input[strlen(input) - 1] == '\n')
        input[strlen(input) - 1] = '\0';
    
    lexicalAnalysis(input);
    
    return 0;
}
