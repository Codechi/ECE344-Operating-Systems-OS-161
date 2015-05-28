/*
 * Sample/test code for running a user program.  You can use this for
 * reference when implementing the execv() system call. Remember though
 * that execv() needs to do more than this function does.
 */

#include <types.h>
#include <kern/unistd.h>
#include <kern/errno.h>
#include <lib.h>
#include <addrspace.h>
#include <thread.h>
#include <curthread.h>
#include <vm.h>
#include <vfs.h>
#include <test.h>

/*
 * Load program "progname" and start running it in usermode.
 * Does not return except on error.
 *
 * Calls vfs_open on progname and thus may destroy it.
 */
 
 
 //for example progname is args[0] = testbin/add, and args[0] is testbin/add, args[1]="2" and args[2]="3"
 //called from os161/kern/main/menu.c by cmd_progthread
 //args[nargs] = NULL, the last element of args is NULL
int
runprogram(char *progname, char *const *args, unsigned long nargs) 
{
	
	struct vnode *v;	//vnode in os161/kern/include/vnode.h.  A struct vnode is an abstract representation of a file.
	vaddr_t entrypoint, stackptr;
	int result;

	/* Open the file. */
	result = vfs_open(progname, O_RDONLY, &v);
	if (result) {
		return result;
	}
	
	/* We should be a new thread. */
	assert(curthread->t_vmspace == NULL);

	/* Create a new address space. */
	curthread->t_vmspace = as_create();
	if (curthread->t_vmspace==NULL) {
		vfs_close(v);
		return ENOMEM;
	}

	/* Activate it. */
	as_activate(curthread->t_vmspace);

	/* Load the executable. */
	// load_elf(struct vnode *v, vaddr_t *entrypoint)
	// Load an ELF executable user program into the current address space.
 	//Returns the entry point (initial PC) for the program in ENTRYPOINT.
	
	result = load_elf(v, &entrypoint);	//load_elf in os161/kern/userprog
	if (result) {
		/* thread_exit destroys curthread->t_vmspace */
		vfs_close(v);
		return result;
	}

	/* Done with the file now. */
	vfs_close(v);

	/* Define the user stack in the address space */
	result = as_define_stack(curthread->t_vmspace, &stackptr);//sets my stack pointer
	if (result) {
		/* thread_exit destroys curthread->t_vmspace */
		return result;
	}
	
	//-------------------------Added in to handle arguments---------------------------------------------------
	//copyout function in copyinout.c in os161/kern/lib/copyinout.c
	int a;
	
	for(a=0;a<nargs;a++)
	{
		//kprintf("args[%d] = %s\n\n",a,args[a]);
		
	}
	//kprintf("nargs = %d\n\n", (int) nargs);

	int i;
	vaddr_t * user_arg = kmalloc((nargs + 1)*sizeof(vaddr_t) );
	
	args[nargs] = NULL;
	//kprintf("now working with stack pointer etc\n");
	for( i = nargs-1; i >=0 ; i --){//to copy arguments on user_arg
		int arglenght = strlen(args[i]) + 1;
		//kprintf("length of %s is %d\n",args[i],arglenght);
			stackptr -= arglenght;
			user_arg[i] = stackptr;			
  			copyout( args[i] , stackptr, arglenght );	//int copyout(const void *src, userptr_t userdest, size_t len) copies from src to userdest          
	}
	
	if(stackptr % 4 != 0) stackptr -= ( stackptr%4);
	stackptr -= (nargs + 1)*sizeof(vaddr_t);
	
	copyout(user_arg, stackptr, (nargs + 1)*sizeof(vaddr_t));//copies whole user_arg
	
	//--------------------------------------------End handling arguments-----------------------------------------------------
	
	

	/* Warp to user mode. */	//function in os161/kern/arch/mips/mips  
	// md_usermode is meant for use in exec and equivalent.
	
	md_usermode(nargs/*argc*/, stackptr /*userspace addr of argv*/,
		    stackptr, entrypoint);
	
	/* md_usermode does not return */
	panic("md_usermode returned\n");
	return EINVAL;
}

