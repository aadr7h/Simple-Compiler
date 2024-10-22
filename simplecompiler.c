#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

int pos = 0; // to track the current position in the input expression
char current_char; //holds the currrent character being parsed

//function prototypes
void error(const char* message);
int expression(const char* input);
int term(const char* input);
int factor(const char* input);
void advance(const char* input);
void skip_whitespace(const char* input);

//function to handle errors
void error(const char* message) {
	printf("Error: %s\n", message);
	exit(EXIT_FAILURE);
}

//function to move to the next character in the input 
void advance(const char* input) {
	current_char = input[pos];
	pos++;
}

//function to skip whitespace
void skip_whitespace(const char* input) {
	while (current_char != '\0' && isspace(current_char)) {
		advance(input);
	}
}

// persing a 'factor' (either a number or an expression in parentheses)
int factor(const char* input) {
	int result = 0;
	skip_whitespace(input);
	
	if (isdigit(current_char)) {
		// if it's a number, parse it
		result = current_char - '0';
		advance(input);
		while (isdigit(current_char)) {
			result = result * 10 + (current_char - '0');
			advance(input);
		}
	} else if (current_char == '(') {
		//if it's a paremthesis evaluate the exression the expression inside
		advance(input); //skip '('
		result = expression(input);
		if (current_char == ')') {
			advance(input); //skip ')'
		} else {
			error("Expected ')'");
		}
	} else {
		error("Expected number or '('");
	}
	return result;
}

//parsing a "term" (factor combined with * or /)
int term(const char* input) {
	int result = factor(input);
	skip_whitespace(input);
	
	while (current_char == '*' || current_char == '/') {
		char operator = current_char;
		advance(input);
		
		int right = factor(input);
		
		if (operator == '*') {
			result *= right;
		} else if (operator == '/') {
			if (right == 0) {
				error("Division by zero");
			}
			result /= right;
		}
	}
	return result;
}

// parsing an 'expression' (term combined with + or -)
int expression(const char* input) {
	int result = term(input);
	skip_whitespace(input);
	
	while (current_char == '+' || current_char == '-') {
		char operator = current_char;
		advance(input);
		
		int right = term(input);
		
		if (operator == '+') {
			result += right;
		} else if (operator == '-') {
			result -= right;
		}
	}
	return result;
}

// main function to interpret the input expression
int interpret(const char* input) {
	pos = 0;
	advance(input); //initialize the first character
	int result = expression(input);
	
	// if there is a still input left , it's an error
	if (current_char != '\0') {
		error("Unexpected character");
	}
	
	return result;
}

// main program
int main() {
	char input[100];
	
	printf("Enter an expression (e.g., 3 + 5 * (2 - 4) / 2): ");
	fgets(input, sizeof(input), stdin);
	
	// evaluate and print the result
	int result = interpret(input);
	printf("Result: %d\n", result);
	
	return 0;
}
