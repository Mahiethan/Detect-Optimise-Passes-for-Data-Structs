/*
    This program uses the re-ordered struct from the unordered.c file

    Re-ordering was done manually by hand
*/

#include <stdio.h>
#include <stdlib.h>

#define SIZE 500000

struct nodeOneOld
{
    double f[2];
    double b;
    double g;
    double h;
    double i;
    long long int d : 48;
    char e;
    int a;
    int c;
    float j;
};

struct nodeOneOld arrayOne[SIZE]; // global AoS

void populateNode(struct nodeOneOld* array, int size)
{
        for(int i = 0; i < size; i++)
        {
            array[i].a = 1;
            array[i].b = 10.0;
            array[i].c = 9;
            array[i].d = 23.0;
            array[i].e = 'a';
            for(int j = 0; j < 2; j++)
                array[i].f[j] = 23.0;
            array[i].g = 23.0;
            array[i].h = 456.3234;
        }

        for(int i = 0; i < size; i++)
        {
            arrayOne[i].a = 1;
            arrayOne[i].b = 10.0;
            arrayOne[i].c = 9;
            arrayOne[i].d = 23.0;
            arrayOne[i].e = 'a';
            for(int j = 0; j < 2; j++)
                array[i].f[j] = 23.0;
            arrayOne[i].g = 23.0;
            arrayOne[i].h = 456.3234;
        }
}

void calculationsOne(int size)
{
    for(int j = 0; j < 100000; j++)
    {
        for(int i = 0; i < size; i++)
        {
            arrayOne[i].a += 1;
            arrayOne[i].b += 0.001;
            arrayOne[i].c += 1;
            arrayOne[i].d += 1;
            for(int j = 0; j < 2; j++)
                arrayOne[i].f[j] += 0.001;
            arrayOne[i].g += 0.001;
            arrayOne[i].h += 0.001;
        }
    }
}

void calculationsTwo(int size, struct nodeOneOld* array)
{
    for(int j = 0; j < 100000; j++)
    {
        for(int i = 0; i < size; i++)
        {
            array[i].a += arrayOne[i].a;
            array[i].b += arrayOne[i].b;
            array[i].c += arrayOne[i].c;
            array[i].d += arrayOne[i].d;
            for(int j = 0; j < 2; j++)
                array[i].f[j] += arrayOne[i].f[j];
            array[i].g += arrayOne[i].g;
            array[i].h += arrayOne[i].h;
        }
    }
}

int main(int argc, char *argv[])
{
    int n = atoi(argv[1]); // command line argument represents size of AoS
   
    struct nodeOneOld* arrayTwoOld = (struct nodeOneOld*) malloc(n * sizeof(struct nodeOneOld)); // dynamic AoS
    
    populateNode(arrayTwoOld,n);

    calculationsOne(n);
    calculationsTwo(n,arrayTwoOld);

    int i = (rand() % n);
    int j = (rand() % 67);

    // print outputs for checking validity

    printf("Validity check\n");

    printf("\na: ---\n%d",arrayTwoOld[i].a);
    printf("\nb: ---\n%f",arrayTwoOld[i].b);
    printf("\nc: ---\n%d",arrayTwoOld[i].c);
    printf("\nd: ---\n%lld",arrayTwoOld[i].d);
    printf("\ne: ---\n%d",arrayTwoOld[i].e);

    printf("\ng: ---\n%f",arrayTwoOld[i].g);
    printf("\nh: ---\n%f",arrayTwoOld[i].h);
    printf("\ni: ---\n%f",arrayTwoOld[i].i);


    return 0;
}