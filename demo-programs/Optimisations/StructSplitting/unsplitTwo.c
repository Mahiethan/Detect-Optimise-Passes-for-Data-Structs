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

int main()
{
   int n = 999999; 
   int i;
   free(NULL); //required to have its declaration in the IR - if it doesn't exist already
   for(i = 0; i < 100; i++)
    {
    ////dynamic
    struct nodeOneOld* arrayOneOld = (struct nodeOneOld*) malloc(n*sizeof(struct nodeOneOld));
    struct nodeTwoOld* arrayTwoOld = (struct nodeTwoOld*) malloc(999999*88);

     populateNodeOneOld(arrayOneOld,n);
     populateNodeTwoOld(arrayTwoOld,n);

     multNodeOneOld(arrayOneOld,n);
     multArrays(arrayOneOld,arrayTwoOld,n);

     //validity check
    if(i == 77)
     {
        printf("Validity check\n");

        printf("%d\n---\n",arrayOneOld[5].a);
        printf("%f\n---\n",arrayOneOld[5].b);
        printf("%d\n---\n",arrayTwoOld[5].a);
        printf("%f\n---\n",arrayTwoOld[5].b);


        printf("%f\n---\n",arrayOneOld[5].d);
        printf("%c\n---\n",arrayOneOld[5].e);
        printf("%f\n---\n",arrayTwoOld[5].d);
        printf("%c\n---\n",arrayTwoOld[5].e);
     }

     free(arrayOneOld); 

     free(arrayTwoOld);
    }

    return 0;
}