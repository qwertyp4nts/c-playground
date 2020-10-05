#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <wctype.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1; //exit
    }

    if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1; //exit
    }

    if (strlen(argv[1]) == 26)
    {
        //check validity
        for (int i = 0; i < 26; i++)
        {
            if (isalpha(argv[1][i]) == 0) //if the char is not alpha
            {
                printf("Key must only contain alpha characters.\n");
                return 1; //exit
            }

            int checkBothCases = 0;

            for (int o = 0; o < 26; o++)
            {

                if (iswlower(argv[1][i]) != 0) //means its lowercase
                {
                    checkBothCases = toupper(argv[1][i]);
                }

                if (iswupper(argv[1][i]) != 0) //means its uppercase
                {
                    checkBothCases = tolower(argv[1][i]);
                }


                if (argv[1][i] == argv[1][o] || checkBothCases == argv[1][o])
                {
                    if (i != o)
                    {
                        printf("Key contains duplicates.\n");
                        return 1; //exit
                    }
                }
            }
        }
    }

    string plaintext = get_string("plaintext: ");
    int inputStringLength = strlen(plaintext);
    int adderArray[inputStringLength];
    int caseCheck = 65; //uppercase by default
    int keyCaseCheck = 65; //uppercase by default

    printf("ciphertext: ");
    for (int i = 0; i < inputStringLength; i++)
    {
        if (isalpha(plaintext[i]) != 0) //if the char is alpha
        {
            if (iswlower(plaintext[i]) != 0) //means its lowercase
            {
                caseCheck = 97;
            }
            if (iswupper(plaintext[i]) != 0) //means its uppercase
            {
                caseCheck = 65;
            }

            int alphabetArrayPos = plaintext[i] - caseCheck;
            //printf("%d ", alphabetArrayPos);

            if (iswlower(argv[1][alphabetArrayPos]) != 0) //means its lowercase
            {
                keyCaseCheck = 97;
            }

            if (iswupper(argv[1][alphabetArrayPos]) != 0) //means its uppercase
            {
                keyCaseCheck = 65;
            }

            adderArray[i] = plaintext[i] - caseCheck + 65 - argv[1][alphabetArrayPos] + keyCaseCheck - 65;
            //printf("%d ", adderArray[i]);
            printf("%c", plaintext[i] - adderArray[i]);
        }
        else
        {
            printf("%c", plaintext[i]);
        }
    }

    printf("\n");


}