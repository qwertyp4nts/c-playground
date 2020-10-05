#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

bool isImage();
unsigned char block[512];

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Please provide one argument\n");
        return 1;
    }

    FILE *card = fopen(argv[1], "r");

    if (card == NULL)
    {
        printf("No such file '%s' exists\n", argv[1]);
        return 1;
    }

    fseek(card, 0L, SEEK_END);
    unsigned int sz = ftell(card);
    printf("File size is: %d\n", sz);
    rewind(card);

    int totalBlocks = sz / 512;
    bool gotFirstBlock = false;
    int currentBlock = 0;
    
    FILE *f_image = 0;
    int   f_counter = 0;

    while (currentBlock < totalBlocks)
    {
        // read in a block
        fread(block, 1, 512, card);
    
        // if this is an image block, it needs to be written out
        if (isImage())
        {
            if (f_image != 0)
            {
                fclose(f_image);
                f_image = 0;
            }
            char fn[8];
            sprintf(fn, "%03d.jpg", f_counter ++);  // notice we inc the counter
            f_image = fopen(fn, "w");
        }
        
        if (f_image)
        {
            fwrite(block, 1, 512, f_image);
        }
        
        currentBlock++;
    }
        
    if (f_image != 0)
    {
        fclose(f_image);
        f_image = 0;
    }
    
    rewind(card);
    fclose(card);
    return 0;
}

bool isImage()
{
    if (block[0] == 0xff)
    {
        if (block[1] == 0xd8)
        {
            if (block[2] == 0xff)
            {
                unsigned char high = block[3] >> 4;
                if (high == 0xe)
                {
                    return true;
                }
            }
        }
    }
    return false;
}