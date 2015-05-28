/* 
 * stoplight.c
 *
 * 31-1-2003 : GWA : Stub functions created for CS161 Asst1.
 *
 * NB: You can use any synchronization primitives available to solve
 * the stoplight problem in this file.
 */


#include <types.h>
#include <lib.h>
#include <test.h>
#include <thread.h>
#include <synch.h>

/*
 * Number of cars created.
 */
 
#define MAX_REGIONS 4
#define NCARS 20




/*Global Variables*/
int intersection_cars = 0;//number of cars in the intersection
lockz* intersectionlock = NULL;
cvz* intersectioncv = NULL;

typedef struct
{
	int origin;
	int destination;
}car;

car cars[NCARS];


static const char *directions[] = { "E", "N", "W", "S" };

static const char *msgs[] = {
        "approaching:",
        "region1:    ",
        "region2:    ",
        "region3:    ",
        "leaving:    "
};

/* use these constants for the first parameter of message */
enum { APPROACHING, REGION1, REGION2, REGION3, LEAVING };

static void
message(int msg_nr, int carnumber, int cardirection, int destdirection)
{
        kprintf("%s car = %2d, direction = %s, destination = %s\n",
                msgs[msg_nr], carnumber,
                directions[cardirection], directions[destdirection]);
}
 

#define NE 0
#define NW 1
#define SW 2
#define SE 3
int count = 0;
cvz* region_cvs[2][4];
lockz* region_locks[2][4];
int region_acquired[2][4] = {{0,0,0,0},{0,0,0,0}};

void printblockvalues(){
    kprintf("array[0] = %d, array[1] = %d, array[2] = %d, array[3] = %d\n", region_acquired[0][0] , region_acquired[0][1],region_acquired[0][2], region_acquired[0][3]);
}
void sanitycheck(){
    int i;
    for (i=0; i <4; i++)
        assert(region_acquired[0][i] == 1 || region_acquired[0][i]==0); 
}

int advance (int region,int carnumber,int cardirection,int destination){
    assert(region < 4);
    lock_acquire(region_locks[0][region]);
    while(region_acquired[0][region]){
        cv_wait(region_cvs[0][region],region_locks[0][region]);
    }
    region_acquired[0][region]++; 
    printblockvalues();
    if (cardirection==region)
	message(REGION1, carnumber, cardirection, destination);
    lock_release(region_locks[0][region]);
    return region;
}

void leave(int region){
    assert(region < 4);
    lock_acquire(region_locks[0][region]);
    region_acquired[0][region]-- ;
    printblockvalues();
    cv_signal(region_cvs[0][region],region_locks[0][region]);
    lock_release(region_locks[0][region]);
} 

int getdestination(int destination_steps, int cardirection){
    return (cardirection + destination_steps) % 4;
}

message2(int i, int carnumber, int cardirection, int destination){
    switch(i){
        case 1:
            message(REGION2, carnumber, cardirection, destination);
	    break;
	case 2:
	    message(REGION3, carnumber, cardirection, destination);
	    break;
    }
}

int intersectioncars = 0;
lockz* lock2;
cvz* intersectioncv;

cvz* yetanothercv;
lockz* yetanotherlock;

static
void
approachintersection(void * unusedpointer,
                     unsigned long carnumber)
{        
        /*
         * Avoid unused variable and function warnings.
         */

        (void) unusedpointer;

	/*
         * cardirection is set randomly.
         */
         
        int cardirection = random() % 4;
       	int destination_steps = random()%3 + 1;
       	int i =1;
	int destination = getdestination(destination_steps, cardirection);


	lock_acquire(lock2);
	while(intersectioncars>2){
	    cv_wait(intersectioncv,lock2);
	}
	intersectioncars++;
	lock_release(lock2); 

	lock_acquire(region_locks[1][cardirection]);
        while(region_acquired[1][cardirection]){
	    cv_wait(region_cvs[1][cardirection], region_locks[1][cardirection]); 
	} 
	region_acquired[1][cardirection]++;
	message(APPROACHING, carnumber, cardirection, destination);	
	lock_release(region_locks[1][cardirection]);

        advance(cardirection,carnumber, cardirection, destination);

	lock_acquire(region_locks[1][cardirection]);
        region_acquired[1][cardirection]--;
	cv_signal(region_cvs[1][cardirection], region_locks[1][cardirection]); 
	lock_release(region_locks[1][cardirection]);

	for (i = 1; i < destination_steps; i++){
	    advance((cardirection + i) %4, carnumber,cardirection, destination);
	    message2(i, carnumber,cardirection, destination);
	    leave((cardirection + i -1)%4);
	}
        leave((cardirection + destination_steps - 1) %4);
	message(LEAVING, carnumber, cardirection, destination);
	lock_acquire(lock2);
	cv_signal(intersectioncv,lock2);
	intersectioncars--;
	lock_release(lock2);
        
//------------------------------------------------------------------------------------------------fine till here-----------------------------------------------------------------

    
}


/*
 * createcars()
 *
 * Arguments:
 *      int nargs: unused.
 *      char ** args: unused.
 *
 * Returns:
 *      0 on success.
 *
 * Notes:
 *      Driver code to start up the approachintersection() threads.  You are
 *      free to modiy this code as necessary for your solution.
 */

int
createcars(int nargs,
           char ** args)
{
        int index, error;

        /*
         * Avoid unused variable warnings.
         */

        (void) nargs;
        (void) args;
	yetanothercv=cv_create("");
	yetanotherlock=lock_create("");
	//making the locks
	intersectioncv = cv_create("");
	intersectionlock = lock_create("");	
	intersectioncv = cv_create("");
	lock2= lock_create("");
	int count2,count=0;
	for (count2=0; count2<2;count2++)
	for (count = 0 ; count<4; count++){
	    region_cvs[count2][count] = cv_create("");
	    region_locks[count2][count] = lock_create("");
	}	
	
	kprintf("qwer\n");


        /*
         * Start NCARS approachintersection() threads.
         */

        for (index = 0; index < NCARS; index++) {

                error = thread_fork("approachintersection thread",
                                    NULL,
                                    index,
                                    approachintersection,
                                    NULL
                                    );

                /*
                 * panic() on error.
                 */

                if (error) {
                        
                        panic("approachintersection: thread_fork failed: %s\n",
                              strerror(error)
                              );
                }
        }

        return 0;
}
