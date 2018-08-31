#include <conf.h>
#include <kernel.h>
#include <q.h>
//#include <sched.h>
#include <stdio.h>
#include <proc.h>
#include <lab1.h>

#define LOOP	50

extern int currpid;
int prA, prB, prC;
int proc_a(char), proc_b(char), proc_c(char);
int proc(char);
volatile int a_cnt = 0;
volatile int b_cnt = 0;
volatile int c_cnt = 0;
volatile int s = 0;

int main() {
	int i;
	int count = 0;
	char buf[8];

	srand(1234);

	kprintf("Please Input:\n");
	while ((i = read(CONSOLE, buf, sizeof(buf))) < 1)
		;
	buf[i] = 0;
	s = atoi(buf);
	kprintf("Get %d\n", s);
//	kprintf("Please Input:\n");
//	while ((i = read(CONSOLE, buf, sizeof(buf))) < 1)
//		;
//	buf[i] = 0;
//	s = atoi(buf);
	//s=1;
///	kprintf("Get %d\n", s);
	if (s < 2) {
		//SCHEDCLASS = 1;
		setschedclass(RANDOMSCHED);
		prA = create(proc_a, 2000, 10, "proc A", 1, 'A');
		prB = create(proc_b, 2000, 20, "proc B", 1, 'B');
		prC = create(proc_c, 2000, 30, "proc C", 1, 'C');
		resume(prA);
		resume(prB);
		resume(prC);
		int new_pid;	
		//kprintf("current %d", currpid);
		//kprintf("getlast %d\n", new_pid=getlast(rdytail));
		
	//	insert(new_pid, rdyhead,  proctab[new_pid].pprio);
//
//		kprintf("getfirst %d\n", getfirst(rdyhead));
		//dequeue(getlast(rdytail));
//		
//		kprintf("getlast %d\n", getlast(rdytail));
		//insert(new_pid, rdyhead, nptr->pprio);
//		kprintf("getfirst %d\n", getfirst(rdyhead));
//		while (count++ < LOOP) {
//			kprintf("M %d\n",currpid);
//			for (i = 0; i < 1000000; i++)
//				;
//		}
		//kprintf("before sleep\n");
		sleep(10);
		kill(prA);
		kill(prB);
		kill(prC);

		kprintf("\nTest Result: A = %d, B = %d, C = %d\n", a_cnt, b_cnt, c_cnt);
	}
	else {
		setschedclass(LINUXSCHED);
		//SCHEDCLASS=2;
		//kprintf("here %d\n",getschedclass());
		resume(prA = create(proc, 2000, 5, "proc A", 1, 'A'));
	//	kprintf("after process A\n");
		resume(prB = create(proc, 2000, 50, "proc B", 1, 'B'));
		resume(prC = create(proc, 2000, 90, "proc C", 1, 'C'));

		while (count++ < LOOP) {
			kprintf("M");
			for (i = 0; i < 1000000; i++)
				;
		}
        kprintf("\n");
	}

	return 0;
}

int proc_a(char c) {
	int i;
	kprintf("Start... %c\n", c);
	b_cnt = 0;
	c_cnt = 0;

	while (1) {
		for (i = 0; i < 10000; i++)
			;
		a_cnt++;
	//kprintf("proc a %d %d\n", a_cnt, currpid);
	}
	return 0;
}

int proc_b(char c) {
	int i;
	kprintf("Start... %c\n", c);
	a_cnt = 0;
	c_cnt = 0;
	
	while (1) {
//		kprintf("%d last\n", getlast(rdytail));
             //           dequeue(getlast(rdytail));
	// kprintf("%d last\n", getlast(rdytail));
          //              dequeue(getlast(rdytail));
// kprintf("%d last\n", getlast(rdytail));
  //                      dequeue(getlast(rdytail));


//		kprintf("")
		for (i = 0; i < 10000; i++)
			;
		b_cnt++;
		//kprintf( "proc B %d %d\n", b_cnt, currpid);
	}
	return 0;
}

int proc_c(char c) {
	int i;
	kprintf("Start... %c\n", c);
	a_cnt = 0;
	b_cnt = 0;

	while (1) {
	
		
//	kprintf("proc c %d %d\n", c_cnt, currpid);
		for (i = 0; i < 10000; i++)
			;
		c_cnt++;
//	kprintf("proc c %d %d\n", c_cnt, currpid);
	}
	return 0;
}

int proc(char c) {
	int i;
	int count = 0;

	while (count++ < LOOP) {
		kprintf("%c", c);
		for (i = 0; i < 1000000; i++)
			;
	}
	return 0;
}
