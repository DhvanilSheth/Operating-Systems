#include <linux/kernel.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE4(kernel_2d_memcpy, float*, src, float*, dest, int, rows, int, cols)
{
    float buffer[10][10];

     /* Copy from user copies the data from src to buffer of size rows*cols */
    if (__copy_from_user(buffer, src, rows*cols*sizeof(float)))
        return -EFAULT;

    /* Copy to user copies the data from buffer to dest of size rows*cols */
    if (__copy_to_user(dest, buffer, rows*cols*sizeof(float)))
        return -EFAULT;

    return 0;
}
