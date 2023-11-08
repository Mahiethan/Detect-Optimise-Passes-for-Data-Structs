#include <stdio.h>
#include <stdlib.h>
int AoS_Start;
int AoS_End;

struct nodeOne
{
    int a;
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

struct nodeOne* globalOne;
struct nodeTwo* globalTwo;

void populateNodeOne(struct nodeOne* array,int size)
{
    // struct nodeOne one;

    for(int i = 0; i < size; i++)
    {
        array[i].a = 1;
        // array[i].b = 2;
        // array[i].c = 'a';
    }
}

void populateNodeTwo(struct nodeTwo* array, int size)
{
    // struct nodeTwo two;

    for(int i = 0; i < size; i++)
    {
        array[i].a = 1;
        array[i].b = 'a';
        // array[i].c = 'a';
    }
}

struct nodeTwo setNodeTwo()
{
    struct nodeTwo n = {1,'a'};
    return n;
}

void populateNodeThree(int size, struct nodeThree* array)
{
    // struct nodeThree three;
    // struct nodeTwo two;
    // struct nodeThree three;
    for(int i = 0; i < size; i++)
    {
        array[i].a = 1;
        array[i].b = 2.0;
        array[i].c = 4.5;
    }
}

void populateGlobalOne(int n)
{
    for(int i = 0; i < n; i++)
    {
        globalOne[i].a = 1;
        // array[i].b = 2;
        // array[i].c = 'a';
    }
}

void populateGlobalTwo()
{
    for(int i = 0; i < 1293; i++)
    {
        globalTwo[i].a = 1;
        globalTwo[i].b = 'b';
        globalTwo[i].c = 'a';
    }
}

void printArray(struct nodeOne* array,int size)
{
    for(int i = 0; i < size; i++)
    {
        printf("Index %d:\n",i);
        printf("a = %d ",array[i].a);
        // printf("b = %d ",array[i].b);
        // printf("c = %c \n",array[i].c);
    }
}

int main()
{
    // int n = 500000000;
    int n = 697000; //matching size of staticAoS
    // struct nodeOne element;
    //int a = sizeof(element);
    


    (void) AoS_Start;
    struct nodeOne* arrayOne = (struct nodeOne*) malloc(n*sizeof(struct nodeOne)); //dynamic AoS (stored on heap) (of size n)
    (void) AoS_End;

    // double* testPtr;
    // testPtr = (double*) malloc(n*sizeof(double));

    for(int i = 0; i < n; i++)
    {
        arrayOne[i].a = 1;
        // array[i].b = 2;
        // array[i].c = 'a';
    }

    int* testPtr2;
    testPtr2 = (int*) malloc(n*sizeof(int));

    // struct nodeOne staticOne[0];

    globalOne = (struct nodeOne*) malloc(n*sizeof(struct nodeOne));

    globalTwo = (struct nodeTwo*) malloc(n*sizeof(struct nodeTwo));

    populateGlobalOne(n);
    populateGlobalTwo();

    //populateNodeOne(arrayOne,n);

    // printArray(arrayOne,n);

    n = 7080; //matching size of staticAoS
    (void) AoS_Start;
    struct nodeTwo* arrayTwo = (struct nodeTwo*) malloc(n*sizeof(struct nodeTwo)); //dynamic AoS (stored on heap) (of size n)
    (void) AoS_End;

    populateNodeTwo(arrayTwo,n);
    // arrayTwo[0] = setNodeTwo();

    n = 372; //matching size of staticAoS
    (void) AoS_Start;
    struct nodeThree* arrayThree = (struct nodeThree*) malloc(n*sizeof(struct nodeThree)); //dynamic AoS (stored on heap) (of size n)
    (void) AoS_End;

    populateNodeThree(n,arrayThree);

    // struct nodeTwo* arrayFour = (struct nodeTwo*) malloc(n*sizeof(struct nodeTwo)); //dynamic AoS (stored on heap) (of size n)

    // populateNodeTwo(arrayFour,n);

    free(arrayOne);

    free(arrayTwo);

    free(arrayThree);

        // //NOT ALLOWED - SEG FAULT OCCURS
    // // n = 372; //matching size of staticAoS
    // // (void) AoS_Start;
    // // struct nodeThree* arrayThree;
    // // (void) AoS_End;

    // // populateNodeThree(arrayThree,n);

    // // arrayThree = (struct nodeThree*) malloc(n*sizeof(struct nodeThree)); //dynamic AoS (stored on heap) (of size n)

    return 0;
}