#include "funcs.h"
#include <dos.h>

#include <alloc.h>
#include <mem.h>

unsigned char   Surface[64000U];

void Mode(int m)
{
    if (m == 0)
    {
        asm
        {
            mov ax,3;
            int 0x10;
        }
    }
    else if (m == 1)
    {
        asm
        {
            mov ax,0x13;
            int 0x10;
        }
    }
}

void Blit(void)
{
    //  WaitRetrace();
    memcpy(((void *) (0xA0000000L)), Surface, 64000U);
}

void WaitRetrace(void)
{
    asm
    {
        mov dx,0x3DA
    }
    l1:
    asm
    {
        in al,dx
        and al,0x08
        jnz l1
    }
    l2:
    asm
    {
        in al,dx
        and al,0x08
        jz  l2
    }
}