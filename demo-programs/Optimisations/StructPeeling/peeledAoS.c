#include <stdio.h>
#include <stdlib.h>

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
        arrayOneCold[i].d = 23.0;
        arrayOneCold[i].e = 'a';
        arrayOneCold[i].f = 23.0;
        arrayOneCold[i].g = 23.0;  

        arrayTwoHot[i].a = 1;
        arrayTwoHot[i].b = 10.0;
        arrayTwoHot[i].c = 9;
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

int main()
{
   int n = 999999;
   int i;
   for(i = 0; i < 100; i++)
    {

     populateNodeOneOld(n);
     populateNodeOneOld(n);

     multNodeOneOld(n);
     multArrays(n);

     //validity check
    if(i == 77)
     {
        printf("Validity check\n");

        printf("%d\n---\n",arrayOneHot[5000].a);
        printf("%f\n---\n",arrayOneHot[5000].b);        
     }

    }


    return 0;
}