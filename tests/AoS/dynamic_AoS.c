#include <stdio.h>
#include <stdlib.h>

struct nodeOne
{
    int a;
    int b;
    char c;
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
    struct nodeOne* arrayOne = (struct nodeOne*) malloc(n*sizeof(struct nodeOne)); //dynamic AoS (stored on heap) (of size n)

    // populateNodeOne(arrayOne,n);

    // printArray(arrayOne,n);
    return 0;
}