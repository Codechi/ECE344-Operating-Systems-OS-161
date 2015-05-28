//new vm.c now has dumbvm.c contents


#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <thread.h>
#include <curthread.h>
#include <addrspace.h>
#include <vm.h>
#include <machine/spl.h>
#include <machine/tlb.h>

/*
 * Dumb MIPS-only "VM system" that is intended to only be just barely
 * enough to struggle off the ground. You should replace all of this
 * code while doing the VM assignment. In fact, starting in that
 * assignment, this file is not included in your kernel!
 */

/* under dumbvm, always have 48k of user stack */
#define DUMBVM_STACKPAGES    12

int total_pages;
int done_vm_bootstrap;

struct coremap_element *coremap_ptr; //coremap_ptr points to a V.A. It points to first coremap entry

 struct coremap_element my_coremap [77];

//making own lock variables variable


//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void
vm_bootstrap(void)
{
	/* Do nothing. */

	//kprintf("In vm_bootstrap: Done_vm_bootstrap = %d\n",done_vm_bootstrap);
	
	paddr_t firstpaddress, lastpaddress,freepaddress;
	
	ram_getsize(&firstpaddress, &lastpaddress);	//ram_getsize in ram.c
	
	//lock_alloc_kpages = sem_create("lock_alloc_kpages",1);	//lock for now

	//kprintf("before adjusting firstpaddress is %d\n",firstpaddress);
	
	while((firstpaddress % 4096) != 0)
	{
		firstpaddress++;
	}
	//kprintf("adjusting firstpaddress is %d\n",firstpaddress);	
		
	total_pages = (lastpaddress - firstpaddress) / (PAGE_SIZE);
	//kprintf("Total number of free pages in physical memory: %d\n",total_pages);
			
	//initialzing coremap entries			
	int  i = 0;
	for(i = 0;  i < total_pages; i++)
	{
		//putting in coremap entry details BUT DIDNT SET VIRTUAL ADDRESS YET!!!
		my_coremap[i].physical_address =firstpaddress + i*PAGE_SIZE;
		my_coremap[i].virtual_address =PADDR_TO_KVADDR(firstpaddress + i*PAGE_SIZE);
		my_coremap[i].state = FREE;
		my_coremap[i].num_continuous_pages = 0;
	}
	
	//Printing out coremap info
	for (i = 0; i < total_pages; i++)	//set to 51 for now as too many entries if RAM big
	{
		
		//kprintf("page entry: %d  physical adress: %u virtual address: %u state: %d\n", i, my_coremap[i].physical_address, my_coremap[i].virtual_address, my_coremap[i].state);
		
	}
	done_vm_bootstrap = 1;
	//kprintf("In vm_bootstrap: Done_vm_bootstrap = %d\n",done_vm_bootstrap);
	
	//splx(spl);//not in code
	return;
		
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

paddr_t
getppages(unsigned long npages)	//called from alloc_kpages
{
		/* OLD CODE - this works
		int spl;
		paddr_t addr;
		spl = splhigh();
		addr = ram_stealmem(npages);	//in ram.c - just moves firstpaddr by that many pages
		splx(spl);
		return addr;
		*/

	//NEW CODE
	///*
	int spl;
	spl = splhigh();
	paddr_t addr;
	
	if(done_vm_bootstrap == 0)
	{
		//Cant print anytihg here, cause everything else not set up when enters here		
		addr = ram_stealmem(npages);	//in ram.c - just moves firstpaddr by that many pages
		splx(spl);
		return addr;		
	}

	else //(done_vm_bootstrap ==1)
	{
			
			//kprintf("in getppages where done_vm_bootstrap = %d\n",done_vm_bootstrap);
			//kprintf("npages in getppages= %d\n",npages);
					
			int consectuve_pages_found = 0;
			int i;
			
			int starting_page = 0;
			
			//looping through all pages to find npages free consecutive pages
			for (i = 0; i < total_pages; i++) //was will (lastpaddr / PAGE_SIZE)
			{
				{
					if (my_coremap[i].state == FREE)//Found a free page
					{	
						consectuve_pages_found++;
						//kprintf("FOUND A FREE PAGE, its page no. is : %d\n and consecutive pages found right now is %d\n",i,consectuve_pages_found);
					}
					
					else //not a free page
					{	
						consectuve_pages_found = 0;
						//kprintf("page %d has state %d so cant use\n",i,element_access_ptr->state);	
					}
				}
								
				if(consectuve_pages_found == npages)	//found npages consecutive pages
				{
					starting_page = i- npages + 1;
					//kprintf("FOUND %d consecutive pages and starting page is %d so BREAK\n",consectuve_pages_found,starting_page);
					break;
				}
			}			
			
			//change this later - now doing tricks
			if(consectuve_pages_found != npages)//did not find npages consecutive pages
			{
				//kprintf("only found %d consecutive pages so return back with 0\n",consectuve_pages_found);
				/*
				for (i = 0; i <npages; i++)
				{
					my_coremap[i].state = FREE;
					my_coremap[i].num_continuous_pages = 0;
				}
				
				for(i=0;i<npages;i++)
				{
					my_coremap[i].state = FIXED;	
				}
				
				my_coremap[0].num_continuous_pages = 0;
				addr = my_coremap[0].physical_address;
				*/				
				splx(spl);
				//return 0;
				return 0;
			}
				
			//found enough consecutive pages
			for(i=0;i<npages;i++)
			{
				//no vitrual right now, MAY NEED LATER
				my_coremap[starting_page + i].state = FIXED;
			
				//kprintf("physical address for %dth page being assigned is %u and hex representation is %x\n",i,element_access_ptr->physical_address,element_access_ptr->physical_address);
			}
						
			
		 	//finding physical address of first page of the ones chosen
			addr = my_coremap[starting_page].physical_address;
			my_coremap[starting_page].num_continuous_pages = npages;
			
			//kprintf("Physical address being returned: %u and hex representation is %x\n",addr,addr);
			
			
	//Printing out coremap info
			for (i = 0; i < total_pages; i++)	//set to 51 for now as too many entries if RAM big
			{
		
				//kprintf("page entry: %d  physical adress: %u virtual address: %u state: %d\n", i, my_coremap[i].physical_address,my_coremap[i].virtual_address,my_coremap[i].state);
		
			}
			
			
			splx(spl);
			return addr;
	}

	//*/			

}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


/* Allocate/free some kernel-space virtual pages */
vaddr_t 
alloc_kpages(int npages)	//called from kmalloc. which is in lib/kheap.c
{		
		
		//kprintf("inside alloc_kpages: value of done_vm_bootstrap = %d\n", done_vm_bootstrap);
		paddr_t pa;
		pa = getppages(npages);
		if (pa==0) {
			//kprintf("in alloc_kpages: returning %u and hex representation is %x to kheap\n",pa,pa);
			//means couldnt find enough pages
			return 0;
		}
		//kprintf("in alloc_kpages: returning %u and hex representation is %x to kheap\n",pa,pa);
		
		return PADDR_TO_KVADDR(pa);
	
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void 
free_kpages(vaddr_t addr)
{

	/* nothing */
	int spl;
	spl = splhigh();
	
	//kprintf("inside free_kpages, value of done_vm_bootstrap = %d\n", done_vm_bootstrap);

	paddr_t paddr = addr - MIPS_KSEG0;
	
	//kprintf("In free_kpages: virtual address is %u\n",addr);
	//kprintf("In free_kpages: converted physical address is %u\n",paddr);
	
	int i;
	for(i = 0;i<total_pages;i++)
	{
		if(my_coremap[i].physical_address == paddr)
		{
			break;
		}
	}
	
	int my_pages;
	my_pages = my_coremap[i].num_continuous_pages;
	
	//kprintf("In free_kpages: num_continuous_pages = %d\n",my_pages);
	
	int j;
	for (j = 0; j <my_pages; j++)
	{
		my_coremap[i].state = FREE;
		my_coremap[i].num_continuous_pages = 0;

	}
	
	for (i = 0; i < total_pages; i++)	//set to 51 for now as too many entries if RAM big
	{
		
		//kprintf("page entry: %d  physical adress: %u virtual address: %u state: %d\n", i, my_coremap[i].physical_address,my_coremap[i].virtual_address,my_coremap[i].state);
		
	}
	splx(spl);
	return;

	//(void)addr;
	
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int
vm_fault(int faulttype, vaddr_t faultaddress)
{
	vaddr_t vbase1, vtop1, vbase2, vtop2, stackbase, stacktop;
	paddr_t paddr;
	int i;
	u_int32_t ehi, elo;
	struct addrspace *as;
	int spl;

	spl = splhigh();

	faultaddress &= PAGE_FRAME;

	DEBUG(DB_VM, "dumbvm: fault: 0x%x\n", faultaddress);

	switch (faulttype) {
	    case VM_FAULT_READONLY:
		/* We always create pages read-write, so we can't get this */
		panic("dumbvm: got VM_FAULT_READONLY\n");
	    case VM_FAULT_READ:
	    case VM_FAULT_WRITE:
		break;
	    default:
		splx(spl);
		return EINVAL;
	}

	as = curthread->t_vmspace;
	if (as == NULL) {
		/*
		 * No address space set up. This is probably a kernel
		 * fault early in boot. Return EFAULT so as to panic
		 * instead of getting into an infinite faulting loop.
		 */
		return EFAULT;
	}

	/* Assert that the address space has been set up properly. */
	assert(as->as_vbase1 != 0);
	assert(as->as_pbase1 != 0);
	assert(as->as_npages1 != 0);
	assert(as->as_vbase2 != 0);
	assert(as->as_pbase2 != 0);
	assert(as->as_npages2 != 0);
	assert(as->as_stackpbase != 0);
	assert((as->as_vbase1 & PAGE_FRAME) == as->as_vbase1);
	assert((as->as_pbase1 & PAGE_FRAME) == as->as_pbase1);
	assert((as->as_vbase2 & PAGE_FRAME) == as->as_vbase2);
	assert((as->as_pbase2 & PAGE_FRAME) == as->as_pbase2);
	assert((as->as_stackpbase & PAGE_FRAME) == as->as_stackpbase);

	vbase1 = as->as_vbase1;
	vtop1 = vbase1 + as->as_npages1 * PAGE_SIZE;
	vbase2 = as->as_vbase2;
	vtop2 = vbase2 + as->as_npages2 * PAGE_SIZE;
	stackbase = USERSTACK - DUMBVM_STACKPAGES * PAGE_SIZE;
	stacktop = USERSTACK;

	if (faultaddress >= vbase1 && faultaddress < vtop1) {
		paddr = (faultaddress - vbase1) + as->as_pbase1;
	}
	else if (faultaddress >= vbase2 && faultaddress < vtop2) {
		paddr = (faultaddress - vbase2) + as->as_pbase2;
	}
	else if (faultaddress >= stackbase && faultaddress < stacktop) {
		paddr = (faultaddress - stackbase) + as->as_stackpbase;
	}
	else {
		splx(spl);
		return EFAULT;
	}

	/* make sure it's page-aligned */
	assert((paddr & PAGE_FRAME)==paddr);

	for (i=0; i<NUM_TLB; i++) {
		TLB_Read(&ehi, &elo, i);
		if (elo & TLBLO_VALID) {
			continue;
		}
		ehi = faultaddress;
		elo = paddr | TLBLO_DIRTY | TLBLO_VALID;
		DEBUG(DB_VM, "dumbvm: 0x%x -> 0x%x\n", faultaddress, paddr);
		TLB_Write(ehi, elo, i);
		splx(spl);
		return 0;
	}

	kprintf("dumbvm: Ran out of TLB entries - cannot handle page fault\n");
	splx(spl);
	return EFAULT;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

