#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

//Applying structure splitting

struct nodeOneCold
{
    //// Cold fields - least used fields
    int c;
    double d;
    char e;
    double f;
    double g;
    double h;
    double i;
    float j;
};

struct nodeOneOld
{
    //// Hot fields - most commonly used fields
    int a;
    double b;
    //addition of a new pointer to 'cold' structure - increase in struct size
    struct nodeOneCold* cold;
    //pointer to same type
    struct nodeOneOld* next;
};

//adding new argument for nodeOneCold struct
void populateNodeOneOld(struct nodeOneOld* array, int size)
{
    for(int i = 0; i < size; i++)
    {
        array[i].a = 1;
        array[i].b = 10.0;
        array[i].cold = (struct nodeOneCold*) malloc(sizeof(struct nodeOneCold));
        array[i].cold->c = 9;
        array[i].cold->d = 23.0;
        array[i].cold->e = 'a';
        array[i].cold->f = 23.0;
        array[i].cold->g = 23.0;
    }
}

void multNodeOneOld(struct nodeOneOld* array, int size)
{
    for(int j = 0; j < 40; j++) //the greater the limit, the more effective the optimisation is since the affinity of a and b is very high (see notes)
    {
        for(int i = 0; i < size; i++)
        {
            array[i].a = (array[i].b * array[i].a) + (100 / 5);
            array[i].b = (array[i].a / array[i].cold->c) - (99 * 3);
        }
    }
}

void multArrays(struct nodeOneOld* arrayOne, struct nodeOneOld* arrayTwo, int size)
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

void freeAoS(struct nodeOneOld* aos)
{
     int arrayOneOldSize = malloc_usable_size(aos)/sizeof(struct nodeOneOld);
     int currElem = 0;
     while(currElem != arrayOneOldSize)
     {
        free(aos[currElem].cold);
        currElem++;
     }

    free(aos);
}

int main()
{
   int n = 999999;

    ////dynamic
    struct nodeOneOld* arrayOneHot = (struct nodeOneOld*) malloc(n*sizeof(struct nodeOneOld));

    struct nodeOneOld* arrayTwoHot = (struct nodeOneOld*) malloc(n*sizeof(struct nodeOneOld));

     populateNodeOneOld(arrayOneHot,n);
     populateNodeOneOld(arrayTwoHot,n);

     multNodeOneOld(arrayOneHot,n);
     multArrays(arrayOneHot,arrayTwoHot,n);

      //validity check

        printf("Validity check\n");

        printf("%d\n---\n",arrayOneHot[5].a);
        printf("%f\n---\n",arrayOneHot[5].b);

        printf("%f\n---\n",arrayOneHot[5].cold->d);
        printf("%c\n---\n",arrayOneHot[5].cold->e);

    freeAoS(arrayOneHot); //required for large AoS, otherwise program will crash

    freeAoS(arrayTwoHot); //required for large AoS, otherwise program will crash

    return 0;
}