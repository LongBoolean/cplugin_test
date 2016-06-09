#include <stdlib.h>
#include <stdio.h>

extern "C" 
{
    static int value = 0;

    void setup(int val)
    {
        value = val;
    }

    void update()
    {
        value++;
    }

    void print()
    {
        printf("Plugin A, value: %d\n", value);
    }
}
