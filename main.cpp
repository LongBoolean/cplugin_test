#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

void 
loadLib(void **handle, const char *lib_path)
{
    *handle = dlopen (lib_path, RTLD_NOW);
    if (!*handle) {
        fputs (dlerror(), stderr);
        printf("\nError in loadLib\n");
    }
}

void* 
getLibFunc(void *handle, const char *func_name)
{
    char *error;
    void* func;
    func = dlsym(handle, func_name);  
    if ((error = dlerror()) != NULL)  {
        fputs(error, stderr);
        printf("\nError in getLibFunc\n");
    }
    return func;
}

struct plugin
{
    void *handle;
    void (*setup)(int);
    void (*update)();
    void (*print)();
};

void 
create_plugin(struct plugin *Plugin, const char* path)
{
    loadLib(&Plugin->handle, path);
    Plugin->setup = (void (*)(int))getLibFunc(Plugin->handle, "setup");
    Plugin->update = (void (*)())getLibFunc(Plugin->handle, "update");
    Plugin->print = (void (*)())getLibFunc(Plugin->handle, "print");
}

void
run_plugin(struct plugin *Plugin, int i)
{
    if(Plugin->handle)
    {
        //todo(nick): make sure function pointers exist aswell
        if(i==0)
            Plugin->setup(1);
        Plugin->print();
        Plugin->update();
    }
    else
    {
        printf("NULL Plugin.\n");
    }
}

void 
remove_plugin(struct plugin *Plugin)
{
    if(Plugin->handle)
    {
        dlclose(Plugin->handle);
    }
}

int 
main()
{
    struct plugin *PluginA = (struct plugin*)malloc(sizeof(struct plugin));
    struct plugin *PluginB = (struct plugin*)malloc(sizeof(struct plugin));


    //todo(nick): find better way to deturmine full path
    create_plugin(PluginA, "./pluginA.so");
    create_plugin(PluginB, "./pluginB.so");

    for(int i=0; i<10; i++)
    {
        run_plugin(PluginA, i);
        run_plugin(PluginB, i);
        printf("******************************\n");
    }

    remove_plugin(PluginA);
    remove_plugin(PluginB);
} 
