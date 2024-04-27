#include <stdio.h>
#include <stdlib.h>

// #pragma pack(1)

struct nodeOneOld
{
    /////////////////////// Word 0
    double a;
    /////////////////////// Word 1
    long long int b : 48; // 64-bit integer with bit-cast to 6 bytes (48 bits)
    char e;
    // 1 byte padding
    /////////////////////// Word 2
    int c;
    int d;
};

// Total size: 24 bytes


void populateNodeOneOld(struct nodeOneOld* array, int size)
{
    for(int i = 0; i < size; i++)
    {
        array[i].a = 17.0;
        array[i].b = 1000;
        array[i].c = 77;
        array[i].d = 10;
        array[i].e = 'd';
        //array[i].b = array[i].a + array[i].b;
    }
}

void multNodeOneOld(struct nodeOneOld* array, int size)
{
    for(int i = 0; i < size; i++)
    {
        array[i].a = (array[i].b * array[i].a) / (100 / 5);
        array[i].b = (array[i].a - array[i].b) * (100 / 5);
    }
}

void multArrays(struct nodeOneOld* arrayOne, struct nodeOneOld* arrayTwo, int size)
{
    for(int j = 0; j < 1000; j++)
    {
        for(int i = 0; i < size; i++)
        {
            arrayOne[i].b = arrayOne[i].b * arrayTwo[i].b;
            arrayOne[i].e = 'f';
            arrayTwo[i].e = 'f';

        }
    }
}

int main()
{
    // int n = 99999;
    int n = 9999999;
    int i;
   
//    for(i = 0; i < 100; i++)
//     {
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

    // if(i == 79)
    // {
        printf("Validity check\n");
        // for(int j = 5000; j < 5010; j++)
        // {
            printf("a: %f\n---\n",arrayOneOld[10].a);
            printf("b: %lld\n---\n",arrayOneOld[10].b);
            printf("c: %d\n---\n",arrayOneOld[10].c);
            printf("d: %d\n---\n",arrayOneOld[10].d);
            printf("e: %c\n---\n",arrayOneOld[10].e);
        //}
    // }


    
    free(arrayOneOld); 
    free(arrayTwoOld);
    //}


    return 0;
}