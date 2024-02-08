#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

// #pragma pack(1)


struct nodeOneOld
{
    //// Hot fields - most commonly used fields
    int a;
    double b;
    int c;
    //// Cold fields - least used fields
    double d;
    char e;
    double f;
    double g;
    double h;
    double i;
    float j;
    //pointer to same type
    struct nodeOneOld* next;
};

struct nodeTwoOld
{
    //// Hot fields - most commonly used fields
    int a;
    double b;
    int c;
    //// Cold fields - least used fields
    double d;
    char e;
    double f;
    double g;
    double h;
    double i;
    float j;
    //pointer to same type
    struct nodeOneOld* next;
};

// void populateNodeOneOld(struct nodeOneOld array[], int size)
void populateNodeOneOld(struct nodeOneOld* array, int size)
{
    for(int i = 0; i < size; i++)
    {
        array[i].a = 1;
        array[i].b = 10.0;
        array[i].c = 9;
        array[i].d = 23.0;
        array[i].e = 'a';
        array[i].f = 23.0;
        array[i].g = 23.0;
        //array[i].b = array[i].a + array[i].b;
    }
}

void populateNodeTwoOld(struct nodeTwoOld* array, int size)
{
    for(int i = 0; i < size; i++)
    {
        array[i].a = 1;
        array[i].b = 10.0;
        array[i].c = 9;
        array[i].d = 23.0;
        array[i].e = 'a';
        array[i].f = 23.0;
        array[i].g = 23.0;
        //array[i].b = array[i].a + array[i].b;
    }
}

void multNodeOneOld(struct nodeOneOld* array, int size)
{
    for(int j = 0; j < 40; j++)
    {
        for(int i = 0; i < size; i++)
        {
            array[i].a = (array[i].b * array[i].a) + (100 / 5);
            array[i].b = (array[i].a / array[i].c) - (99 * 3);
        }
    }
}

void multArrays(struct nodeOneOld* arrayOne, struct nodeTwoOld* arrayTwo, int size)
{
   for(int j = 0; j < 30; j++)
    {
        for(int i = 0; i < size; i++)
        {
            arrayOne[i].a = arrayOne[i].a * arrayTwo[i].a;
            arrayOne[i].b = arrayOne[i].b * arrayTwo[i].b;
        }
    }
}

// void freeAoS(struct nodeOneOld* aos)
// {
//      int arrayOneOldSize = malloc_usable_size(aos)/sizeof(struct nodeOneOld);
//      int currElem = 0;
//      while(currElem != arrayOneOldSize)
//      {
//         if(!aos[currElem].next)
//             free(aos[currElem].next);
//         currElem++;
//      }
//     // for(currElem = 0; currElem < arrayOneOldSize; currElem++)
//     // {
//     //     if(!arrayOneOld[currElem].next)
//     //         free(arrayOneOld[currElem].next);
//     // }
// }

int main()
{
   // int n = 99999;
//    int n = 100; //works
   int n = 999999; //now works
   int i;
   malloc_usable_size(NULL); //required to have its declaration in the IR
   free(NULL); //required to have its declaration in the IR
   for(i = 0; i < 100; i++)
    {
    ////static
    // struct nodeOneOld arrayOneOld[n];
    ////dynamic
    struct nodeOneOld* arrayOneOld = (struct nodeOneOld*) malloc(n*sizeof(struct nodeOneOld));
    struct nodeTwoOld* arrayTwoOld = (struct nodeTwoOld*) malloc(n*sizeof(struct nodeTwoOld));

    //  struct nodeOneOld* arrayOneOld = aligned_alloc(64,sizeof(struct nodeOneOld) * n); //no difference
    // struct nodeOneOld* arrayTwoOld = aligned_alloc(64,sizeof(struct nodeOneOld) * n); //no difference

     populateNodeOneOld(arrayOneOld,n);
     populateNodeTwoOld(arrayTwoOld,n);

     multNodeOneOld(arrayOneOld,n);
     multArrays(arrayOneOld,arrayTwoOld,n);

     //validity check
    if(i == 77)
     {
        printf("Validity check\n");
        // for(int j = 5000; j < 5010; j++)
        // {
            printf("%d\n---\n",arrayOneOld[5].a);
            printf("%f\n---\n",arrayOneOld[5].b);
            printf("%d\n---\n",arrayTwoOld[5].a);
            printf("%f\n---\n",arrayTwoOld[5].b);

            // arrayOneOld[5000].d = 90;

            printf("%f\n---\n",arrayOneOld[5].d);
            printf("%c\n---\n",arrayOneOld[5].e);
            printf("%f\n---\n",arrayTwoOld[5].d);
            printf("%c\n---\n",arrayTwoOld[5].e);

            // printf("%zu\n",malloc_usable_size(arrayOneOld)/sizeof(struct nodeOneOld)); //to get size of AoS

            // printf("%d\n---\n",arrayOneOld[5000].a);
            // printf("%f\n---\n",arrayOneOld[5000].b);

            // // arrayOneOld[5000].d = 90;

            // printf("%f\n---\n",arrayOneOld[5000].d);
            // printf("%c\n---\n",arrayOneOld[5000].e);
        //}
     }
     
    //  int arrayOneOldSize = malloc_usable_size(arrayOneOld)/sizeof(struct nodeOneOld);
    //  int currElem = 0;
    //  while(currElem != arrayOneOldSize)
    //  {
    //     if(!arrayOneOld[currElem].next)
    //         free(arrayOneOld[currElem].next);
    //     currElem++;
    //  }
    // // for(currElem = 0; currElem < arrayOneOldSize; currElem++)
    // // {
    // //     if(!arrayOneOld[currElem].next)
    // //         free(arrayOneOld[currElem].next);
    // // }
    //  freeAoS(arrayOneOld);
     free(arrayOneOld); 

     free(arrayTwoOld);
    }

    return 0;
}