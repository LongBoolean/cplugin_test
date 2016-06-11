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
        value = value * 2;
    }

    void print()
    {
        printf("Plugin B, value: %d\n", value);
    }
}
