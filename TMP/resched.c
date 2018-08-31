/* resched.c  -  resched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lab1.h>
//#include <math.h>
int schedclass;
unsigned long currSP;	/* REAL sp of current process */
extern int ctxsw(int, int, int, int);
//extern int SCHEDCLASS;
/*-----------------------------------------------------------------------
 * resched  --  reschedule processor to highest priority ready process
 *
 * Notes:	Upon entry, currpid gives current process id.
 *		Proctab[currpid].pstate gives correct NEXT state for
 *			current process if other than PRREADY.
 *------------------------------------------------------------------------
 */
int resched()
{
	//int SCHEDCLASS=1;	
	if(schedclass == RANDOMSCHED){
		//kprintf("resched called %d\n",currpid);
		register struct	pentry	*optr;	/* pointer to old process entry */
		register struct	pentry	*nptr;	/* pointer to new process entry */
		int denom=0;
		int temphead=rdyhead;
		//kprintf("\nold %d\n",currpid);
		denom+=proctab[currpid].pprio;
		while(q[temphead].qnext<NPROC){
			denom+=proctab[q[temphead].qnext].pprio;
		//	kprintf("%d ", q[temphead].qnext);
			temphead=q[temphead].qnext;
		
		}
		//srand(1234);
		int flag=0;
		int new_pid;
		int priority_rand=0;
		if(denom !=0){
		 priority_rand = rand() % denom;
		}
		//kprintf("prio %d %d\n",denom, priority_rand);
		int kicked_out[NPROC];
		int counter = 0;
		temphead=rdyhead;
	//	kprintf("random number %d\n", priority_rand);
		 if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
		   ((priority_rand<optr->pprio)||(q[rdyhead].qnext<0 || q[rdyhead].qnext>=NPROC))) {
		//	printf("old pid %d\n",currpid);
		//	 #ifdef  RTCLOCK
                  //      preempt = QUANTUM;              /* reset preemption counter     */
                    //    #endif

			return(OK);
		}
		else {	
			priority_rand -= optr->pprio;
			//kprintf("temphead %d temphead next %d\n", rdyhead, q[rdyhead].qnext);
			int i=0;
			for(i=q[rdyhead].qnext; i!=rdytail; i=q[i].qnext){
			//while(q[temphead].qnext<NPROC/*<=rdytail*/){
		//		kprintf("priority %d random number %d\n", proctab[q[temphead].qnext].pprio, priority_rand);
		//		printf("temphead %d ",q[temphead].qnext);
				if(proctab[i].pprio>=priority_rand){
					nptr = &proctab[i];
					new_pid = i;
					flag=1;
					break;
					
				}
				else{
					priority_rand -= proctab[i].pprio;
			//		temphead = q[temphead].qnext;
				}
				//kprintf("pid %d %d %d\n", q[temphead], temphead, rdytail);
				//temphead=q[temphead].qnext;

			}
			
			if(flag!=1){
				nptr = &proctab[new_pid=getfirst(rdyhead)];
				//new_pid=(q[rdyhead].qnext).qprev;
			}
			if(optr->pstate == PRCURR){
				optr->pstate = PRREADY;
				insert(currpid,rdyhead,optr->pprio);
			}
			currpid = new_pid;
			nptr->pstate = PRCURR;          /* mark it currently running    */
		//	#ifdef  RTCLOCK
		//	preempt = QUANTUM;              /* reset preemption counter     */
		//	#endif
		//	kprintf("\nnew pid %d\n", currpid);
			if(flag==1){
				dequeue(new_pid);
			}
		//	 optr->pstate = PRREADY;
		  //      insert(currpid,rdyhead,optr->pprio);

			ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);

		/* The OLD process returns here when resumed. */
			return OK;

			
		}
	}
	else{
		//kprintf("linux");
		register struct pentry  *optr;  /* pointer to old process entry */
                register struct pentry  *nptr;  /* pointer to new process entry */
		optr = &proctab[currpid];
		int new_pid;
		if(preempt <= 0 || currpid == NULLPROC){
			optr->counter = 0;
			optr->goodness = 0;
		//	kprintf("Null\n");
		}
		else{
			optr->goodness = optr->goodness - optr->counter + preempt;
			optr->counter = preempt;
			
		}
		int maxg=0;
		if(optr->pstate==PRCURR){
			maxg = optr->goodness;
			new_pid = currpid;
		}
		int i=0;
		//kprintf("\nqueue:");
		for(i=q[rdytail].qprev; i!=rdyhead; i=q[i].qprev){
			if(maxg < proctab[i].goodness){
				maxg = proctab[i].goodness;
				new_pid = i;
						
			}	
		//	kprintf("pid %d gdns %d",i, proctab[i].goodness);
		}
		struct pentry *temp;
		//kprintf("\nmaxg before the if %d\n", maxg);
		if(maxg==0 && (optr->pstate != PRCURR || optr->counter == 0)){
		
			for(i=0; i<NPROC; i++){
				temp = &proctab[i];
				if((temp->pstate != PRFREE) && (temp->counter == 0 || temp->counter == temp->quantum)){
					temp->quantum = temp->pprio; 
					 temp->counter = temp->quantum;
                                	temp->goodness = temp->counter + temp->pprio;
 
				}
				else if(temp->pstate != PRFREE){
					temp->quantum = (temp->counter)/2 + temp->pprio;
					 temp->counter = temp->quantum;
                                	temp->goodness = temp->counter + temp->pprio;

				}
				//temp->counter = temp->quantum;
				//temp->goodness = temp->counter + temp->pprio;
				if(temp->goodness>maxg && (temp->pstate == PRREADY || temp->pstate==PRCURR)){
					maxg = temp->goodness;
					new_pid = i;
				}
			}
			
		}

		if(maxg > 0){
			//kprintf("\nmaxg newpid %d currpid %d maxg %d counter %d priority %d optr goodness %d optr pstate %d\n", new_pid, currpid, maxg, proctab[new_pid].counter, proctab[new_pid].pprio,optr->goodness, optr->pstate);
			if(optr->goodness >= maxg && optr->pstate == PRCURR){
				preempt = optr->counter;
			//	kprintf("optr gets scheduled as it is %d\n", currpid);
				return OK;
			}
			else{
				if(optr->pstate == PRCURR){
					optr->pstate = PRREADY;
					insert(currpid,rdyhead,optr->pprio);
				}
				nptr = &proctab[new_pid];
				currpid = new_pid;
                        	nptr->pstate = PRCURR;          /* mark it currently running    */
                        	#ifdef  RTCLOCK
                        	preempt = nptr->counter;              /* reset preemption counter     */
                        	#endif
				dequeue(new_pid);
				//kprintf("new proc %d\n", currpid);
				ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
			//	kprintf("after new proc context switch\n");
				return OK;
	

			} 
		}
		else{
		
			if(optr->pstate == PRCURR){		
				optr->pstate = PRREADY;
				insert(currpid, rdyhead, optr->pprio);
				
			}
			currpid = NULLPROC;
			nptr = &proctab[NULLPROC];
			nptr->pstate = PRCURR;
			#ifdef RTCLOCK
			preempt = QUANTUM;
			#endif
			//kprintf("before dequeue %d %d\n", maxg, q[rdyhead].qnext);
			dequeue(NULLPROC);
			//kprintf("after dequeue\n");
			ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
			//kprintf("after context switch\n");
			return OK;
		}
		
		
	}	
}
