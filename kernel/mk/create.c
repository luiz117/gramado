/*
 * File: mk/create.c
 *
 *   cria a thread idle do processo init;
 */

 
#include <kernel.h>

 

/*
 *******************************************************************
 * KiCreateIdle:
 *     Criando Idle thread manualmente.
 *     #bugbug Essa idle fica em user mode.
 *
 * *IMPORTANTE: Na verdade esse processos e threads devem ser criados 
 * com uma fun�ao que passe os parametros via argumento, tipo 
 * create_process create_thread. Exceto a idle que pode ser feita manualmente. 
 *
 * Obs: Criar idle thread com prioridade alta. TID=0 Idle (RING 3).
 * @todo: Mudar o nome para createCreateIdleThread()
 */

void *KiCreateIdle (void){

    // using IdleThread structure.
    // Fake idle. It's a idle in ring3.

    char *ThreadName = "idlethread";    // Name.

    void *idleStack;                    // Stack pointer.


    int r;
    int q;  //msg queue.


    // Struct.

    IdleThread = (void *) kmalloc ( sizeof(struct thread_d) );	

    if ( (void *) IdleThread == NULL )
    {
        panic ("mk-create-KiCreateIdle: IdleThread\n");

    } else {

        // Ver se a estrutura do processo � v�lida.
        if ( (void *) InitProcess == NULL )
        {
            panic ("create-KiCreateIdle: InitProcess\n");

        }else{

            // ??
            // e a validade da estrutura de processo? 

            //Indica � qual processo a thread pertence.
            IdleThread->process = (void *) InitProcess;
        };
        //Continua...
    };


	// @todo: 
	// #bugbug: #importante
	// A stack da idle n�o deve ficar no heap do kernel.
	// Pois a idle est� em user mode e deve ter sua stack 
	// em user mode para ter permiss�o de acesso.
	// Mas ficar� aqui por enquanto.
	// Obs: Mais abaixo a pilha foi configurada manualmente 
	// no lugar certo.
	
	//IdleThread->Heap = ?;
	//IdleThread->HeapSize = ?;
	//IdleThread->Stack = ?;
	//IdleThread->StackSize = ?;	
	
	//Stack. @todo: A stack deve ser a que est� na TSS
	//#BugBug.
	// Estamos alocando mas n�o etamos usando.
	//# podemos usar o alocador de p�ginas e alocar uma p�gina para isso.

    // Stack.

    idleStack = (void *) kmalloc (4*1024);

    if ( (void *) idleStack == NULL )
    {
        panic ("create-KiCreateIdle: idleStack\n");
    }


	// #todo: 
	//     � poss�vel usar a fun��o create_thread nesse momento.
	//     Mas � mais veloz fazer o m�ximo em uma fun��o s�.
	//     Mas por enquanto ser�o feitas � m�o essas primeiras threads. 


	// #todo: 
	// objectType, objectClass, appMode


    IdleThread->used = 1;
    IdleThread->magic = 1234;


    //Identificadores.
	IdleThread->tid = 0;
	IdleThread->ownerPID = (int) InitProcess->pid;  



	IdleThread->name_address = (unsigned long) ThreadName;   //Funciona.

    // Obs: J� fizemos isso no in�cio da rotina.
    IdleThread->process = (void *) InitProcess;


	//
	// Page Directory
	//

	IdleThread->DirectoryPA = (unsigned long ) InitProcess->DirectoryPA;	


    // Wait reason.

    for ( r=0; r<8; r++ ){
        IdleThread->wait_reason[r] = (int) 0;
    };


    IdleThread->plane = BACKGROUND;

	// Procedimento de janela.
    //O procedimento.
	IdleThread->procedure = (unsigned long) &system_procedure;


    //
    // Single message;
    //

    //Argumentos do procedimento de janela.	
    IdleThread->window = NULL; //window;//arg1.
    IdleThread->msg   = 0;     //arg2.
    IdleThread->long1 = 0;     //arg3.
    IdleThread->long2 = 0;     //arg4.
    //IdleThread->long
    //IdleThread->long
    //...


    //
    // Message queue.
    //

    for ( q=0; q<32; q++ )
    {
        IdleThread->MsgQueue[q] = 0;
    }
    IdleThread->MsgQueueHead = 0;
    IdleThread->MsgQueueTail = 0;



	//Caracter�sticas.
    IdleThread->type = TYPE_IDLE;    //TYPE_SYSTEM.
    IdleThread->iopl = RING3;        //Idle thread � uma thread de um processo em user mode.
    IdleThread->state = INITIALIZED;   

    // Priorities.

    IdleThread->base_priority = KernelProcess->base_priority;  //b�sica.  
    //IdleThread->base_priority = PRIORITY_NORMAL;
    IdleThread->priority = IdleThread->base_priority;          //din�mica.


	IdleThread->saved = 0; 
	IdleThread->preempted = UNPREEMPTABLE; 

	//Temporizadores.
	IdleThread->step = 0;          
	IdleThread->quantum = QUANTUM_BASE;
	IdleThread->quantum_limit = QUANTUM_LIMIT;

	IdleThread->standbyCount = 0;
	IdleThread->runningCount = 0;    //Tempo rodando antes de parar.
	
	IdleThread->readyCount = 0;      //Tempo de espera para retomar a execu��o.
	
	IdleThread->initial_time_ms = get_systime_ms();
	IdleThread->total_time_ms = 0;
	
	//quantidade de tempo rodando dado em ms.
	IdleThread->runningCount_ms = 0;
	
	IdleThread->ready_limit = READY_LIMIT;
	IdleThread->waitingCount  = 0;
	IdleThread->waiting_limit = WAITING_LIMIT;
	IdleThread->blockedCount = 0;    //Tempo bloqueada.		
	IdleThread->blocked_limit = BLOCKED_LIMIT;
	
	
	IdleThread->ticks_remaining = 1000;
	
	//signal
	//Sinais para threads.
	IdleThread->signal = 0;
	IdleThread->signalMask = 0;
	
	//...
	
	//
	// Obs: Essa parte � dependente da arquitetura, deveria estar em 
	//      uma pasta, por exemplo, microkernel\i386.
	//	
	
	//if(MachineType == i386Type){...};


    // Context.
    // #todo: 
    // Isso deve ser uma estrutura de contexto.

	IdleThread->ss  = 0x23;                          //RING 3.
	IdleThread->esp = (unsigned long) GRAMADOCORE_IDLETHREAD_STACK; //0x0044FFF0;    //idleStack; (*** RING 3)
	IdleThread->eflags = 0x3200;  //0x3202, pois o bit 1 � reservado e est� sempre ligado.
	IdleThread->cs = 0x1B;                                
	IdleThread->eip = (unsigned long) GRAMADOCORE_IDLETHREAD_ENTRYPOINT; //0x00401000;     	                                               
	IdleThread->ds = 0x23; 
	IdleThread->es = 0x23; 
	IdleThread->fs = 0x23; 
	IdleThread->gs = 0x23; 
	IdleThread->eax = 0;
	IdleThread->ebx = 0;
	IdleThread->ecx = 0;
	IdleThread->edx = 0;
	IdleThread->esi = 0;
	IdleThread->edi = 0;
	IdleThread->ebp = 0;
	//...
	
	//O endere�o incial, para controle.
	IdleThread->initial_eip = (unsigned long) IdleThread->eip; 
	
	//#bugbug
	//Obs: As estruturas precisam j� estar devidamente inicializadas.
	//IdleThread->root = (struct _iobuf *) file_root;
	//IdleThread->pwd  = (struct _iobuf *) file_pwd;

	
	//CPU configuration.
	//IdleThread->cpuID = 0;              //Qual processador.
	//IdleThread->confined = 1;           //Flag, confinado ou n�o.
	//IdleThread->CurrentProcessor = 0;   //Qual processador.
	//IdleThread->NextProcessor = 0;      //Pr�ximo processador. 
	
	//Coloca na lista de estruturas.
	threadList[0] = (unsigned long) IdleThread;
	
	rootConductor = (struct thread_d *) IdleThread;
	
	// #importante
	// # current idle thread #
	//current_idle_thread = IdleThread->tid;
	
	//Pr�xima thread.
	IdleThread->Next = NULL;
	//IdleThread->Next = (void*) IdleThread;    //Op��o.
	
	// #importante
    // Contador de threads
    // Vamos atualizar o contador de threads, 
	// pois mais uma thread existe, mesmo que n�o esteja rodando ainda.
	
	//#importante 
	//nesse caso o contador foi configurado manualmente. 
	//isso acontece com as threads do gramado core.

	// #importante
	// A cria��o da thread idle vai inicializar o contador,
	// para depois s� incrementarmos.
	
    ProcessorBlock.threads_counter = (int) 1;
	
    
    // #bugbug
	// N�o h� a necessidade de colocar na fila de inicializadas
	// se logo em seguida estamos selecionando para execu��o 
	// colocando no estado standby.
    
	queue_insert_data ( queue, (unsigned long) IdleThread, QUEUE_INITIALIZED );
    
	// * MOVEMENT 1 ( Initialized ---> Standby ).
	SelectForExecution (IdleThread);    

	return (void *) IdleThread;
}


/*
 **************************************************************
 * KiCreateShell:
 *     Criando Thread Shell manualmente.
 * @todo: Mudar o nome para createCreateShellThread()
 */

void *KiCreateShell (void)
{
    return NULL; //cancelado deletar fun��o.
}


/*
 ***************************************************************
 * KiCreateTaskManager:
 *     Criando thread task manager manualmente.
 * @todo: Mudar o nome para createCreateTaskmanThread()
 */

void *KiCreateTaskManager (void)
{
    return NULL; //cancelado deletar fun��o.
}


//
// End.
//

