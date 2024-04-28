#include <stdio.h>
#include <stdlib.h>

struct nodeOneOld
{
    //// Hot fields - most commonly used fields
    int a;
    double b;
    int c;
    //// Cold fields - least used fields
    long long int d : 48;
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

void checkIndex(double e)
{
    printf("Yes\n");
}

int main()
{
   int n = 999999;
   int i;
   for(i = 0; i < 100; i++)
    {

    struct nodeOneOld arrayOneLocal[100];
    struct nodeOneOld arrayTwoLocal[100];

    populateNodeOneOld(n);

    arrayOneLocal[i].a = 1;
    arrayOneLocal[i].b = 10.0;
    arrayOneLocal[i].c = 9;

    arrayTwoLocal[i].a = 1;
    arrayTwoLocal[i].b = 10.0;
    arrayTwoLocal[i].c = 9;

     multNodeOneOld(n);
     multArrays(n);

      //validity check
      if(i == 77)
     {
        printf("Validity check\n");

        struct nodeOneOld* result = arrayOneOld;
        printf("%d\n---\n",result[5000].a);

            arrayOneLocal[67].d = 90;
            printf("%lld\n",arrayOneLocal[67].d);

        printf("\na: ---\n%d",arrayOneOld[5000].a);
        printf("\nb: ---\n%f",arrayOneOld[5000].b);
        printf("\nc: ---\n%d",arrayOneOld[5000].c);
        printf("\nd: ---\n%lld",arrayOneOld[5000].d);
        printf("\ne: ---\n%d",arrayOneOld[5000].e);
        printf("\nf: ---\n%f",arrayOneOld[5000].f);
        printf("\ng: ---\n%f",arrayOneOld[5000].g);
        printf("\nh: ---\n%f",arrayOneOld[5000].h);
        printf("\ni: ---\n%f",arrayOneOld[5000].i);
        printf("\nj: ---\n%f\n",arrayOneOld[5000].j);
     }

    }


    return 0;
}