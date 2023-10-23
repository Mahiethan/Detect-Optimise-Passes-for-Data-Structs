#include <stdio.h>
#include <stdlib.h>
int AoS_Start;
int AoS_End;


struct nodeOne
{
    int a;
    int b;
    char c;
};

struct nodeTwo
{
    int a;
    char b;
    char c;
};

struct nodeThree
{
    int a;
    float b;
    double c;
};

int main()
{
    // int n = 500000000;
    int n = 697000; //matching size of staticAoS
    (void) AoS_Start;
    struct nodeOne* arrayOne = (struct nodeOne*) malloc(n*sizeof(struct nodeOne)); //dynamic AoS (stored on heap) (of size n)
    (void) AoS_End;
    // populateNodeOne(arrayOne,n);

    // printArray(arrayOne,n);

    n = 7080; //matching size of staticAoS
    (void) AoS_Start;
    struct nodeTwo* arrayTwo = (struct nodeTwo*) malloc(n*sizeof(struct nodeTwo)); //dynamic AoS (stored on heap) (of size n)
    (void) AoS_End;

    n = 697000; //max size before stack overflow occurs - stack has limited size
    (void) AoS_Start;
    struct nodeOne arrayThree[n]; //static Array of Structs (stored on stack) (of size n)
    (void) AoS_End;

    n = 372; //matching size of staticAoS
    (void) AoS_Start;
    struct nodeThree* arrayFour= (struct nodeThree*) malloc(n*sizeof(struct nodeThree)); //dynamic AoS (stored on heap) (of size n)
    (void) AoS_End;

    n = 124;
    (void) AoS_Start;
    struct nodeThree arrayFive[n]; //static Array of Structs (stored on stack) (of size n)
    (void) AoS_End;

    n = 12432;
    (void) AoS_Start;
    struct nodeTwo arraySix[n]; //static Array of Structs (stored on stack) (of size n)
    (void) AoS_End;

    free(arrayOne);

    free(arrayTwo);

    free(arrayFour);

    return 0;
}