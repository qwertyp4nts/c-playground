#include <stdio.h>
#include <cs50.h>
#include <math.h>

int checkCardType(long long cardNum)
{
    int digit = floor(log10(cardNum)) + 1;
    int arr[digit];
    int i;
    for (i = digit; i > 0; i--)
    {
        long banana = (long)(cardNum / pow(10, i - 1));
        arr[digit - i] = banana % 10;
    }
    if (arr[0] == 3 && digit == 15)
    {
        if (arr[1] == 4 || arr[1] == 7)
        {
            return 1;
        }
    }

    if (arr[0] == 5 && digit == 16)
    {
        if (arr[1] >= 1 && arr[1] <= 5)
        {
            return 2;
        }
    }

    if (arr[0] == 4 && (digit == 13 || digit == 16))
    {
        return 3;
    }

    return 0;
}

bool checkSum(long long cardNum) //needs to pass checkCardType first
{
    int digit = floor(log10(cardNum)) + 1;
    int arr[digit];

    for (int i = digit; i > 0; i--)
    {
        long banana = (long)(cardNum / pow(10, i - 1));
        arr[digit - i] = banana % 10;
    }

    int step1arraySize = floor(digit / 2);
    int step1[step1arraySize];
    int d = digit - 1;

    for (int k = 0; k < step1arraySize; k++)
    {
        d--;
        step1[k] = arr[d] * 2;
        d--;
    }

    //2nd part

    int step2ArraySize = 0;

    for (int m = 0; m < step1arraySize; m++)
    {
        //Use the step 1 array, and create a new array based on it. Add a new entry when number is greater than 9
        //C doesn't support variable size arrays at this version, so we need to determine the size first
        if (step1[m] < 10)
        {
            step2ArraySize += 1;
        }
        if (step1[m] > 9)
        {
            step2ArraySize += 2;
        }
    }

    int step2[step2ArraySize];
    int n2 = 0;
    for (int n = 0; n < step2ArraySize; n++)
    {
        if (step1[n] < 10)
        {
            step2[n2] = step1[n];
        }
        if (step1[n] > 9)
        {
            double step1nToDouble = step1[n];
            double num = step1nToDouble / 10;
            int intpart = (int)num;
            double decpart = num - intpart;
            double multiplyDouble = decpart * 10 + 0.5; //add 0.5 to account for rounding error
            int secondIntPart = multiplyDouble;

            step2[n2] = intpart;
            step2[n2 + 1] = secondIntPart;
            n2++;
        }
        n2++;
    }

    int step3 = 0; //step 3: Add those products’ digits (i.e., not the products themselves) together:
    for (int o = 0; o < step2ArraySize; o++)
    {
        step3 += step2[o];
    }

    //Add that sum (13) to the sum of the digits that weren’t multiplied by 2 (starting from the end):
    for (int p = digit - 1; p >= 0; p--)
    {
        {
            step3 += arr[p];
            p--;
        }
    }
    //step 4: If the total’s last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid!

    if (step3 % 10 == 0)
    {
        return true;
    }
    return false;
}

int main(void)
{
    long long cardNum;
    int count = 0;
    bool validInput = false;
    int cardType = 0;
    bool checkSumResult = false;

    do
    {
        cardNum = get_long("Number: ");
        count = (cardNum == 0) ? 1  : (log10(cardNum) + 1);

        if (count > 0 && count < 21)
        {
            validInput = true;
        }
    }
    while (validInput == false);

    //determine what kind of card
    if (count == 13 || count == 15 || count == 16)
    {
        cardType = checkCardType(cardNum);
        if (cardType == 1)
        {
            checkSumResult = checkSum(cardNum);
            if (checkSumResult == true)
            {
                printf("AMEX\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        if (cardType == 2)
        {
            checkSumResult = checkSum(cardNum);
            if (checkSumResult == true)
            {
                printf("MASTERCARD\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        if (cardType == 3)
        {
            checkSumResult = checkSum(cardNum);
            if (checkSumResult == true)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        if (cardType == 0)
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }

}

