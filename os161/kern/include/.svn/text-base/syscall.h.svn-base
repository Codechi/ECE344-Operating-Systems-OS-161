#ifndef _SYSCALL_H_
#define _SYSCALL_H_

/*
 * Prototypes for IN-KERNEL entry points for system call implementations.
 */


//VERY IMPORTANT COMMENTS TO KNOW WHATS WHERE
// error codes in os161/kern/include/kern/errno.h
// file system macros etc in os161/kern/include/kern/unistd.h
// os161/include/unistd.h has user level interface of system calls e.g read, write
// os161/kern/include/lib.h contains kernel library calls like kprintf, getch etc
//system call numbers in callno.h

 
//system calls related to thread pids  like for example sys_fork, sys_waitpid, sys_getpid are mplemented in syscall.c
int sys_reboot(int code);
int sys_write(int filehandle, const char *buf, size_t size,int * returnval);//called by write function
int sys_read(int filehandle, char *buf, size_t size, int * returnval);
int sys_getpid(void);


#endif /* _SYSCALL_H_ */

