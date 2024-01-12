#include <stdio.h>
#include <stdlib.h>

// #pragma pack(1)

//Applying structure splitting

struct nodeOneCold
{
    //// Cold fields - least used fields
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
    int c;
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
        array[i].c = 9;
        array[i].cold = (struct nodeOneCold*) malloc(sizeof(struct nodeOneCold));
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
            array[i].b = (array[i].a / array[i].c) - (99 * 3);
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

void freeAoS(struct nodeOneOld* array, int size)
{
    for(int i = 0; i < size; i++)
        free(array[i].cold);
    free(array);
}

int main()
{
   // int n = 99999;
   int n = 999999;
   int i;
   for(i = 0; i < 100; i++)
    {
    ////static
    // struct nodeOneOld arrayOneOld[n];
    ////dynamic
    struct nodeOneOld* arrayOneHot = (struct nodeOneOld*) malloc(n*sizeof(struct nodeOneOld));
    //create one for nodeOneCold struct too

    struct nodeOneOld* arrayTwoHot = (struct nodeOneOld*) malloc(n*sizeof(struct nodeOneOld));

    //  struct nodeOneOld* arrayOneOld = aligned_alloc(64,sizeof(struct nodeOneOld) * n); //no difference
    // struct nodeOneOld* arrayTwoOld = aligned_alloc(64,sizeof(struct nodeOneOld) * n); //no difference

     populateNodeOneOld(arrayOneHot,n);
     populateNodeOneOld(arrayTwoHot,n);

     multNodeOneOld(arrayOneHot,n);
     multArrays(arrayOneHot,arrayTwoHot,n);

      //validity check
     if(i == 77)
     {
        printf("Validity check\n");
        // for(int j = 5000; j < 5010; j++)
        // {
            printf("%d\n",arrayOneHot[5000].a);
            printf("%f\n---\n",arrayOneHot[5000].b);
        //}
     }


    freeAoS(arrayOneHot,n); 
    // free(arrayOneHot);
    // free(arrayOneCold); 
    freeAoS(arrayTwoHot,n); 
    // free(arrayTwoHot);
    // free(arrayTwoCold); 
    }


    return 0;
}