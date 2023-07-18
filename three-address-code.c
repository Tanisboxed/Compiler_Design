#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BLOCKS 10  // Maximum number of basic blocks

typedef struct {
    char code[20];  // Three-address code line
} BasicBlock;

int tempCount = 0;  // Counter for temporary variables

void generateThreeAddressCode(char expression[]) {
    int len = strlen(expression);

    char result[4];
    char operand1[4];
    char operand2[4];

    int codeCount = 0;  // Counter for code lines
    char code[100][20]; // Array to store three-address code lines

    // Parsing the first operand
    operand1[0] = expression[0];
    operand1[1] = '\0';

    int j = 0;
    // Parsing the operator and the second operand
    for (int i = 1; i < len - 1; i += 2) {
        char operator = expression[i];
        operand2[0] = expression[i + 1];
        operand2[1] = '\0';

        // Assign the result to the temporary variable
        sprintf(result, "T%d", tempCount++);

        // Generate the three-address code line
        sprintf(code[codeCount++], "%s = %s %c %s", result, operand1, operator, operand2);

        // The result becomes the new operand1 for the next operation
        strcpy(operand1, result);
        j = i + 1;
    }

    // Print the generated three-address code
    printf("\nThree-Address Code:\n");
    for (int i = 0; i < codeCount; i++) {
        printf("T%d: %s\n", i, code[i]);
    }

    // Identify basic blocks based on control flow
    BasicBlock blocks[MAX_BLOCKS];
    int blockCount = 0;

    int leaderCount = 0;
    int leaders[MAX_BLOCKS];

    leaders[leaderCount++] = 0;
    leaders[leaderCount++] = codeCount - 1;

    // Print the basic blocks
    printf("\nBasic Blocks:\n");
    for (int i = 0; i < leaderCount - 1; i++) {
        printf("Block %d:\n", i + 1);
        for (int j = leaders[i]; j <= leaders[i + 1]; j++) {
            printf("%s\n", code[j]);
        }
        printf("\n");
    }

    // Print the leaders
    printf("Leaders:\n");
    for (int i = 0; i < leaderCount; i++) {
        printf("Line %d\n", leaders[i]);
    }
}

int main() {
    char expression[20];
    printf("Enter the expression (e.g., a + b): ");
    fgets(expression, sizeof(expression), stdin);

    // Remove the newline character from the expression
    expression[strcspn(expression, "\n")] = '\0';

    generateThreeAddressCode(expression);

    return 0;
}
