#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<cs50.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    // TODO #1
    if(argc!=3)
    {
       printf("./reverse input.wav output.wav\n");
       return 1;
    }
    if(argc==3)
    {
        for(int i=1;i<=2;i++)
        {
        int j=strlen(argv[i])-1;
        if(argv[i][j]!='v'||argv[i][j-1]!='a'||argv[i][j-2]!='w'||argv[i][j-3]!='.')
        {
           printf("./reverse input.wav output.wav\n");
           return 1;
        }
        }
    }
    // Open input file for reading
    // TODO #2
    FILE *fptr = fopen(argv[1],"rb");//Remember to close the file also
    if(fptr==NULL)
    {
        printf("Failed to open file\n");
        return 1;
    }
    // Read header
    // TODO #3
    WAVHEADER header;
    fread(&header,sizeof(header),1,fptr);
    // Use check_format to ensure WAV format
    // TODO #4
    if(!check_format(header))
    {
       printf("Not a .wav file\n");
    }
    // Open output file for writing
    // TODO #5
    FILE *fileout = fopen(argv[2],"wb");
    if(fileout==NULL)
    {
        printf("Failed to open file\n");
        return 1;
    }
    // Write header to file
    // TODO #6
    fwrite(&header,sizeof(header),1,fileout);
    // Use get_block_size to calculate size of block
    // TODO #7
    int blocksize = get_block_size(header);
    // Write reversed audio to file
    // TODO #8
    int rawaudiosize = (int)header.subchunk2Size;
    int numchannels = rawaudiosize/blocksize;
    fseek(fptr,rawaudiosize,SEEK_CUR);
    DWORD copy[numchannels];
    for(int i=0;i<numchannels;i++)
    {
        if(i==0||i==numchannels-1)
        {
            fseek(fptr,-1*blocksize,SEEK_CUR);
        }
        else
        {
            fseek(fptr,-2*blocksize,SEEK_CUR);
        }
        fread(&copy[i],blocksize,1,fptr);
        fwrite(&copy[i],blocksize,1,fileout);
    }
    // Closing all the other files
    fclose(fptr);
    fclose(fileout);
    return 0;
}

int check_format(WAVHEADER header)
{
    // TODO #4
    if(header.format[0]!='W'||header.format[1]!='A'||header.format[2]!='V'||header.format[3]!='E')
    {
        return false;
    }
    else
    {
        return true;
    }
    return 0;
}

int get_block_size(WAVHEADER header)
{
    int blocksize = ((int)header.numChannels*(int)header.bitsPerSample)/8;
    // TODO #7
    return blocksize;
}