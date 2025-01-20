#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Define Token Types
typedef enum {
    NUMBER, PLUS, MINUS, MULTIPLY, DIVIDE, LPAREN, RPAREN, END, INVALID
} TokenType;

// Define Token Structure
typedef struct {
    TokenType type;
    int value;
} Token;

char *input;
Token currentToken;

void getNextToken();
int evaluateExpression();
int evaluateTerm();
int evaluateFactor();
void throwError(const char *message);

// Function to Fetch the Next Token
void getNextToken() {
    // Skip spaces
    while (isspace(*input)) input++;

    if (*input == '\0') { // End of input
        currentToken.type = END;
        return;
    }

    if (isdigit(*input)) { // Handle numbers
        currentToken.type = NUMBER;
        currentToken.value = 0;
        while (isdigit(*input)) {
            currentToken.value = currentToken.value * 10 + (*input - '0');
            input++;
        }
        return;
    }

    // Handle operators and parentheses
    switch (*input) {
        case '+': currentToken.type = PLUS; input++; break;
        case '-': currentToken.type = MINUS; input++; break;
        case '*': currentToken.type = MULTIPLY; input++; break;
        case '/': currentToken.type = DIVIDE; input++; break;
        case '(': currentToken.type = LPAREN; input++; break;
        case ')': currentToken.type = RPAREN; input++; break;
        default: currentToken.type = INVALID; input++; break;
    }
}

// Throw Error and Exit
void throwError(const char *message) {
    printf("Error: %s\n", message);
    exit(EXIT_FAILURE);
}

// Parse and Evaluate an Expression (Handles +, -)
int evaluateExpression() {
    int result = evaluateTerm();

    while (currentToken.type == PLUS || currentToken.type == MINUS) {
        TokenType operator = currentToken.type;
        getNextToken();
        int nextValue = evaluateTerm();

        if (operator == PLUS) {
            result += nextValue;
        } else {
            result -= nextValue;
        }
    }

    return result;
}

// Parse and Evaluate a Term (Handles *, /)
int evaluateTerm() {
    int result = evaluateFactor();

    while (currentToken.type == MULTIPLY || currentToken.type == DIVIDE) {
        TokenType operator = currentToken.type;
        getNextToken();
        int nextValue = evaluateFactor();

        if (operator == MULTIPLY) {
            result *= nextValue;
        } else {
            if (nextValue == 0) {
                throwError("Division by zero is not allowed.");
            }
            result /= nextValue;
        }
    }

    return result;
}

// Parse and Evaluate a Factor (Handles numbers and parentheses)
int evaluateFactor() {
    if (currentToken.type == NUMBER) {
        int value = currentToken.value;
        getNextToken();
        return value;
    } else if (currentToken.type == LPAREN) {
        getNextToken();
        int value = evaluateExpression();
        if (currentToken.type != RPAREN) {
            throwError("Missing closing parenthesis.");
        }
        getNextToken();
        return value;
    } else {
        throwError("Invalid input.");
        return 0; // Unreachable
    }
}

// Main Function
int main() {
    char inputBuffer[256];

    printf("Enter an expression: ");
    if (!fgets(inputBuffer, sizeof(inputBuffer), stdin)) {
        throwError("Failed to read input.");
    }

    input = inputBuffer;
    getNextToken();

    int result = evaluateExpression();

    if (currentToken.type != END) {
        throwError("Unexpected input at the end.");
    }
    if(result){
        printf("Valid Expression\n");
        printf("Result: %d\n", result);
    }
    return 0;
}
