/*
 * Gramado Microkernel - The main file for the Microkernel module in the 
 * kernel base. 
 * (c) Copyright 2015 Fred Nora.
 *
 * File: mk.c 
 *
 * History:
 *     2015 - Created by fred Nora.
 */

 
#include <kernel.h>

//#todo
//MicroKernel Internal Data.
//#include "mkidata.h"  


//unsigned char  MicrokernelName[] = "MICROKERNEL LAYER";


//#define LOCALSTUFF1 0
//int localsstuff1;


//coloca um processo ou thread em um dos planos de execu��o
int jobcontrol ( int type, int id, int plane ){
	
	struct process_d *p;
	struct thread_d *t;
	
    switch(type)
    {
		case PROCESS:
		    goto do_process;
		    break;
			
		case THREAD:
		    goto do_thread;
			break;
			
		default:
		   goto fail;
           break;		
	};
	
	goto fail;

do_process:
    if( id <0 || id >= PROCESS_COUNT_MAX ){
		goto fail;
	}else{
		
		p = (struct process_d*) processList[id];
		if( (void*) p == NULL ){
			goto fail;
		}else{
			
			if( p->used != 1 || p->magic != 1234 ){
				goto fail;
			}
			
			if( plane == FOREGROUND ){
			    p->plane = (int) plane;	
			}else{
				//Default
				p->plane = (int) BACKGROUND;
			};
			goto done;
		};
	};

do_thread:	
    if( id <0 || id >= THREAD_COUNT_MAX ){
		goto fail;
	}else{
		
		t = (struct thread_d*) threadList[id];
		if( (void*) t == NULL ){
			goto fail;
		}else{
			
			if( t->used != 1 || t->magic != 1234 ){
				goto fail;
			}
			if( plane == FOREGROUND ){
			    t->plane = (int) plane;	
			}else{
				//Default
				t->plane = (int) BACKGROUND;
			};
			goto done;
		};
	};	
	
fail:
    return (int) -1;
done:
    return (int) 0;	
};



//se o processo ou thread estiver em background o retorno ser� -1
//indicando que ele n�o pode pegar input de teclado.
int jobcheck ( int type, int id ){
	
	struct process_d *p;
	struct thread_d *t;
	
    switch(type)
    {
		case PROCESS:
		    goto do_process;
			break;
			
		case THREAD:
		    goto do_thread;
			break;
			
		default:
		   goto fail;
           break;		
	};	
	
do_process:	
    if( id <0 || id >= PROCESS_COUNT_MAX ){
		goto fail;
	}else{

		p = (struct process_d*) processList[id];
		if( (void*) p == NULL ){
			goto fail;
		}else{
			
			if( p->used != 1 || p->magic != 1234 ){
				goto fail;
			}
			
			//Pode pegar input de teclado
			if( p->plane == FOREGROUND ){
				goto done;;
			//Est� impedido de pegar input de teclado.
			}else{
				goto done;
			};
		};	
		
	};
	
do_thread:
    if( id <0 || id >= THREAD_COUNT_MAX ){
		goto fail;
	}else{
	
		t = (struct thread_d*) threadList[id];
		if( (void*) t == NULL ){
			goto fail;
		}else{
			
			if( t->used != 1 || t->magic != 1234 ){
				goto fail;
			}
			
			//Pode pegar input de teclado
			if( t->plane == FOREGROUND ){
				goto done;;
			//Est� impedido de pegar input de teclado.
			}else{
				goto done;
			};
		};
	
	};

fail:
    return (int) -1;
done:
    return (int) 0;	
}


/*
 * microkernelTestLimit:
 * #bugbug: isso tem que ir pra for do kernel base.
 */

void microkernelTestLimit (void){
	
	/*
	
    //�ndice gen�rico.
	int i;
	struct process_d *p;
	struct thread_d *t;
	
	
	//#debug
	//printf("microkernelTestLimit:\n");  
	//refresh_screen();
	
   //
   // Criando todos os processos poss�veis.
   //
testProcess:   
    printf("Creating processes...\n");      
    i=128;	
    while( i<PROCESS_COUNT_MAX )
	{
		p = (void *) create_process( NULL, NULL, gui->screen, 0x400000, 
		                 DISPATCHER_PRIORITY_LOW, 0, "TestLimits", 
						 RING3, (unsigned long ) KERNEL_PAGEDIRECTORY);	    
		
		
		if( (void *) p == NULL )
		{
			goto testThread;
		}
		i++;	
	};
      

   //
   // Criando todas as threads poss�veis.
   //
   
testThread:	  
 
   // printf("Creating threads...\n");      	  
   // i=128;
   // while(i<THREAD_COUNT_MAX)
	//{
	//	t = (void*) create_thread( NULL, NULL, gui->screen, 0x400000, DISPATCHER_PRIORITY_LOW, 0, "TestLimits");  
	//	if((void*)t == NULL ){
	//		goto done;
	//	}	    
	//	i++;	
//	}
  
  
   //Nothing for now.
   
done:
    printf("microkernelTestLimit: Done.\n");   
	return;	
	
	*/
};



void sys_dead_thread_collector (void)
{	
    dead_thread_collector ();
}


//exit process.
//servi�o do sistema.

void sys_exit_process ( int pid, int code ){
	
	//if ( pid < 0 )
	//    return;
	
	exit_process ( pid, code );
};


//exit thread.
void sys_exit_thread (int tid){

	//if ( pid < 0 )
	//    return;
	
    exit_thread (tid);
};


/*
 *****************************************************
 * sys_create_process:
 *     Create process system interface.
 */

void *sys_create_process ( struct room_d *room,
                           struct desktop_d  *desktop,
                           struct window_d *window,
                           unsigned long init_eip, 
                           unsigned long priority, 
					       int ppid, 
					       char *name,
						   unsigned long iopl, 
						   unsigned long directory_address )
{
    //
	// @todo: Create some interface routine.
	//
	
	//@todo: Filtros para ponteiros NULL.
	
	// Create process.
	
    return (void *) create_process ( room, desktop, window, init_eip, 
	                   priority, ppid, name, iopl, 
					   (unsigned long) directory_address);
}


/*
 *********************************************************
 * sys_create_thread:
 *     Create thread system interface.
 */

void *sys_create_thread( struct room_d *room,
                         struct desktop_d  *desktop,
                         struct window_d *window,
                         unsigned long init_eip, 
                         unsigned long priority, 
					     int ppid, 
					     char *name )
{
    //
	// @todo: Create some interface routine.
	//
	
	//@todo filtros, para ponteiros NULL.
	
	
	// Create thread.
    return (void *) create_thread ( room, desktop, window, init_eip, priority, ppid, name ); 
}


/*
 *********************************************
 * sys_fork:
 *     
 */
 
int sys_fork (void){
	
	return (int) fork ();
}


//Pega o id do processo atual.
int sys_getpid (void){
	
	return (int) current_process;
}


//Pega o ID do processo pai do processo atual.
int sys_getppid (void){
	
    int pid;
	int ppid;
	
	struct process_d *p;
	
	pid = (int) current_process;
	
	if ( pid >= 0 && pid < PROCESS_COUNT_MAX )
	{
		p = (void *) processList[pid]; 		
		
		if ( (void *) p == NULL )
		{
			return (int) -1;
		}
		
		if ( p->used != 1 || p->magic != 1234 ){
		    
			return (int) -1;	
		}
		
		return (int) p->ppid;
	}

    return (int) -1;	
}


 


/*
 *****************************************************
 * init_microkernel:
 *     Initialize the kernel base microkernel.
 *     Archtecture independent inicialization ...
 *
 * In these function:
 *    + Init timer.
 *    + Init tasks. (process)
 *    + Init IPC.
 *    + Init semaphore.
 *    + Lock the scheduler.
 *
 * @todo: Mudar para microkernelInit().
 */

int init_microkernel (void){
	
    int Status = 0;
	
#ifdef KERNEL_VERBOSE	
    printf("MICROKERNEL:\n");
#endif

    // Lock task switch and lock scheduler. 
	set_task_status(LOCKED);
	scheduler_lock();
	
    // Init scheduler.
	init_scheduler();
	
	// Init processes, threads, 
	init_processes();
	init_threads();    
	
	// Init IPC and Semaphore.
	init_ipc();
    create_semaphore(); //@todo: criar fun��o.
	

	//
	// Queue.
	// 
	
	//Inicializar as filas que alimentar�o a lista do dispatcher.	
	queue = kmalloc ( sizeof( struct queue_d ) );
	
	if( (void *) queue == NULL )
	{
	    panic ("init_microkernel: queue\n");
	}else{

		//Inicializa todas as filas do microkernel.
	    init_queue (queue);
		
		// Initializing the dispatcher list.
		init_dispatcher ();
		//...
	};
	
	//...
	
	//
	// Dispatch Count Block.
	//
	
	DispatchCountBlock = (void *) kmalloc ( sizeof( struct dispatch_count_d ) );
	
	if ( (void *) DispatchCountBlock == NULL )
	{
	    panic ("init_microkernel: DispatchCountBlock\n");
	   
	} else {
		
		DispatchCountBlock->SelectIdleCount = 0;
        DispatchCountBlock->SelectInitializedCount = 0;
		DispatchCountBlock->SelectNextCount = 0;
		DispatchCountBlock->SelectCurrentCount = 0;
		DispatchCountBlock->SelectAnyCount = 0;
		DispatchCountBlock->SelectIdealCount = 0;
		DispatchCountBlock->SelectDispatcherQueueCount = 0;
		//...
	};

    //More?!

    Initialization.microkernel = 1;

#ifdef MK_VERBOSE
    printf("Done\n");
#endif

#ifdef BREAKPOINT_TARGET_AFTER_MK
    //#debug 
	//a primeira mensagem s� aparece ap�s a inicializa��o da runtime.
	//por isso n�o deu pra limpar a tela antes.
	printf(">>>debug hang: after init_microkernel");
	die();
#endif	

    return (int) Status;
}


//
// End.
//

