#include <stdio.h>
#include <stdlib.h>

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

void populateNodeOneOld(struct nodeOneOld* array, int size)
{
    for(int i = 0; i < size; i++)
    {
        array[i].a = 17.0;
        array[i].b = 10;
        array[i].c = 'c';
        array[i].d = 'd';
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
    for(int j = 0; j < 70; j++)
    {
        for(int i = 0; i < size; i++)
        {
            arrayOne[i].a = arrayOne[i].a * arrayTwo[i].a;
            arrayOne[i].b = arrayOne[i].b * arrayTwo[i].b;
        }
    }
}

int main()
{
    int n = 999999;
    int i;
   
   for(i = 0; i < 100; i++)
    {
    ////dynamic
    struct nodeOneOld* arrayOneOld = (struct nodeOneOld*) calloc(n,sizeof(struct nodeOneOld));
    struct nodeOneOld* arrayTwoOld = (struct nodeOneOld*) malloc(n*sizeof(struct nodeOneOld));


    populateNodeOneOld(arrayOneOld,n);
    populateNodeOneOld(arrayTwoOld,n);

    multNodeOneOld(arrayOneOld,n);
    multArrays(arrayOneOld,arrayTwoOld,n);


    if(i == 79)
    {
        printf("Validity check\n");

        printf("a: %d\n---\n",arrayOneOld[1].a);
        printf("b: %f\n---\n",arrayOneOld[1].b);
        printf("c: %c\n---\n",arrayOneOld[1].c);
        printf("d: %c\n---\n",arrayOneOld[1].d);
    }

    free(arrayOneOld); 
    free(arrayTwoOld);
    }


    return 0;
}