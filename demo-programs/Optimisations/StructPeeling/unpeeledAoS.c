#include <stdio.h>
#include <stdlib.h>

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
};

struct nodeOneOld arrayOneOld[999999];
struct nodeOneOld arrayTwoOld[999999];

void populateNodeOneOld(int size)
{
    for(int i = 0; i < size; i++)
    {
        arrayOneOld[i].a = 1;
        arrayOneOld[i].b = 10.0;
        arrayOneOld[i].c = 9;
        arrayOneOld[i].d = 23.0;
        arrayOneOld[i].e = 'a';
        arrayOneOld[i].f = 23.0;
        arrayOneOld[i].g = 23.0;

        arrayTwoOld[i].a = 1;
        arrayTwoOld[i].b = 10.0;
        arrayTwoOld[i].c = 9;
        arrayTwoOld[i].d = 23.0;
        arrayTwoOld[i].e = 'a';
        arrayTwoOld[i].f = 23.0;
        arrayTwoOld[i].g = 23.0;
    }
}

void multNodeOneOld(int size)
{
    for(int j = 0; j < 30; j++)
    {
        for(int i = 0; i < size; i++)
        {
            arrayOneOld[i].a = (arrayOneOld[i].b * arrayOneOld[i].a) + (100 / 5);
            arrayOneOld[i].b = (arrayOneOld[i].a / arrayOneOld[i].c) - (99 * 3);
        }
    }
}

void multArrays(int size)
{
    for(int j = 0; j < 30; j++)
    {
        for(int i = 0; i < size; i++)
        {
            arrayOneOld[i].a = arrayOneOld[i].a * arrayTwoOld[i].a;
            arrayOneOld[i].b = arrayOneOld[i].b * arrayTwoOld[i].b;
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

    printf("%d\n---\n",arrayOneOld[5000].a);
    printf("%f\n---\n",arrayOneOld[5000].b);
    }

    }

    return 0;
}