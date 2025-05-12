/*============================================================================
 |   Assignment:  pa01 - Encrypting a plaintext file using the Hill cipher
 |
 |       Author:  Isabel Barragan
 |     Language:  C
 |   To Compile:  gcc -o pa01 pa01.c
 |   To Execute:  ./pa01 k#.txt p#.txt
 |                where k#.txt is the keytext file
 |                and p#.txt is the plaintext file
 |         Note:
 |                All input files are simple 8 bit ASCII input
 |                All execute commands above have been tested on Eustis
 |
 |  
 |
 |
 |
 |
 |
 +============================================================================*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MATRIX_SIZE_MAXIMUM 9
#define TEXT_SIZE_MAXIMUM 10000

//Read and store the matrix from the key file, making sure the matrix is valid, in every factor, as instructed
void matrix(const char* fKeyPathway, int matrix[MATRIX_SIZE_MAXIMUM][MATRIX_SIZE_MAXIMUM], int *size) 
{
    //Open the key file, if not exit 
    FILE* keyFile = fopen(fKeyPathway, "r");
    if (keyFile == 0) {
        printf("Could not open the key file.\n");
        exit(1);
    }

    //Read the matrix size
    fscanf(keyFile, "%d", size);  

    //Determine if the matrix size is valid, if not exit
    if (*size <= 0 || *size > MATRIX_SIZE_MAXIMUM) {      
        printf("Matrix size in the key file is not valid.\n"); 
        fclose(keyFile);
        exit(1);
    }

    //Read the matrix values from the files and put the values in the matrix array if plausible
    for (int i = 0; i < *size; i++) {
        for (int j = 0; j < *size; j++) {
            if (fscanf(keyFile, "%d", &matrix[i][j]) != 1) {
                printf("Matrix data is incomplete in the key file.\n");
                fclose(keyFile);
                exit(1);
            }
        }
    }

    //Close the key file when the matrix has loaded successfully
    fclose(keyFile);
}

//Read and process plaintext from file, include adding padding as needed using "x"
void plaintext(const char* fPlainPathway, char* outputText, int n)
{
    //Open the plaintext file, if not exit
    FILE* textFile = fopen(fPlainPathway, "r");
    if (textFile == 0) {
        printf("Could not open the plaintext file.\n");
        exit(1);
    }

    char buffer;
    int length = 0;

    //Extract the alphabetic characters and convert them from uppercase letters to lowercase letters (isalpha), read the data character by character
    while ((buffer = fgetc(textFile)) != EOF) {       //Not equal to the end of file
        if (isalpha(buffer)) {
            outputText[length++] = tolower(buffer);
        }
    }

    fclose(textFile);

    //Add padding (x) if needed when the length of the plaintext is not a multiple of the matrix size
    while (length % n != 0) {
        outputText[length++] = 'x';
    }

    //Null-terminate, this is officially marking the end of the string
    outputText[length] = '\0';  
}

//Perform matrix multiplication for a block of plaintext
void matrixMultiplication(int key[MATRIX_SIZE_MAXIMUM][MATRIX_SIZE_MAXIMUM], char* splitTheBlock, int* result, int n)
{
    int textWord[MATRIX_SIZE_MAXIMUM];

    //Convert each character to its integer value/representation (a = 0, b = 1, and so on all the way through z = 25)
    for (int i = 0; i < n; i++) {
        textWord[i] = splitTheBlock[i] - 'a';
    }

    //Perform matrix multiplication using the modulo 26 arithmetic 
    for (int i = 0; i < n; i++) {
        int sum = 0;
        for (int j = 0; j < n; j++) {
            sum += key[i][j] * textWord[j];
        }
        //Use the modulo 26 arithmetic to bring the result within the appropriate range of letters (0-25)
        result[i] = sum % 26;  
    }
}

//Encrypt the entire plaintext using matrix multiplication in blocks of n
void encryption(int key[MATRIX_SIZE_MAXIMUM][MATRIX_SIZE_MAXIMUM], const char* plaintext, char* ciphertext, int n)
{
    int length = strlen(plaintext);
    char block[MATRIX_SIZE_MAXIMUM];
    int result[MATRIX_SIZE_MAXIMUM];
    int position = 0;

    //Iterate through the plaintext in blocks of n-length
    for (int i = 0; i < length; i += n) {
        //Copy the n-length block
        strncpy(block, plaintext + i, n);
        block[n] = '\0';

        matrixMultiplication(key, block, result, n);

        //Convert the numerical results back to characters and store them in the ciphertext
        for (int j = 0; j < n; j++) {
            ciphertext[position++] = result[j] + 'a';
        }
    }

    //Null-terminate, this is officially marking the end of the string
    ciphertext[position] = '\0'; 
}

//Print and format the matrix, plaintext, and ciphertext accordingly
void formatting(int key[MATRIX_SIZE_MAXIMUM][MATRIX_SIZE_MAXIMUM], const char* plaintext, const char* ciphertext, int n) 
{
    //Print and display the key matrix, using the correct spacing as hinted "%4d"
    printf("\nKey matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d", key[i][j]);
        }
        printf("\n");
    }

    //Print and display the plaintext
    printf("\nPlaintext:\n");
    for (int i = 0; plaintext[i]; i++) {
        printf("%c", plaintext[i]);
        if ((i + 1) % 80 == 0) {
            printf("\n");    //Produce a new line after every 80 characters per row 
        }
    }
    printf("\n");

    //Print and display the ciphertext
    printf("\nCiphertext:\n");
    for (int i = 0; ciphertext[i]; i++) {
        printf("%c", ciphertext[i]);
        if ((i + 1) % 80 == 0) {
            printf("\n");    //Produce a new line after every 80 characters per row 
        }
    }
    printf("\n");
}

//Main function with two command-line arguments for the encryption key file and the plaintext file. Argument count and argument vector
int main(int argc, char *argv[]) 
{
    //Ensures there are a correct number of arguments being passed, two command-line arguments
    if (argc != 3) {
        printf("Invalid number of command-line arguments being passed.\n");
        return 1;
    }

    int key[MATRIX_SIZE_MAXIMUM][MATRIX_SIZE_MAXIMUM];
    int matrixSize;
    char plaintextSize[TEXT_SIZE_MAXIMUM];
    char ciphertext[TEXT_SIZE_MAXIMUM];

    //The first argument that will load and process the matrix from the provided files
    matrix(argv[1], key, &matrixSize);

    //The second argument that will load and process the plaintext from the provided files
    plaintext(argv[2], plaintextSize, matrixSize);

    //Encrypt the plaintext using the key matrix
    encryption(key, plaintextSize, ciphertext, matrixSize);

    //Displaying the matrix, plaintext, and ciphertext appropriately
    formatting(key, plaintextSize, ciphertext, matrixSize);

    return 0;
}


/*=============================================================================
 |     I Isabel Barragan (is958647) affirm that this program is 
 | entirely my own work and that I have neither developed my code together with 
 | any another person, nor copied any code from any other person, nor permitted
 | my code to be copied or otherwise used by any other person, nor have I
 | copied, modified, or otherwise used programs created by others. I acknowledge
 | that any violation of the above terms will be treated as academic dishonesty.
 +=============================================================================*/