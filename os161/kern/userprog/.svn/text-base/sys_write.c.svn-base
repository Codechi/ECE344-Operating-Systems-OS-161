#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <uio.h>
#include <elf.h>
#include <addrspace.h>
#include <thread.h>
#include <curthread.h>
#include <vnode.h>
#include <syscall.h>

//added new
#include <kern/unistd.h>


//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//included in syscall.h header file
int sys_write(int filehandle, const char *buf, size_t size, int* returnval)//called by write function
{
	
		if(filehandle!=1)//fd is not a valid file descriptor, or was not opened for writing.
		{
			*returnval = EBADF;
			return -1;
		}
	
		if (buf == NULL)//Part or all of the address space pointed to by buf is invalid
		{
			*returnval = EFAULT;
			return -1; 
		}
	
	//kprintf(buf); actually may just only need to do this as using kernel library calls
	
	struct vnode *vn;
	struct uio write_uio;
	
	char *console = NULL;	// screen is console here
	console = kstrdup("con:");
	
	int result_of_vfs_open;

	
	result_of_vfs_open = vfs_open(console, 1, &vn);	//used to create a vnode, but unsure about mode bit whether 1 or not
	kfree(console);
		
	//similar to loadelf.c and see uio.h and uio.c
	
	//the uio structure has an ioviec structure. the iovec is used to determine the length 
	//and location of the buffer that is is used. It could be either bein application 
	//part of the kernel address or in the kernal part
	write_uio.uio_iovec.iov_ubase = (userptr_t) buf;
	write_uio.uio_iovec.iov_len = size;   // length of memory space


	write_uio.uio_resid = size;//amount to actually transfer
	write_uio.uio_offset = 0;//desired offset into object

	//uio_segflg determines whether buffer is in user or kernel part of address space
	write_uio.uio_segflg = UIO_USERSPACE;
	
	write_uio.uio_rw = UIO_WRITE;//identifies whether the transfer is from the file/device or to the file/device,
	
	//uio_space points to the addrspace object
	// of the process that is doing the VOP_READ or VOP_WRITE, onlu used if buffer is in user
	// part of address space or else set to NULL
	write_uio.uio_space = curthread->t_vmspace;
	//curthread is global variable in thread.c for the thread cuurently executing
	//t_vmspace in thread.h
	//i think current thread's address space. 
	
	//VOP_WRITE = (vn, uio);//prototype in kern/include/vnode.h
	int vop_write_return;
	vop_write_return = VOP_WRITE (vn, &write_uio);
	
	if (vop_write_return == 0)
	{
		*returnval = size;
 		return 0;
	}
	else	//may be done here - ENOSPC - There is no free space remaining on the filesystem containing the file, EIO - A hardware I/O error occurred writing the data.
			
	{
		*returnval = vop_write_return;
 		return -1;
	}	

}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

