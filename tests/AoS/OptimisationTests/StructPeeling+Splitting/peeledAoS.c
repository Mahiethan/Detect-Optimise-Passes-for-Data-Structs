#include <stdio.h>
#include <stdlib.h>

// #pragma pack(1)

//Applying structure peeling
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

struct nodeOneHot
{
    //// Hot fields - most commonly used fields
    int a;
    double b;
    int c;
};

struct nodeOneHot arrayOneHot[999999];
//new array to store cold structure
struct nodeOneCold arrayOneCold[999999];

struct nodeOneHot arrayTwoHot[999999];
//new array to store cold structure
struct nodeOneCold arrayTwoCold[999999];

//adding new argument for nodeOneCold struct
void populateNodeOneOld(int size)
{
    for(int i = 0; i < size; i++)
    {
        arrayOneHot[i].a = 1;
        arrayOneHot[i].b = 10.0;
        arrayOneHot[i].c = 9;
        // array[i].cold = (struct nodeOneCold*) malloc(sizeof(struct nodeOneCold));
        arrayOneCold[i].d = 23.0;
        arrayOneCold[i].e = 'a';
        arrayOneCold[i].f = 23.0;
        arrayOneCold[i].g = 23.0;  

        arrayTwoHot[i].a = 1;
        arrayTwoHot[i].b = 10.0;
        arrayTwoHot[i].c = 9;
        // array[i].cold = (struct nodeOneCold*) malloc(sizeof(struct nodeOneCold));
        arrayTwoCold[i].d = 23.0;
        arrayTwoCold[i].e = 'a';
        arrayTwoCold[i].f = 23.0;
        arrayTwoCold[i].g = 23.0;  
    }
}

void multNodeOneOld(int size)
{
    for(int j = 0; j < 30; j++) //the greater the limit, the more effective the optimisation is since the affinity of a and b is very high (see notes)
    {
        for(int i = 0; i < size; i++)
        {
            arrayOneHot[i].a = (arrayOneHot[i].b * arrayOneHot[i].a) + (100 / 5);
            arrayOneHot[i].b = (arrayOneHot[i].a / arrayOneHot[i].c) - (99 * 3);
        }
    }
}

void multArrays(int size)
{
    for(int j = 0; j < 30; j++)
    {
        for(int i = 0; i < size; i++)
        {
            arrayOneHot[i].a = arrayOneHot[i].a * arrayTwoHot[i].a;
            arrayOneHot[i].b = arrayOneHot[i].b * arrayTwoHot[i].b;
        }
    }
}

// void freeAoS(struct nodeOneHot* array, int size)
// {
//     for(int i = 0; i < size; i++)
//         free(array[i].cold);
//     free(array);
// }

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
    // struct nodeOneHot* arrayOneHot = (struct nodeOneHot*) malloc(n*sizeof(struct nodeOneHot));
    // //create one for nodeOneCold struct too
    // struct nodeOneCold* arrayOneCold = (struct nodeOneCold*) malloc(n*sizeof(struct nodeOneCold));

    // struct nodeOneHot* arrayTwoHot = (struct nodeOneHot*) malloc(n*sizeof(struct nodeOneHot));
    // struct nodeOneCold* arrayTwoCold = (struct nodeOneCold*) malloc(n*sizeof(struct nodeOneCold));

    //  struct nodeOneOld* arrayOneOld = aligned_alloc(64,sizeof(struct nodeOneOld) * n); //no difference
    // struct nodeOneOld* arrayTwoOld = aligned_alloc(64,sizeof(struct nodeOneOld) * n); //no difference

     populateNodeOneOld(n);
     populateNodeOneOld(n);

     multNodeOneOld(n);
     multArrays(n);

     //validity check
      if(i == 77)
     {
        printf("Validity check\n");


        // for(int j = 5000; j < 5010; j++)
        // {
            printf("%d\n---\n",arrayOneHot[5000].a);
            printf("%f\n---\n",arrayOneHot[5000].b);
        //}
        
     }

    // freeAoS(arrayOneHot,n); 
    // free(arrayOneHot);
    // free(arrayOneCold); 
    // // freeAoS(arrayTwoHot,n); 
    // free(arrayTwoHot);
    // free(arrayTwoCold); 
    }


    return 0;
}