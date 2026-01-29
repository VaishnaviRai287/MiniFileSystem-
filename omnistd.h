/* omnistd.h */
#pragma once
#include <stdio.h>
#include <osapi.h>

#define kprintf(f, args...) printf(f "\n", args)

#define cmp(d,s,n)          memorycmp(d,s,n,false)
#define copy(d,s,n)         memorycopy(d,s,n,false)
#define stringcopy(d,s,n)   memorycopy(d,s,n,true)
#define stringcmp(d,s,n)    memorycmp(d,s,n,true)
#define findcharl(s,c)      findchar(s,c,true)
#define findcharr(s,c)      findchar(s,c,false)

#define getbit_(b,p)        ((b & (1 << (p))) >> (p))
#define unsetbit_(b,p)      (b & ~(1 << (p)))
#define setbit_(b,p)        (b | (1 << (p)))

internal void zero(int8*,int16);
internal void memorycopy(int8*,int8*,int16,bool);
internal bool memorycmp(int8*,int8*,int16,bool);
internal int16 stringlen(int8*);

internal bool getbit(int8*,int16);
internal void setbit(int8*,int16,bool);

internal void tolowercase(int8*);
internal int8 low(int8);

internal int8 *findchar(int8*,int8,bool);
