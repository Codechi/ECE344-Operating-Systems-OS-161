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




//incuded in syscall.h file
int sys_read(int filehandle, char *buf, size_t size, int * returnval)//filehandle gets 0 if sys_read is called (tested)
{
	//int sys_read(int filehandle, char *buf, size_t size, int * returnval)
	
	
	if(filehandle!=0)//fd is not a valid file descriptor, or was not opened for reading.
	{
		*returnval = EBADF;
		return -1;
	}
	
	if(buf == NULL)//Part or all of the address space pointed to by buf is invalid
	{
		*returnval = EFAULT;
		return -1; 
	}
		
	//kprintf("filehandle number for sys_read is %d\n", filehandle);
	
	struct vnode *vn;
    	struct uio u;
	
	char *console = NULL;	// screen is console here
	console = kstrdup("con:");
	
	int result_of_vfs_open;
	
    //used to create a vnode, but unsure about mode bit whether 1 or not
	result_of_vfs_open = vfs_open(console, 0, &vn);	
	kfree(console);
	
	u.uio_iovec.iov_ubase = (userptr_t) buf;
	u.uio_iovec.iov_len = size;   // length of the memory space
	u.uio_resid = size;          // amount to actually read
	u.uio_offset = 0;
	u.uio_segflg = UIO_USERSPACE;
 	u.uio_rw = UIO_READ;
 	u.uio_space = curthread->t_vmspace;

	int vop_read_return;
	vop_read_return = VOP_READ(vn, &u);
	
 	if (vop_read_return == 0)//if no error in VOP_READ
 	 {
 		*returnval = size;
 		return 0;
 	}
 	
 	else//error in VOP_READ, I think check for EIO is done here
 	{
 		*returnval = vop_read_return;
 		return -1;
 	}

	
//EBADF		fd is not a valid file descriptor, or was not opened for reading.
//EFAULT		Part or all of the address space pointed to by buf is invalid.
//EIO		A hardware I/O error occurred reading the data.


}
