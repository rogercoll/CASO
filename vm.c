#include <mach.h>
#include <mach_error.h>
#include <mach/mig_errors.h>
#include <mach/thread_status.h>
#include <mach/processor_info.h>
#include <stdio.h>
#include <stdlib.h>
#include <hurd.h>


#define PAG_SIZE 16384
#define SLEEP_TIME 20


int main(){
	int res;
	mach_port_t child_p;
	mach_port_t parent_p = mach_task_self();
	char info[PAG_SIZE] __attribute__((aligned(PAG_SIZE)));
	vm_address_t address;

	res = task_create(parent_p,0,&child_p);
	if (res != KERN_SUCCESS){
		printf("Error creating task");
	}
	vm_allocate(child_p,&address,PAG_SIZE,0);	
	vm_write(child_p,address,(vm_offset_t)info,PAG_SIZE);
	sleep(SLEEP_TIME);
	res = task_terminate(child_p);
	if (res != KERN_SUCCESS){
		printf("Error terminating task");
	}
}
