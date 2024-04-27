#include <stdio.h>
// int AoS_Start;
// int AoS_End;


typedef struct nodeOne
{
    int a;
    int b;
    char c;
}Node;

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


// void populateNodeOne(struct nodeOne array[],int size)
// {
//     for(int i = 0; i < size; i++)
//     {
//         array[i].a = 1;
//         array[i].b = 2;
//         array[i].c = 'a';
//     }
// }

// void printArray(struct nodeOne array[],int size)
// {
//     for(int i = 0; i < size; i++)
//     {
//         printf("Index %d:\n",i);
//         printf("a = %d, ",array[i].a);
//         printf("b = %d, ",array[i].b);
//         printf("c = %c \n",array[i].c);
//     }
// }

int main()
{
    int n = 697000; //max size before stack overflow occurs - stack has limited size
    //(void) AoS_Start;
    struct nodeOne arrayOne[n]; //static Array of Structs (stored on stack) (of size n)
    //(void) AoS_End;

       //populate array
    // populateNodeOne(arrayOne,n);

    // printArray(arrayOne,n);

    n = 12432;
    //(void) AoS_Start;
    struct nodeTwo arrayTwo[n]; //static Array of Structs (stored on stack) (of size n)
    //(void) AoS_End;

    n = 124;
    //(void) AoS_Start;
    struct nodeThree a[1]; //AoS of size 1
    Node c[100]; //AoS of size 100
    struct nodeThree arrayThree[n]; //static Array of Structs (stored on stack) (of size n)
    //(void) AoS_End;

    //(void) AoS_Start;
    struct nodeOne test[2344]; //AoS of size 2344
    //(void) AoS_End;

    //Should not detect these as AoS
     struct nodeOne x;
     struct nodeTwo y;
     struct nodeTwo z;

     //Should detect 6 AoS data structures - static

    return 0;
}