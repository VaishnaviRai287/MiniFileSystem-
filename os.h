/* os.h */
#pragma once
#define bool int8
#define true 1
#define false 0

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;
typedef unsigned long long int int64;
typedef int8 fd;
typedef int8 error;
typedef int16 ptr;

#define $1 (int8 *)
#define $2 (int16)
#define $4 (int32)
#define $8 (int64)
#define $c (char *)
#define $i (int)
#define $v (void *)

#define packed __attribute__((packed))
#define public __attribute__((visibility("default")))
#define internal __attribute__((visibility("hidden")))
#define private static

#define reterr(x)   do {\
    errnumber = (x);        \
    return 0;           \
} while(false)
#define throw return 0
#define alloc(x)    malloc($i x)
#define destroy(x)  free(x)

#ifdef Library
 public bool initialized = false;
 public error errnumber;
#else
 extern public bool initialized;
 extern public error errnumber;
#endif

public packed enum {
    ErrNoErr,
    ErrInit,
    ErrIO,
    ErrBadFD,
    ErrNotAttached,
    ErrNoMem,
    ErrBusy,
    ErrArg,
    ErrFilename,
    ErrInode,
    ErrBadDir,
    ErrNotFound,
    ErrDrive,
    ErrPath
};

public bool load(fd,int8);
public int8 store(fd);
public void init(void);
public void dinit(void);

public void *opendir(int8*);
public ptr mkdir1(int8*);
