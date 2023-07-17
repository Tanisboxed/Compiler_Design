#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Structure representing a state in the NFA
typedef struct {
    int id;
    bool isInitial;
    bool isFinal;
} State;

// Structure representing a transition in the NFA
typedef struct {
    int currentState;
    char input;
    int nextState;
} Transition;

// Function to add a transition to the transition table
void addTransition(Transition* transitionTable, int* transitionCount, int currentState, char input, int nextState) {
    Transition transition;
    transition.currentState = currentState;
    transition.input = input;
    transition.nextState = nextState;

    transitionTable[*transitionCount] = transition;
    (*transitionCount)++;
}

// Function to construct the NFA using Thompson's construction method
void constructNFA(char* regex, int* initialStateSet, int* finalStateSet, Transition* transitionTable, int* transitionCount) {
    int stateCounter = 0;
    int currentState, nextState;

    currentState = stateCounter++;
    initialStateSet[0] = currentState;

    int regexIndex = 0;
    char currentChar, nextChar;

    while ((currentChar = regex[regexIndex++]) != '\0') {
        if (regex[regexIndex] == '*') {
            nextState = stateCounter++;

            // Add epsilon transition from current state to next state
            addTransition(transitionTable, transitionCount, currentState, '\0', nextState);

            // Add epsilon transition from next state back to current state
            addTransition(transitionTable, transitionCount, nextState, '\0', currentState);
        }
        else if (regex[regexIndex] == '|') {
            nextState = stateCounter++;
            int branchStartState = stateCounter++;

            // Add epsilon transition from current state to branch start state
            addTransition(transitionTable, transitionCount, currentState, '\0', branchStartState);

            // Add epsilon transition from branch start state to next state
            addTransition(transitionTable, transitionCount, branchStartState, '\0', nextState);

            // Add epsilon transition from current state to next state
            addTransition(transitionTable, transitionCount, currentState, '\0', nextState);

            currentState = nextState;
        }
        else if (regex[regexIndex] == '(') {
            // Find the closing ')' to extract the sub-expression
            int subExpressionStart = regexIndex;
            int subExpressionEnd = subExpressionStart;
            int openBrackets = 1;

            while (openBrackets > 0) {
                char subChar = regex[++subExpressionEnd];
                if (subChar == '(')
                    openBrackets++;
                else if (subChar == ')')
                    openBrackets--;
            }

            // Recursively construct NFA for the sub-expression
            char* subExpression = (char*)malloc(sizeof(char) * (subExpressionEnd - subExpressionStart));
            strncpy(subExpression, regex + subExpressionStart, subExpressionEnd - subExpressionStart - 1);
            subExpression[subExpressionEnd - subExpressionStart - 1] = '\0';

            int subExpressionInitialStateSet[100];
            int subExpressionFinalStateSet[100];
            int subExpressionTransitionCount = 0;
            Transition subExpressionTransitionTable[100];

            constructNFA(subExpression, subExpressionInitialStateSet, subExpressionFinalStateSet, subExpressionTransitionTable, &subExpressionTransitionCount);

            // Add epsilon transitions from current state to sub-expression initial states
            for (int i = 0; i < subExpressionFinalStateSet[0]; i++) {
                addTransition(transitionTable, transitionCount, currentState, '\0', subExpressionInitialStateSet[i]);
            }

            // Add epsilon transitions from sub-expression final states to next state
            nextState = stateCounter++;
            for (int i = 0; i < subExpressionFinalStateSet[0]; i++) {
                addTransition(transitionTable, transitionCount, subExpressionFinalStateSet[i], '\0', nextState);
            }

            free(subExpression);

            currentState = nextState;
            regexIndex = subExpressionEnd;
        }
        else {
            nextState = stateCounter++;

            // Add transition from current state to next state with the input character
            addTransition(transitionTable, transitionCount, currentState, currentChar, nextState);

            currentState = nextState;
        }
    }

    // Set the final state
    finalStateSet[0] = stateCounter++;
}

// Function to print the NFA
void printNFA(int* initialStateSet, int* finalStateSet, Transition* transitionTable, int transitionCount) {
    printf("\nInitial State Set:\n");
    printf("State Set: %d\n", initialStateSet[0]);

    printf("\nFinal State Set:\n");
    printf("State Set: %d", finalStateSet[0]);
    
    printf("\n\nTransition Table:\n");
    printf("Current State\tInput\tNext State\n");
    printf("------------------------------------\n");

    for (int i = 0; i < transitionCount; i++) {
        Transition transition = transitionTable[i];
        printf("%d\t\t%c\t%d\n", transition.currentState, transition.input, transition.nextState);
    }

    printf("------------------------------------\n");
}

int main() {
    char regex[100];

    printf("Enter the regular expression: ");
    scanf("%s", regex);

    int initialStateSet[100];
    int finalStateSet[100];
    int transitionCount = 0;
    Transition transitionTable[100];

    constructNFA(regex, initialStateSet, finalStateSet, transitionTable, &transitionCount);

    printNFA(initialStateSet, finalStateSet, transitionTable, transitionCount);

    return 0;
}
