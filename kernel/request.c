/*
 * File: kernel/request.c
 *
 *      Kernel requests.
 *
 * Descri��o:
 *     Arquivo principal da central de requests do kernel.
 *     Rotinas de atendimento dos requests que o kernel recebe dele mesmo. 
 * Requests s�o solicita��es feitas em kernel mode para o kernel atender. Uma 
 * rotina em kernel mode sinaliza um tipo de request atrav�s da configura��o
 * da vari�vel kernel_request e em algum momento da troca e escalonamento de
 * threads esse request � atendido com a chamada da rotina request().
 *     As requests s�o para o funcionamento do kernel, e n�o para atender
 * solicita��es de aplicativos em ring 3. Tamb�m serve para medir algumas 
 * demandas de recursos.
 *
 * * IMPORTANTE: 'Request' ser� considerado 'sin�nimo' de Signal !
 *                Uma rotina de sinal agendar� um request par o 
 *                kernel base atender.
 *                Signal ser� considerado um servi�o e far� parte 
 *                das rotinas do sistema em executive/sys. 
 *
 * Lista de requests:
 * =================
 *
 * KERNEL_REQUEST_NONE: 
 *     Request null, sem motivo definido.
 * KERNEL_REQUEST_TIME: 
 *     Contador de vezes que a thread rodou.
 * KERNEL_REQUEST_SLEEP: 
 *     Faz a thread atual dormir. Entrando no estado waiting.
 * KERNEL_REQUEST_WAKEUP: 
 *     Acorda a thread atual.
 * KERNEL_REQUEST_ZOMBIE: 
 *     Torna zombie a thread atual.
 * KERNEL_REQUEST_NEW: 
 *     Inicia uma nova thread. A thread roda pela primeira vez.
 *     Ela estar no estado INITIALIZED.
 * KERNEL_REQUEST_NEXT: 
 *     ?? Reinicia a execu��o de uma thread.
 * KERNEL_REQUEST_TIMER_TICK: 
 *      ?? O tick do timer.
 * KERNEL_REQUEST_TIMER_LIMIT: 
 *     ?? Um limite de contagem.
 * //...
 *
 * History:
 *     2015 - Created by Fred Nora.
 *     //...
 */  
 
 
/*
    # planejando pr�ximos eventos ...

	{
	#bugbug: Situa��o em que temos que acordar uma threa por causa de evento de input.	
	+ Ocorreu um evento de input, devemos acordar a trhead que estava esperando por ele.
	#importante: Me parece que esse caso n�o deve ser tratado aqui e sim na hora do 
	tratamento da interrup��o, pois input n�o pode esperar. O tempo de resposta � importante.
	Lembrando que esse tratamento de requests � 'adiado' at� o fim da rotina de interrup��o 
	de timer.
	}
	
	{
	   ...	
	}
	

*/ 
 
 
 
#include <kernel.h>


//
// Internas,
//

void do_request_12 ( int tid );

/*
 *********************************************************
 * KiRequest: 
 *    Trata os requests. 
 *    S�o servi�os que ter�o seu atendimento atrazado at�
 * pouco antes de retornar da interrup��o do timer.
 * S�o servi�os para o ambiente ring0.    
 *
 *    (eventos, solicita��es , waits ...).
 *    Essa rotina � chamada em todas as 
 * interrup��es de timer.
 *
 *    @todo: Aqui deve ficar apenas as intefaces que 
 * chamam as rotinas de tratamento dos requests.
 */
 
int KiRequest (void){
		
    if ( kernel_request < 0 || kernel_request > KERNEL_REQUEST_MAX )
	{	
        //#debug
		
		printf ("KiRequest: %d", kernel_request );
		die ();
	}
	
	return (int) request ();
}


/*
 *******************************************************
 * request:
 *    Trata os requests do Kernel.
 *    S�o servi�os que ter�o seu atendimento atrazado at� pouco antes de 
 * retornar da interrup��o do timer.
 *    Sendo assim, um programa em user mode pode solicitar um request atrav�s
 * de uma system call, sendo a system call transformada em request, adiando
 * o atendimento.
 *   O atendimento do request pendende ser� feito com o aux�lio da estrutura
 * de request.
 *   Um request ser� atendido somente quando o timeout zerar. (defered)
 */
 
int request (void){
	
	//targets

    struct process_d *Process; 
    struct thread_d *Thread;

    int PID;
    int TID;

    unsigned long r;    //N�mero do request.
    unsigned long t;    //Tipo de thread. (sistema, peri�dica...).


	// targets.

    PID = (int) REQUEST.target_pid;
    TID = (int) REQUEST.target_tid;



	//
	// ## timeout ##
	//

    if ( REQUEST.timeout > 0 )
    {
        REQUEST.timeout--;
        return 1;
    }


	//
	// Filtro.
	//

	if ( PID < 0 || PID > PROCESS_COUNT_MAX )
	{
		Process = NULL;
	}else{
	    Process = (void *) processList[PID];
	};


	if ( TID < 0 || TID > THREAD_COUNT_MAX )
	{
	    Thread = NULL;
	}else{
	
	    Thread = (void *) threadList[TID];
	};



	//
	// # Number #
	//

    r = kernel_request;

    if (r >= KERNEL_REQUEST_MAX)
        return -1;


    switch (r) 
    {
        //0 - request sem motivo, (a vari�vel foi negligenciada).
        case KR_NONE:
            //nothing for now.       
            break;


        //1 - Tratar o tempo das threads de acordo com o tipo.  
        //#importante: De acordo com o tipo de thread.
        case KR_TIME:
            panic ("request: KR_TIME\n");
			//return -1;
            break;


        //2 - faz a current_thread dormir. 
        case KR_SLEEP:   
            do_thread_blocked ( (int) REQUEST.target_tid );
            break;


	    //3 - acorda a current_thread.
	    case KR_WAKEUP:
		    wakeup_thread ( (int) REQUEST.target_tid );
		    break;


        //
	    case KR_ZOMBIE:
            //panic ("request: KR_ZOMBIE\n");
			do_thread_zombie ( (int) REQUEST.target_tid );
		    break;


		//5 - start new task.
		//Uma nova thread passa a ser a current, para rodar pela primeira vez.
		//N�o mexer. Pois temos usado isso assim.	
		case KR_NEW:	
		    //Start a new thread. 
	        if(start_new_task_status == 1)
			{		
		        current_thread = start_new_task_id;
	        };
		    break;


        //6 - torna atual a pr�xima thread anunciada pela atual.
		case KR_NEXT:
            panic ("request: KR_NEXT\n");
			//return -1;
			break;	


		//7 - tick do timer.
		case KR_TIMER_TICK:
		    panic ("request: KR_TIMER_TICK\n");
		    //return -1;
			break;


		//8 - limite de funcionamento do kernel.
        case KR_TIMER_LIMIT:
		    panic ("request: KR_TIMER_LIMIT\n");
		    //return -1;
			break;


		// 9 - Checa se ha threads para serem inicializadas e 
		// inicializa pelo m�todo spawn.
		// obs: se spawn retornar, continua a rotina de request. sem problemas.	
		case KR_CHECK_INITIALIZED:
            check_for_standby ();
		    break;


		//#todo
		//Chama o procedimento do sistema.
        // ?? args ??	
        // o servi�o 124 aciona esse request.
		case KR_DEFERED_SYSTEMPROCEDURE:
		    //system_procedure ( REQUEST.window, REQUEST.msg, REQUEST.long1, REQUEST.long2 );
			break;


	    //exit process
		case 11:
			exit_process ( (int) REQUEST.target_pid, (int) REQUEST.long1 );
			break;


		// exit thread.
		// sairemos da thread, mas se for a thread de controle, tamb�m 
		// sairemos do processo.
        case 12:
            printf ("request: 12, exiting thread %d\n", REQUEST.target_tid);
            do_request_12 ( (int) REQUEST.target_tid );
            break;


		//make target porcess current
		//cuidado.	
		case 13:	
			current_process = REQUEST.target_pid;
			break;
			
		//make target thread current
		//cuidado.	
		case 14:	
			current_thread = REQUEST.target_tid;
			break;
			
		case 15:
		    wait_for_a_reason ( REQUEST.target_tid, (int) REQUEST.long1  );
		    break;
		
		//@todo: Tratar mais tipos.	
		//...
		
		default:
		    printf ("request: Default request {%d} \n", r );
		    break;
    };
	
//Done:
//   Essas finaliza��es aplicam para todos os requests.

    clear_request ();
	kernel_request = (unsigned long) 0;  
	
	
	// OK.
	return 0;
}


int 
create_request ( unsigned long number, 
                 int status, 
                 int timeout,
				 int target_pid,
				 int target_tid,
                 struct window_d *window, 
                 int msg, 
                 unsigned long long1, 
                 unsigned long long2 )
{
	
	if (number > KERNEL_REQUEST_MAX)
		return 1;
	
	kernel_request = number;
    REQUEST.kernel_request = number;
	
	REQUEST.status = status;
	
	if (timeout < 0 )
	{
		REQUEST.timeout = 0;
	}else{
	    REQUEST.timeout = timeout;
	}
	
	REQUEST.target_pid = target_pid;
	REQUEST.target_tid = target_tid;

	REQUEST.window = (struct window_d *) window;
	REQUEST.msg = msg;
	REQUEST.long1 = long1;
	REQUEST.long2 = long2;

	//extra.
	//rever isso depois.
	REQUEST.long3 = 0;
	REQUEST.long4 = 0;
	REQUEST.long5 = 0;
	REQUEST.long6 = 0;

	//OK
	return 0;
}



void clear_request (void){
	
    REQUEST.kernel_request = 0;
	
	REQUEST.status = 0;
	REQUEST.timeout = 0;
	
	REQUEST.target_pid = 0;
	REQUEST.target_tid = 0;
		
	REQUEST.window = NULL;
	REQUEST.msg = 0;
	REQUEST.long1 = 0;
	REQUEST.long2 = 0;
		
	REQUEST.long3 = 0;
	REQUEST.long4 = 0;
	REQUEST.long5 = 0;
	REQUEST.long6 = 0;
}


/*
 **************** 
 * do_request_12:
 *     exit thread.
 */

	// exit thread.
	// sairemos da thread, mas se for a thread de controle, tamb�m 
	// sairemos do processo.

void do_request_12 ( int tid ){

    //curent process
    struct process_d *p;
    struct thread_d *t;

    //parent process
    struct process_d *parent;
    int __ppid = 0;



	//#debug
    //printf ("do_request_12: code=%d \n", REQUEST.long1);


	//exit code.
	switch (REQUEST.long1)
	{
		//sem erros.
		case 0:
			goto do_exit;
			break;
			
		//com erro 1.
		//vamos imprimir a mensagem de erro que estiver no
		//arquivo stderr.
		//#todo talvez precise de fflush se a mensagem estiver
		//no buffer em user mode.
		case 1:
			goto do_printf_error_message;
			break;
			
		//...
		default:
			goto do_exit;
			break;
    };



//#test
do_printf_error_message:

    printf ("do_request_12: do_printf_error_message\n");
    refresh_screen ();

do_exit:


    // #todo
    // #importante
    // Nesse momento temos que que ver qual � o processo pai do processo
    //que saiu, para acordar ele, caso esteja esperando por esse.

   

	//#debug
	//Se n�o ha erros, n�o mostramos mensagem.
   // printf ("do_request_12: Exit thread, no error\n");


    //#importante
	// Isso est� certo. O que importa � exibir
	// a stream stderr do processo.

    t = (struct thread_d *) threadList[REQUEST.target_tid];
    p = (struct process_d *) processList[t->ownerPID];

    //parent process
    __ppid = p->ppid;
    parent = (struct process_d *) processList[__ppid];

    //se o processo pai est� esperando pelo processo atual,
    //ent�o acordamos o processo pai.
    if (parent->wait4pid == p->pid )
    {
        printf ("do_request_12: Acordando pai e sua thread de controle.\n");
        refresh_screen();
        parent->state = PROCESS_RUNNING;
        parent->control->state = RUNNING;
    }


	// #bugbug
	// Ainda n�o podemos usar esse tipo de coisa,
	// estamos nos preparando par usar,

	//stdout = (FILE *) p->Streams[1]; //stdout
	//stderr = (FILE *) p->Streams[2]; //stderr
	
	// #importante
	// Testando a concatena��o.
	
	//fprintf (stderr, "Exiting the thread %d ", REQUEST.target_tid );
	//fprintf (stderr, " *OK ");



    exit_thread ( (int) REQUEST.target_tid );


	//kprintf ("%s \n", stdout->_base ); 
	//kprintf ("%s \n", stderr->_base );
	
	// Done.
	
	// #bugbug:
	// Isso � realmente necess�rio ?
	// Queremos apenas exibir a mensagem no terminal.


	// Clear request structure.
	clear_request ();

	//#debug
    //kprintf ("do_request_12: done :) \n");
    //refresh_screen ();
}



//
// End.
//

