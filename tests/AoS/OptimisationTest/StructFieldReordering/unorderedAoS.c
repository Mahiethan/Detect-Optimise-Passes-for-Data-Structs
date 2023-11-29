#include <stdio.h>
#include <stdlib.h>

// #pragma pack(1)


struct nodeOneOld
{

    // 24 bytes 
    // 24 / 8 = three CPU cycles needed to be accessed
    int a;
    // 4 byte padding
    double b;
    char c;
    char d;
    // 6 byte padding
};

// void populateNodeOneOld(struct nodeOneOld array[], int size)
void populateNodeOneOld(struct nodeOneOld* array, int size)
{
    for(int i = 0; i < size; i++)
    {
        array[i].a = 1.0;
        array[i].b = 10;
        array[i].c = 'c';
        array[i].d = 'd';
        //array[i].b = array[i].a + array[i].b;
    }
}

void multNodeOneOld(struct nodeOneOld* array, int size)
{
    for(int i = 0; i < size; i++)
    {
        array[i].a = (array[i].b * array[i].a) / (100 / 5);
    }
}

void multArrays(struct nodeOneOld* arrayOne, struct nodeOneOld* arrayTwo, int size)
{
     for(int i = 0; i < size; i++)
    {
        arrayOne[i].a = arrayOne[i].a * arrayTwo[i].a;
        arrayOne[i].b = arrayOne[i].b * arrayTwo[i].b;
    }
}

int main()
{
    // int n = 99999;
    int n = 999999;
   
   for(int i = 0; i < 1000; i++)
    {
    ////static
    // struct nodeOneOld arrayOneOld[n];
    ////dynamic
     struct nodeOneOld* arrayOneOld = (struct nodeOneOld*) malloc(n*sizeof(struct nodeOneOld));
    struct nodeOneOld* arrayTwoOld = (struct nodeOneOld*) malloc(n*sizeof(struct nodeOneOld));

    //  struct nodeOneOld* arrayOneOld = aligned_alloc(64,sizeof(struct nodeOneOld) * n); //no difference
    // struct nodeOneOld* arrayTwoOld = aligned_alloc(64,sizeof(struct nodeOneOld) * n); //no difference

     populateNodeOneOld(arrayOneOld,n);
     populateNodeOneOld(arrayTwoOld,n);

     multNodeOneOld(arrayOneOld,n);
     multArrays(arrayOneOld,arrayTwoOld,n);

    
     free(arrayOneOld); 
    free(arrayTwoOld);
    }


    return 0;
}