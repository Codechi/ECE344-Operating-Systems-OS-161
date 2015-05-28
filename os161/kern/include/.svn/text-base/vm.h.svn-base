#ifndef _VM_H_
#define _VM_H_

#include <machine/vm.h>

//macros for page states
#define FREE 1
#define DIRTY 2
#define CLEAN 3
#define FIXED 4


/*
 * VM system-related definitions.
 *
 * You'll probably want to add stuff here.
 */
 
 //coremap details

 struct coremap_element	//the object definition. Initialized in void vm_bootstrap(void) in os161/kern/vm.c
 {
 	struct addrspace* address_space;	//in os161/kern/vm/addrspace.h - address space of process. RIght now using dumb vm user space of just 2 segments
 	paddr_t physical_address;	//physical address but pearls of life guy dont say to put this in
	vaddr_t virtual_address;	//virtual address
 	int state;//state of the page. Macros defined above
 	int num_continuous_pages;

 };
 
//extern struct coremap_element *coremap_ptr;//points to 


/* Fault-type arguments to vm_fault() */
#define VM_FAULT_READ        0    /* A read was attempted */
#define VM_FAULT_WRITE       1    /* A write was attempted */
#define VM_FAULT_READONLY    2    /* A write to a readonly page was attempted*/


/* Initialization function */
void vm_bootstrap(void);


/* Fault handling function called by trap code */
int vm_fault(int faulttype, vaddr_t faultaddress);

/* Allocate/free kernel heap pages (called by kmalloc/kfree) */
vaddr_t alloc_kpages(int npages);
void free_kpages(vaddr_t addr);

paddr_t getppages(unsigned long npages);


#endif /* _VM_H_ */
