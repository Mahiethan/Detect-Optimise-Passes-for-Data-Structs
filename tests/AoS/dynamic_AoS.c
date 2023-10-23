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

void populateNodeOne(struct nodeOne* array,int size)
{
    for(int i = 0; i < size; i++)
    {
        array[i].a = 1;
        array[i].b = 2;
        array[i].c = 'a';
    }
}

void printArray(struct nodeOne* array,int size)
{
    for(int i = 0; i < size; i++)
    {
        printf("Index %d:\n",i);
        printf("a = %d ",array[i].a);
        printf("b = %d ",array[i].b);
        printf("c = %c \n",array[i].c);
    }
}

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

    n = 372; //matching size of staticAoS
    (void) AoS_Start;
    struct nodeThree* arrayThree = (struct nodeThree*) malloc(n*sizeof(struct nodeThree)); //dynamic AoS (stored on heap) (of size n)
    (void) AoS_End;


    free(arrayOne);

    free(arrayTwo);

    free(arrayThree);

    return 0;
}