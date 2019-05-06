// Copy contents from one file to another

// Specify input and output file names using the command line:
#include <crypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char* toggleLetterCase(char buffer[]) { // Turns capital letters into lowercase and the inverse 
    if (buffer[0]>='A' && buffer[0]<='Z')
        buffer[0] = buffer[0] + 32;
    else if (buffer[0]>='a' && buffer[0]<='z')
        buffer[0] = buffer[0] - 32;
    
    return buffer;
}

char* toggleLeet(char *buffer) { // replaces letters with their leet counterparts
     int i = 0;
     
     while(buffer[i] != '\0') {
           if(buffer[i] == 'a')
               buffer[i] = '@';
           else if(buffer[i] == 'e')
               buffer[i] = '3';
           else if(buffer[i] == 'i') 
               buffer[i] = '!';
           else if(buffer[i] == 'l')
               buffer[i] = '1';
           else if(buffer[i] == 'o') 
               buffer[i] = '0';
           else if(buffer[i] == 's')
               buffer[i] = '5';

           i++; 
     }
     return buffer;
}

void crack(char *hash, char *dictionaryFileName, char *outputFileName, int n, int c, int l){
    FILE *output = fopen(outputFileName, "a");
    FILE *dictionary = fopen(dictionaryFileName, "r");
    
    char buffer[128];
    char *cryptedText = malloc(sizeof(buffer));

    while(fgets(buffer, sizeof(buffer), dictionary)) { // While the dictionary isnt empty
        if(buffer[strlen(buffer) - 1] == '\n') // Take first line and stirp new line
            buffer[strlen(buffer) - 1] = '\0';
            
        if(n == 1 && c == 0 && l == 0){ // If -n only
            int i;
            int b = 0;
            int len = strlen(buffer);
            for(i = 0; i < 10; i++){
                buffer[len] = i + '0';
                buffer[len + 1] = '\0';
                cryptedText = crypt(buffer, "$1$");

                if(strcmp(cryptedText, hash) == 0){
                    fprintf(output, "%s\n", buffer);
                    b = 1;
                    break;
                }
            }
            if(b == 1)
                break;
        }
        else if(c == 1 && n == 0 && l == 0){ // If -c only
            char *newBuffer = toggleLetterCase(buffer);
            cryptedText = crypt(newBuffer, "$1$");
            
            if(strcmp(cryptedText, hash) == 0){
                fprintf(output, "%s\n", newBuffer);
                break;
            }
        }
        else if(l == 1 && n == 0 && c == 0){ // If -l only
            char *newBuffer = toggleLeet(buffer);
            cryptedText = crypt(newBuffer, "$1$");

            if(strcmp(cryptedText, hash) == 0){
                fprintf(output, "%s\n", newBuffer);
                break;
            }
        }
        else if(c == 1 && n == 1 && l == 0){ // If -c and -n
            char *newBuffer = toggleLetterCase(buffer);
            cryptedText = crypt(newBuffer, "$1$");
            
            int i;
            int b = 0;
            int len = strlen(newBuffer);
            for(i = 0; i < 10; i++){
                newBuffer[len] = i + '0';
                newBuffer[len + 1] = '\0';
                cryptedText = crypt(newBuffer, "$1$");

                if(strcmp(cryptedText, hash) == 0){
                    fprintf(output, "%s\n", newBuffer);
                    b = 1;
                    break;
                }
            }
            if(b == 1)
                break;
        }
        else if(c == 1 && n == 0 && l == 1){ // If -c and -l
            char *newBuffer = toggleLetterCase(buffer);
            cryptedText = crypt(newBuffer, "$1$");
            
            newBuffer = toggleLeet(newBuffer);
            cryptedText = crypt(newBuffer, "$1$");

            if(strcmp(cryptedText, hash) == 0){
                fprintf(output, "%s\n", newBuffer);
                break;
            }
        }
        else if(c == 0 && n == 1 && l == 1){
            char *newBuffer = toggleLeet(buffer);
            cryptedText = crypt(newBuffer, "$1$");
            
            int i;
            int b = 0;
            int len = strlen(newBuffer);
            for(i = 0; i < 10; i++){
                newBuffer[len] = i + '0';
                newBuffer[len + 1] = '\0';
                cryptedText = crypt(newBuffer, "$1$");

                if(strcmp(cryptedText, hash) == 0){
                    fprintf(output, "%s\n", newBuffer);
                    b = 1;
                    break;
                }
            }
            if(b == 1)
                break;
        }
        else if(c == 1 && n == 1 && l == 1){ // if -c, -n and -l
            char *newBuffer = toggleLetterCase(newBuffer);
            cryptedText = crypt(newBuffer, "$1$");
            
            newBuffer = toggleLeet(newBuffer);
            cryptedText = crypt(newBuffer, "$1$");
            
            int i;
            int b = 0;
            int len = strlen(newBuffer);
            for(i = 0; i < 10; i++){
                newBuffer[len] = i + '0';
                newBuffer[len + 1] = '\0';
                cryptedText = crypt(newBuffer, "$1$");

                if(strcmp(cryptedText, hash) == 0){
                    fprintf(output, "%s\n", newBuffer);
                    b = 1;
                    break;
                }
            }
            if(b == 1)
                break;
        }
        else if(n == 0 && c == 0 && l == 0){ // if no additional command line args
            cryptedText = crypt(buffer, "$1$");
            
            if(strcmp(cryptedText, hash) == 0){
                fprintf(output, "%s\n", buffer);
                break;
            }  
        }
    }
    
    fclose(output);
    fclose(dictionary);
}


int main(int argc, char *argv[]){
    // Extract input and output files with getopt
    char *inputFileName;
    char *outputFileName;
    char *dictionaryFileName;
    
    int appendDigit = 0;
    int toggleCase = 0;
    int leetStyle = 0;
    
    char c;
    
    while((c = getopt(argc, argv, "i:o:d:ncl")) != -1){
        switch(c){
            case 'i':
                inputFileName = optarg;
                break;
            case 'o':
                outputFileName = optarg;
                break;
            case 'd':
                dictionaryFileName = optarg;
                break;
            case 'n':
                appendDigit = 1;
                break;
            case 'c':
                toggleCase = 1;
                break;
            case 'l':
                leetStyle = 1;
                break;
        }
    }
    
    FILE *inputFile = fopen(inputFileName, "r");
    
    char buffer[128];
    while(fgets(buffer, sizeof(buffer), inputFile)){
        char *user = strtok(buffer, ":");
        
        char *hash = strtok(NULL, ":");
        
        if(hash[strlen(hash) - 1] == '\n')
            hash[strlen(hash) - 1] = '\0';
            
        crack(hash, dictionaryFileName, outputFileName, appendDigit, toggleCase, leetStyle);
    }
    
    fclose(inputFile);
    
    return 0;
}