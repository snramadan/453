#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>

#define PAGES 256 
#define ENTRIES 256
#define TLBS 16 
#define BITS 8 
#define FRAMES 256
#define FENTRIES 256 
#define MEM (FRAMES * FENTRIES) 

int v; 
int pNum; 
int offset; 
int p; 
int fNum; 
int val;
int pTable[ENTRIES]; 
int tlb[TLBS][2]; 
int front = -1; 
int back = -1; 
char memory[MEM]; 
int ind = 0;
int count = 0; 
int addr = 0; 

int getpNum(int v) 
{
    return (v >> BITS);
}

int getOS(int v) 
{ 
    int m = 255;
    return v & m;
}

void initPTable(int n) 
{
    int i;
    for (i = 0; i < ENTRIES; i++) 
    {
        pTable[i] = n;
    }
}

void initTLB(int n) 
{
    int i;
    for (i = 0; i < TLBS; i++) 
    {
        tlb[i][0] = -1;
        tlb[i][1] = -1;
    }
}

int checkTLB(int page) 
{
    int i;
    for (i = 0; i < TLBS; i++) 
    {
        if (tlb[i][0] == page) 
        {
            count++;
            return tlb[i][1];
        }
    }
    return -1;
}

void changeTLB(int page, int frame) 
{
    if (front == -1) {
        front = 0;
        back = 0;
        tlb[back][0] = page;
        tlb[back][1] = frame;
    }
    else 
    {
        front = (front + 1) % TLBS;
        back = (back + 1) % TLBS;
        tlb[back][0] = page;
        tlb[back][1] = frame;
    }
    return;
}

int main(int argc, char *argv[]) 
{
    char* in; 
    char* out; 
    char* file; 
    char* data; 
    int fd; 
    char arr[8];
    FILE* ptrIN; 
    FILE* ptrOUT; 

    initPTable(-1);
    initTLB(-1);

    if (argc != 4) 
    {
        printf("Enter input, output, and store file names!");
        exit(EXIT_FAILURE);
    }

    else 
    {
        in = argv[1];
        out = argv[2];
        file = argv[3];

        if ((ptrIN = fopen(in, "r")) == NULL) 
        {
            printf("Input file could not be opened.\n");
            exit(EXIT_FAILURE);
        }

        if ((ptrOUT = fopen(out, "a")) == NULL) 
        {
            printf("Output file could not be opened.\n");
            exit(EXIT_FAILURE);
        }

        fd = open(file, O_RDONLY);
        data = mmap(0, MEM, PROT_READ, MAP_SHARED, fd, 0);
        if (data == MAP_FAILED) 
        {
            close(fd);
            printf("Error mmapping the backing store file!");
            exit(EXIT_FAILURE);
        }

        while (fgets(arr, sizeof(arr), ptrIN)) 
        {
            v = atoi(arr);
            addr++;
            pNum = getpNum(v);
            offset = getOS(v);
            fNum = checkTLB(pNum);

            if (fNum != -1) 
            {
                p = fNum + offset;
                val = memory[p];
            }
            else 
            {
                fNum = pTable[pNum];
                if (fNum != -1) 
                {
                    p = fNum + offset;
                    changeTLB(pNum, fNum);
                    val = memory[p];
                }
                else 
                {
                    int page_address = pNum * PAGES;

                    if (ind != -1) 
                    {
                        memcpy(memory + ind, data + page_address, PAGES);
                        fNum = ind;
                        p = fNum + offset;
                        val = memory[p];
                        pTable[pNum] = ind;
                        changeTLB(pNum, fNum);

                        if (ind < MEM - FRAMES) 
                        {
                            ind += FRAMES;
                        }
                        else 
                        {
                            ind = -1;
                        }
                    }
                    else 
                    {

                    }
                }
            }
            fprintf(ptrOUT, "Virtual address: %d ", v); 
            fprintf(ptrOUT, "Physical address: %d ", p);
            fprintf(ptrOUT, "Value: %d\n", val);
        }
        fclose(ptrIN);
        fclose(ptrOUT);
        close(fd);
    }
    return EXIT_SUCCESS;
}
