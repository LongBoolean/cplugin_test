#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

void loadLib(void *handle, char *lib_path)
{
    handle = dlopen (lib_path, RTLD_NOW);
    if (!handle) {
        fputs (dlerror(), stderr);
        printf("\nError in loadLib\n");
        exit(1);
    }
}

void* getLibFunc(void *handle, char *func_name)
{
    char *error;
    void* func;
    func = dlsym(handle, func_name);  
    if ((error = dlerror()) != NULL)  {
        fputs(error, stderr);
        printf("\nError in getLibFunc\n");
        exit(1);
        /*
         *This is the output I get when running:
         $ make run
         ./cplugin_test
         ./cplugin_test: undefined symbol: print
         Error in getLibFunc
         make: *** [run] Error 1
         */
    }
    return func;
}

int main()
{
    void *handle;

    //void (*setup)(int);
    //void (*update)();
    void (*print)();

    //todo(nick): find better way to deturmine full path
    loadLib(handle, "./pluginA.so");
    //setup = (void (*)(int))getLibFunc(handle, "setup");
    //update = (void (*)())getLibFunc(handle, "update");
    print = (void (*)())getLibFunc(handle, "print");

    //(*setup)(2);
    //(*update)();
    (*print)();
    
    dlclose(handle);
} 
