/*
 This test ONLY considers hotness when splitting - it doesn't consider affinity group checks.
*/

#include <stdio.h>
#include <stdlib.h>

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

void populateNodeOne(struct nodeOneOld* arrayOne, struct nodeOneOld* arrayTwo, int size)
{
    for(int i = 0; i < size; i++)
    {
        arrayOne[i].a = 1;
        arrayOne[i].b = 10.0;
        arrayOne[i].c = 9;
        arrayOne[i].d = 23.0;
        arrayOne[i].e = 'a';
        for(int j = 0; j < 67; j++)
            arrayOne[i].f[j] = 23.0;
        arrayOne[i].g = 23.0;

        arrayTwo[i].a = 1;
        arrayTwo[i].b = 10.0;
        arrayTwo[i].c = 9;
    }
}


void multArrayOne(struct nodeOneOld* arrayOne, int size)
{
    for(int j = 0; j < 100000; j++)
    {
        for(int i = 0; i < size; i++)
        {
            arrayOne[i].a = (arrayOne[i].b * arrayOne[i].a) + (100 / 5);
            arrayOne[i].b = (arrayOne[i].a / arrayOne[i].c) - (99 * 3);
        }
    }
}

void multArrays(struct nodeOneOld* arrayOne, struct nodeOneOld* arrayTwo, int size)
{
    for(int j = 0; j < 100000; j++)
    {
        for(int i = 0; i < size; i++)
        {
            arrayOne[i].a = arrayOne[i].a * arrayTwo[i].a;
            arrayOne[i].b = (arrayOne[i].b + arrayTwo[i].a) / 1.1;
        }
    }
}


int main(int argc, char *argv[])
{
   int n = atoi(argv[1]); // command line argument represents size of AoS
   
    // dynamic AoS
    struct nodeOneOld* arrayOne = (struct nodeOneOld*) malloc(n*sizeof(struct nodeOneOld));
    struct nodeOneOld* arrayTwo = (struct nodeOneOld*) malloc(n*sizeof(struct nodeOneOld));

    populateNodeOne(arrayOne, arrayTwo, n);

    int i = (rand() % n);
    int j = (rand() % 67);

    arrayOne[i].a = 1;
    arrayOne[i].b = 10.0;
    arrayOne[i].c = 9;

    arrayTwo[i].a = 1;
    arrayTwo[i].b = 10.0;
    arrayTwo[i].c = 9;

    multArrayOne(arrayOne,n);
    multArrays(arrayOne,arrayTwo,n);

    // print outputs for checking validity
    printf("Validity check\n");

    printf("\na: ---\n%d",arrayOne[i].a);
    printf("\nb: ---\n%f",arrayOne[i].b);
    printf("\nc: ---\n%d",arrayOne[i].c);
    printf("\nd: ---\n%lld",arrayOne[i].d);
    printf("\ne: ---\n%d",arrayOne[i].e);
    printf("\ng: ---\n%f",arrayOne[i].g);
    printf("\nh: ---\n%f",arrayOne[i].h);
    printf("\ni: ---\n%f",arrayOne[i].i);

    free(arrayOne);
    free(arrayTwo);

    return 0;
}