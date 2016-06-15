#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <dirent.h>
#include <string.h>
#include "shared.h"

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
    printf("Create plugin: %s\n", path);
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

bool
check_str_inside(const char *haystack_str, const char *needle_str)
{
    bool result = false;
    char haystack_char;
    char needle_char;
    int j = 0;
    for(int i=0; ; i++)
    {
        needle_char = needle_str[j];
        if(needle_char == '\0')
        {
            result = true;
            break;
        }
        
        haystack_char = haystack_str[i];
        if(haystack_char == '\0') break;

        //printf("haystack_char: %c, needle_char: %c\n", haystack_char, needle_char);
        if(haystack_char == needle_char)
        {
            j++;
        }
        else
        {
            j = 0;
        }

    }
    return result;
}

#define NUM_PLUGIN_MAX 16

global_variable const char *PLUGIN_DIR = "./plugins_bin/";
global_variable int num_plugins = 0;

int 
main()
{
    struct plugin *plugins_arr[NUM_PLUGIN_MAX];

    DIR *d;
    struct dirent *dir;
    d = opendir(PLUGIN_DIR);
    if(d)
    {
        while((dir = readdir(d)) != NULL)
        {
            if((dir->d_type == DT_REG) &&
               (check_str_inside(dir->d_name,".so")))
            {
                //add plugin
                if(num_plugins < NUM_PLUGIN_MAX)
                {
                    plugins_arr[num_plugins] = (struct plugin*)malloc(sizeof(struct plugin));
                    char path[64];
                    strcpy(path, PLUGIN_DIR);
                    strcat(path, dir->d_name);
                    create_plugin(plugins_arr[num_plugins], path);
                    num_plugins++;
                }
                else
                {
                    printf("Could not add plugin: %s, to many plugins.\n", dir->d_name);
                }
            }
        }
    }

    for(int i=0; i<10; i++)
    {
        for(int j=0; j<num_plugins; j++)
        {
            run_plugin(plugins_arr[j], i);
        }
        printf("******************************\n");
    }

    for(int j=0; j<num_plugins; j++)
    {
        remove_plugin(plugins_arr[j]);
    }

} 
