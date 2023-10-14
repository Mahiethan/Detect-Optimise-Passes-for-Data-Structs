#include <stdio.h>

struct nodeOne
{
    int a;
    int b;
    char c;
};


void populateNodeOne(struct nodeOne array[],int size)
{
    for(int i = 0; i < size; i++)
    {
        array[i].a = 1;
        array[i].b = 2;
        array[i].c = 'a';
    }
}

void printArray(struct nodeOne array[],int size)
{
    for(int i = 0; i < size; i++)
    {
        printf("Index %d:\n",i);
        printf("a = %d, ",array[i].a);
        printf("b = %d, ",array[i].b);
        printf("c = %c \n",array[i].c);
    }
}

int main()
{
    int n = 697000; //max size before stack overflow occurs - stack has limited size
    struct nodeOne arrayOne[n]; //static Array of Structs (stored on stack) (of size n)

    //populate array
    // populateNodeOne(arrayOne,n);

    // printArray(arrayOne,n);

    return 0;
}