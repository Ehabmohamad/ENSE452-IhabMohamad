#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Define the Complex number structure
typedef struct {
    double real;
    double imag;
} Complex;

// Function prototypes
void addFunction(Complex z1, Complex z2, Complex *result);
void subtractFunction(Complex z1, Complex z2, Complex *result);
void multiplyFunction(Complex z1, Complex z2, Complex *result);
void divideFunction(Complex z1, Complex z2, Complex *result, int *error);
int parse_input(char *input, char *command, Complex *z1, Complex *z2);

int main() {
    char input[100];
    char command;
    Complex z1, z2, result;
    int error_code = 0;

        // Display instructions and supported operations if not in test mode
        fprintf(stderr,"==================\n");
        fprintf(stderr, "Complex number calculator,with this calculatoryou can proform various operation. \n");
        fprintf(stderr, "This calculator can  supports standard scientific e notation (Ex: 4e2), numbers with adecimal point (Ex: 4.2), and integers (Ex: 4).\n");
        fprintf(stderr, "The Supported Aperations:\n");
        fprintf(stderr, "  - Enter (a or A) For Addition \n");
        fprintf(stderr, "  - Enter (s or S) For Subtraction \n");
        fprintf(stderr, "  - Enter (m or M) For Multiplication \n");
        fprintf(stderr, "  - Enter (d or D) For Division \n");
        fprintf(stderr, "  - Enter (q or Q) To Quit \n");
        fprintf(stderr, "Please enter your operation and complex numbers in the following format: <operation (a or A, s or S, m or M, d or D)> <real> <imaginary> <real> <imaginary> \n");
        fprintf(stderr, "For example: 'A 45.67 -17e1 9.2 15' for addition of (45.67 + -17e1) and (9.2 + 15).\n");
        fprintf(stderr, "To quit the calculator You can simply enter 'q'.\n");
        fprintf(stderr, "==================\n");
        
    while (1) {

        fprintf(stderr, "Enter exp: ");
        fgets(input, sizeof(input), stdin);
        
        // Ignore empty lines
        if (strcmp(input, "\n") == 0 || strlen(input) == 0) continue;

        // Parse input and check for errors
        error_code = parse_input(input, &command, &z1, &z2);

        if (error_code == 1) {
            printf("error code: 1: illegal command\n");
            continue;
        } else if (error_code == 2) {
            printf("error code: 2: missing arguments\n");
            continue;
        } else if (error_code == 3) {
            printf("error code: 3: extra arguments\n");
            continue;
        }

        // Handls the commands (A, S, M, D, Q)
        switch (tolower(command)) {
            case 'a':  // Addition
                addFunction(z1, z2, &result);
                printf("%.6f + j %.6f\n", result.real, result.imag);
                break;
            case 's':  // Subtraction
                subtractFunction(z1, z2, &result);
                printf("%.6f + j %.6f\n", result.real, result.imag);
                break;
            case 'm':  // Multiplication
                multiplyFunction(z1, z2, &result);
                printf("%.6f + j %.6f\n", result.real, result.imag);
                break;
            case 'd':  // Division
                divideFunction(z1, z2, &result, &error_code);
                if (error_code == 4) {
                    printf("error code: 4: divide by zero\n");
                } else {
                    printf("%.6f + j %.6f\n", result.real, result.imag);
                }
                break;
            case 'q':  // Quit
                return 0;
            default:
                printf("error code: 1: illegal command\n");
                break;
        }
    }
}

// Function to do the addition 
void addFunction(Complex z1, Complex z2, Complex *result) {
    result->real = z1.real + z2.real;
    result->imag = z1.imag + z2.imag;
}
// Function to do the subtraction
void subtractFunction(Complex z1, Complex z2, Complex *result) {
    result->real = z1.real - z2.real;
    result->imag = z1.imag - z2.imag;
}
// Function to do the multiplrtion 
void multiplyFunction(Complex z1, Complex z2, Complex *result) {
    result->real = z1.real * z2.real - z1.imag * z2.imag;
    result->imag = z1.real * z2.imag + z1.imag * z2.real;
}
// Function to do the division
void divideFunction(Complex z1, Complex z2, Complex *result, int *error) {
    double denominator = z2.real * z2.real + z2.imag * z2.imag;
    if (denominator == 0) {
        *error = 4;
        return;
    }
    result->real = (z1.real * z2.real + z1.imag * z2.imag) / denominator;
    result->imag = (z1.imag * z2.real - z1.real * z2.imag) / denominator;
}

int parse_input(char *input, char *command, Complex *z1, Complex *z2) {
    // Array to hold additional arguments beyond expected
    char extra[100];
    
    int args = sscanf(input, " %c %lf %lf %lf %lf %s", command, &z1->real, &z1->imag, &z2->real, &z2->imag, extra);
    
    if (args < 1) return 1;  // Illegal command
    if (tolower(*command) == 'q' && args > 1) return 3;  // I entered any Extra arguments with quit
    if (tolower(*command) == 'q') return 0;  // Cammand to Quit 
    if (args < 5) return 2;  // If Missing Arguments
    if (args > 5) return 3;  // If Entered Extra Arguments

    return 0;  // No error
}
