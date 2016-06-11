#ifndef SHARED_H
#define SHARED_H

//different names for different uses of static
#if !defined(internal)
#define internal static
#endif
#define local_persist static
#define global_variable static

//temporary assertion macro - not final
// TODO(nick): remember to disable assertions on non dev machines
#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}

#define InvalidCodePath Assert(!"InvalidCodePath")
#define InvalidDefaultCase default: {InvalidCodePath;} break

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

#endif
