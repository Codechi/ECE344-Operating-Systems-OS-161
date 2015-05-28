/*
 * catsem.c
 *
 * 30-1-2003 : GWA : Stub functions created for CS161 Asst1.
 *
 * NB: Please use SEMAPHORES to solve the cat syncronization problem in 
 * this file.
 */



/*
 * 
 * Includes
 *
 */

#include <types.h>
#include <lib.h>
#include <test.h>
#include <thread.h>

/*
 * 
 * Constants
 *
 */

/*
 * Number of food bowls.
 */

#define NFOODBOWLS 2

/*
 * Number of cats.
 */

#define NCATS 6

/*
 * Number of mice.
 */

#define NMICE 2


//semaphores
struct semaphore *cat_lock;	//locks other cats
struct semaphore *mouse_lock;//locks other mice
struct semaphore *cat_or_mouse;//locks to either a cat or a mouse
struct semaphore *bowls;//locks two animals of the same type to the 2 bowls
struct semaphore *eating;//taking care of bowl assignments before and after eating

struct semaphore *print;//to lock print statements (not needed anymore as locks are now implemented)


//cat and mouse counters
int cat_count = 0;
int mouse_count = 0;

//bowl status where 0 means vacant, 1 means occupied
int bowl1_status = 0;
int bowl2_status = 0;

/*
 * 
 * Function Definitions
 * 
 */

/* who should be "cat" or "mouse" */
static void
sem_eat(const char *who, int num, int bowl, int iteration)
{
	// if want cats can eat like 1 guy  starts eating than oter guy than first guy stops. if want sequential like in sample output remove these and put before function call
		kprintf("%s: %d starts eating: bowl %d, iteration %d\n", who, num,  bowl, iteration);
		clocksleep(1);
	
		 
	
		kprintf("%s: %d ends eating: bowl %d, iteration %d\n", who, num, bowl, iteration);
      

}

/*
 * catsem()
 *
 * Arguments:
 *      void * unusedpointer: currently unused.
 *      unsigned long catnumber: holds the cat identifier from 0 to NCATS - 1.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Write and comment this function using semaphores.
 *
 */

static
void
catsem(void * unusedpointer, 
       unsigned long catnumber)
{
        /*
         * Avoid unused variable warnings.
         */

        (void) unusedpointer;
        (void) catnumber;
        
        int cat_iteration = 0;
        int bowl = 0;
        
        while(cat_iteration<4)//keep trying to eat until this cat has eaten 4 times
        {
		P(cat_lock);
			cat_count++;
			if(cat_count ==1)//first cat will have main binary mutex to choose whether a mouse or a cat can eat
				P(cat_or_mouse);
				
		V(cat_lock);
			//kprintf("GOING TO ENTER bowls semaphore by catnumber %d\n",catnumber);	//for DEBUGGING keep this
		
		P(bowls);//2 cats enter this
			
				//kprintf("ENTERED bowls semaphore by catnumber %d\n",catnumber);	//for DEBUGGING keep this
		
			P(eating);//1 cat goes through while other one waits, as bowls are getting assigned
				if(bowl1_status == 0)
				{
					bowl = 1;
					bowl1_status = 1;
				}
				else if (bowl2_status == 0)
				{
					bowl = 2;
					bowl2_status = 1;	
				}
				else
				{
				
						kprintf("SHOULD NOT ENTER HERE: No bowl free for catnumber %d\n",catnumber);//EVEN IF ON, does not get here. TRY WITH TESTER
					
				}	
			V(eating);//releases 1 more cat of the 2 selected to go through
			
					//sem_eat(const char *who, int num, int bowl, int iteration)
					sem_eat("cat",catnumber,bowl,cat_iteration);
					cat_iteration++;
			
			P(eating);//1 cat goes through while the other one waits, as bowls are getting assigned
				if (bowl == 1)
					{
						bowl = 0;
						bowl1_status =0;
					}
		
				else if (bowl == 2)
				{	
					bowl = 0;
					bowl2_status = 0;	
				}
				else
					{
				
					kprintf("SHOULD NOT GET HERE: means catnumber: %d had no bowl\n",catnumber);//EVEN IF ON, does not get here. TRY WITH TESTER
					
					}
			V(eating);			
	
			P(cat_lock);
				if(cat_count ==1)//last cat will enter this
					{
						V(cat_or_mouse);//release main lock by last cat, so mice can enter now if want			
					}
				cat_count--;
			V(cat_lock);
		
		V(bowls);

        }
}
        

/*
 * mousesem()
 *
 * Arguments:
 *      void * unusedpointer: currently unused.
 *      unsigned long mousenumber: holds the mouse identifier from 0 to 
 *              NMICE - 1.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Write and comment this function using semaphores.
 *
 */

static
void
mousesem(void * unusedpointer, 
         unsigned long mousenumber)
{
        /*
         * Avoid unused variable warnings.
         */

        (void) unusedpointer;
        (void) mousenumber;
        
        int mouse_iteration = 0;
        int bowl = 0;
        
        while(mouse_iteration<4)
        {

        	
		P(mouse_lock);
			mouse_count++;
		
			if(mouse_count ==1)//first mouse will have main binary mutex to choose whether a mouse or a cat can eat
				P(cat_or_mouse);//main lock for either mouse or cat
		
		V(mouse_lock);
		
			
				//kprintf("GOING TO ENTER bowls semaphore by mousenumber %d\n",mousenumber);	//KEEP FOR DEBUGGING
			
		
		P(bowls);	//2 mice enter 
			
			P(eating);//1 mouse goes through
				if(bowl1_status == 0)
				{
					bowl = 1;
					bowl1_status = 1;
				}
				else if( bowl2_status == 0)
				{
					bowl = 2;
					bowl2_status = 1;
				}
				else
				{
					
						kprintf("SHOULD NOT ENTER HERE: No bowl free for mouse: %d\n",mousenumber); //EVEN IF ON, does not get here. TRY WITH TESTER
					
				}
				
			V(eating);	//release 1 more mouse		
			
				//sem_eat(const char *who, int num, int bowl, int iteration)
				sem_eat("mouse",mousenumber,bowl,mouse_iteration);
				mouse_iteration++;
			
			P(eating);
				if(bowl ==1)
				{
					bowl = 0;
					bowl1_status =0;
				}
				else if (bowl ==2)
				{
					bowl = 0;
					bowl2_status = 0;
				}
				else
				{
					
						kprintf("SHOULD NOT GET HERE: means mouse: %d had no bowl\n",mousenumber); //EVEN IF ON, does not get here. TRY WITH TESTER
					
				}	
			V(eating);	
		V(bowls); //release 1 bowl
				
			P(mouse_lock);
				if(mouse_count ==1)	//release main lock by last mouse, so cats can enter now if want
					V(cat_or_mouse);	
				
				mouse_count--;
			V(mouse_lock);
	}
}


/*
 * catmousesem()
 *
 * Arguments:
 *      int nargs: unused.
 *      char ** args: unused.
 *
 * Returns:
 *      0 on success.
 *
 * Notes:
 *      Driver code to start up catsem() and mousesem() threads.  Change this 
 *      code as necessary for your solution.
 */

int
catmousesem(int nargs,
            char ** args)
{
        int index, error;
   
        /*
         * Avoid unused variable warnings.
         */

        (void) nargs;
        (void) args;
        
        
   	//making semaphores
   	cat_lock = sem_create("cat_lock",1);//to lock other cats
   	mouse_lock = sem_create("mouse_lock",1);//to lock other mice
   	
   	cat_or_mouse = sem_create("cat_or_mouse",1);//to lock to either a cat or a mouse
	bowls = sem_create("bowls",2);	//counting semphore to lock bowl to 2 animals of the same type
	eating = sem_create("eating",1);
	print = sem_create("print",1);
	 

        
   
        /*
         * Start NCATS catsem() threads.
         */

        for (index = 0; index < NCATS; index++) {
           
                error = thread_fork("catsem Thread", 
                                    NULL, 
                                    index, 
                                    catsem, 
                                    NULL
                                    );
                
                /*
                 * panic() on error.
                 */

                if (error) {
                 
                        panic("catsem: thread_fork failed: %s\n", 
                              strerror(error)
                              );
                }
        }
        
        /*
         * Start NMICE mousesem() threads.
         */

        for (index = 0; index < NMICE; index++) {
   
                error = thread_fork("mousesem Thread", 
                                    NULL, 
                                    index, 
                                    mousesem, 
                                    NULL
                                    );
                
                /*
                 * panic() on error.
                 */

                if (error) {
         
                        panic("mousesem: thread_fork failed: %s\n", 
                              strerror(error)
                              );
                }
        }

        return 0;
}


/*
 * End of catsem.c
 */
