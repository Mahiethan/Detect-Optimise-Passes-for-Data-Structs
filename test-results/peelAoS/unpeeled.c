#include <stdio.h>
#include <stdlib.h>

#define SIZE 1000 // this represents the size of the AoS

struct nodeOneOld
{

    //// Hot fields - most commonly used fields
    int a;
    double b;
    int c;
    //// Cold fields - least used fields
    long long int d : 48;
    char e;
    double f[67];
    double g;
    double h;
    double i;
    float j;
};

struct nodeOneOld arrayOneOld[SIZE]; // global AoS
struct nodeOneOld arrayTwoOld[SIZE]; // global AoS

void populateNodeOneOld(int size)
{
    for(int i = 0; i < size; i++)
    {
        arrayOneOld[i].a = 1;
        arrayOneOld[i].b = 10.0;
        arrayOneOld[i].c = 9;
        arrayOneOld[i].d = 23.0;
        arrayOneOld[i].e = 'a';
        for(int j = 0; j < 67; j++)
            arrayOneOld[i].f[j] = 23.0;
        arrayOneOld[i].g = 23.0;

        arrayTwoOld[i].a = 1;
        arrayTwoOld[i].b = 10.0;
        arrayTwoOld[i].c = 9;
    }
}


void multNodeOneOld(int size)
{
    for(int j = 0; j < 100000; j++)
    {
        for(int i = 0; i < size; i++)
        {
            arrayOneOld[i].a = (arrayOneOld[i].b * arrayOneOld[i].a) + (100 / 5);
            arrayOneOld[i].b = (arrayOneOld[i].a / arrayOneOld[i].c) - (99 * 3);
        }
    }
}

void multArrays(int size)
{
    for(int j = 0; j < 100000; j++)
    {
        for(int i = 0; i < size; i++)
        {
            arrayOneOld[i].a = arrayOneOld[i].a * arrayTwoOld[i].a;
            arrayOneOld[i].b = (arrayOneOld[i].b + arrayTwoOld[i].a) / 1.1;
        }
    }
}


int main(int argc, char *argv[])
{
    populateNodeOneOld(SIZE);

    int i = (rand() % SIZE);
    int j = (rand() % 67);

    arrayOneOld[i].a = 1;
    arrayOneOld[i].b = 10.0;
    arrayOneOld[i].c = 9;

    arrayTwoOld[i].a = 1;
    arrayTwoOld[i].b = 10.0;
    arrayTwoOld[i].c = 9;

    multNodeOneOld(SIZE);
    multArrays(SIZE);

    // print outputs for checking validity
    printf("Validity check\n");

    printf("\na: ---\n%d",arrayOneOld[i].a);
    printf("\nb: ---\n%f",arrayOneOld[i].b);
    printf("\nc: ---\n%d",arrayOneOld[i].c);
    printf("\nd: ---\n%lld",arrayOneOld[i].d);
    printf("\ne: ---\n%d",arrayOneOld[i].e);
    printf("\ng: ---\n%f",arrayOneOld[i].g);
    printf("\nh: ---\n%f",arrayOneOld[i].h);
    printf("\ni: ---\n%f",arrayOneOld[i].i);

    return 0;
}