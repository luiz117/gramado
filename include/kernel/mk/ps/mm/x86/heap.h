/*
 * File: heap.h
 *
 * Descri��o:
 *     Gerenciar o ponteiro global para o heap atual, onde ser�
 * alocada mem�ria.
 * *IMPORTANTE: Esse arquivo � importante para gerenciar a aloca��o
 * de mem�ria para um processo em user mode.
 * 
 * Hist�rico:
 *     Version: 1.0, 2016 - Created.
 */

 
//@todo: diminuir
//Contagem de heap.
#define HEAP_COUNT_MAX  8



/*
 * Heap.
 * Salvamos aqui o Global Heap Pointer atual, ele pode pertencer ao kernel,
 * ao processo, ou ao desktop ao qual o processo pertence.
 *     Obs: Acrescentado static.
 */
static unsigned long Heap;
//static unsigned long KernelHeapPointer;  //Kernel Heap pointer.
//static unsigned long UserHeapPointer;    //User Heap pointer.


/*
 * Kernel Heap support.
 */

unsigned long heapCount;            //Conta os heaps do sistema.
unsigned long kernel_heap_start;    //Start.
unsigned long kernel_heap_end;      //End.
unsigned long g_heap_pointer;       //Pointer.
unsigned long g_available_heap;     //Available.


/*
 * HeapPointer:
 *     Estrutura para manipular o heap pointer
 *     do processo atual na fun��o malloc.
 */
 /*
struct HeapPointer
{
	unsigned int kernel_heap_poiter;	
    unsigned int kernel_last_valid;
    unsigned int kernel_last_size;
	
	unsigned int desktop_heap_poiter;	
    unsigned int desktop_last_valid;
	unsigned int desktop_last_size;
	
	int desktop_id;
};
*/


/*
 * heap_d:
 *     Estrutura para heap.
 *     Cada processo tem seu heap.
 *     Cada heap tem uma lista encadeada de blocos.  
 */  

typedef struct heap_d heap_descriptor_t;

struct heap_d 
{
	object_type_t objectType;
	object_class_t objectClass;	
	
	int Id;      //*Importante.
    int Used;
    int Magic;
    //int ObjectType; ?? //tipo de objeto ao qual pertence o heap.(process, ...)	
	
	unsigned long HeapStart;    //*Importante.             
	unsigned long HeapEnd;
	unsigned long HeapPointer;            
	unsigned long AvailableHeap; 	
	
	
	// Ponteiro para a lista de blocos de um heap.
    // Lista encadeada de blocos que formam o heap.
	// A estrutura para um bloco �: mmblock_d e est� definida em mm.h
	//
    // Obs: 
	// Foram alocados v�rios blocos de mem�ria dentro de um heap. Portanto 
	// podemos colocar os ponteiros para as estruturas desses blocos dentro 
	// de uma lista encadeada e o ponteiro para a lista colocaremos aqui.
	//
	// Cada bloco desse come�a com um 'header', definido na estrutura de bloco.
	struct mmblock_d *mmblockListHead;  
	
	//se esses arrays ocuparem muito espa�o, ent�o faremos com listas encadeads.
	//struct mmblock_d *mmblockUsedBlocks;  
	//struct mmblock_d *mmblockFreeBlocks;  	
	//struct mmblock_d *mmblockAllBlocks;  
	
	
	//Test:
	// Endere�os para os blocos de um heap.
	// Uma lista de blocos que est�o sendo usados
	// Uma lista de blocos livres e j� alocados.
	// Uma lista com todos os blocos.
	//unsigned long usedBlocks[32];
	//unsigned long freeBlocks[32];
	//unsigned long Blocks[64];		
	
	
	//Um heap pertence � um desktop.
	struct desktop_d *desktop;
	
	//Um heap pertence � um processo.
	struct process_d *process;
	
	//Um heap pode pertencer � um thread.
	struct thread_d *thread;
	
	//
	// Compartilhamento de heap:
	// ========================
	//   + As threads de um mesmo processo podem compartilhar o mesmo heap
	//     pois est�o na mesma �rea de mem�ria.
	//   + @todo: O desafio � fazer os processes que est�o no mesmo desktop
	//            compartilharem o mesmo heap, porque eles deveriam estar
	//            na mesma �rea de mem�ria para isso. 
	//
	
	
    //
	// Podemos criar aqui flags de porte��o.
	//
	//int read;  //Apenas leitura.
	//int write;
	
	
	//...

	//Sequenciando heaps sei l� pra qu�. 
	//#bugbug: N�o queremos sequenciar os heaps.
	//cada processo tem seu heap.
	//struct heap_d *next;
};
heap_descriptor_t *KernelHeap;
heap_descriptor_t *UserHeap;
//heap_descriptor_t *CurrentHeap;
//heap_descriptor_t *ProcessHeap;
//heap_descriptor_t *IdleProcessHeap;
//heap_descriptor_t *current_heap;
//...


//Heap list.
//heap pool.
//obs:. heapList[0] = The Kernel Heap !!!    
unsigned long heapList[HEAP_COUNT_MAX];     


//
// Prot�tipos.
//

int init_heap (void);

void SetKernelHeap ( unsigned long HeapStart, unsigned long HeapSize);

//Pega o endere�o do in�cio do header da pr�xima aloca��o.
unsigned long get_process_heap_pointer(int pid);

//
// Alloc and Free.
//

unsigned long heapAllocateMemory (unsigned long size);

void FreeHeap (void *ptr);

//
// End.
//

