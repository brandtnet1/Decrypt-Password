// Demonstration of the crypt() function

// Compile this program using: 
//     gcc -Wall -o cryptExample cryptExample.c -lcrypt
// -lcrypt is required to link in the crypt library

#include <stdio.h>
#include <unistd.h>
#include <crypt.h>

int main() {
    
    char *key = "password";
    char *salt = "$1$";  // Always use "$1$" for the salt string
    char *hash = crypt(key, salt);
    
    printf("%s\n", hash);
    
    return 0;
}