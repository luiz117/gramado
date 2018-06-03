/*
 * File: pc\mm\mm.h  
 *
 * Descri��o:
 *     Header para o Memory Manager. Gerenciamento de mem�ria.
 *
 * Ordem de import�ncia do gerenciamento de mem�ria:
 *     > Endere�os dos page directories.
 *     > Endere�os das pagetables. 
 *     > 
 *
 * Obs:
 *     Em executive, temos �reas de mem�rias reservadas para o sistema. 
 * Privil�gios de executivo.
 *
 * @todo: 
 *     Criar blocos livres e alocados. 
 *     1 - Linked List of free blocks.
 *     2 - Procurar por blocos livres de tamanho suficiente para a aloca��o. 
 *     3 - Memory Pool, pre aloca blocos de tamanho fixo, ex: 4096.
 *     Estrutura e manipula��o de 'page frames'.
 *     ...
 * @todo: + Mapping kernel mode memory into user address space.
 *          Dentro do espa�o dispon�vel para o usu�rio, ele poder�
 *          ler um conte�do colocado pelo kernel para ele.
 *          Isso � uma �rea de transfer�ncia. 
 *
 * Hist�rico:
 *     Vers�o 1.0, 2015 - Esse arquivo foi criado por Fred Nora.
 *     Vers�o 1.0, 2016 - Revis�o.
 *     ...
 */

/*
    Sobre 'pages' e 'page frames':
	============================
	pages: 
	    S�o blocos de mem�ria virtual. 
		?? S�o blocos de disco equivalentes aos blocos de mem�ria f�sica.??
		
    pageframes: 
	    S�o blocos na mem�ria f�sica equivalentes aos blocos do disco.
 */ 
 
 
/*
    Memory map:
    (process page directory).	
	
	" Cada processo tem seu diret�rio e uma configura��o de 4GB de mem�ria 
virtual. Essa � a mem�ria virtual para processos criados, uma padroniza��o que
ser� respeitada na hora de criar o diret�rio de p�ginas de um processo ".	
	" Note que o in�cio da mem�ria virtual est� dispon�vel para drivers ou 
bibliotecas de link din�mico ".   
	" Todo processo come�ar� em 0x00400000vir e ter� o entry point em 
0x00401000virt ". "Bibliotecas de liga��o din�mica, come�ar�o em 0x0vir
e n�o sei se essas bibliotecas tem entry point"
		
	//...
	
 @todo: 	
     " O ideal � que um processo tenha dispon�vel pra si toda a �rea baixa de 
mem�ria virtual, at� o in�cio da �rea do kernel".
     "A divis�o da mem�ria virtual do processo entre a parte que pertence ao 
processo e a parte que pertence ao kernel tem seguido o seguinte padr�o:
(1) meio � meio, onde o processo fica com os 2GB mais baixos e o kernel fica 
com os 2GB superiores, ou (2) O processo fica com os 3GB mais baixos e o
kernel fica com o 1GB superior."	 


    Mapeamento padr�o de mem�ria virtual para todos os processos do sistema:
    =======================================================================	


		     +------------------------------------+
	FFFFFFFF |             FIM                    |
		     +------------------------------------+
		     +------------------------------------+
		     +------------------------------------+ 
		     +------------------------------------+
	         |           Kernel land              | @todo:   
		     |                                    | Mudar de lugar.         
		     |                                    | Seder espa�o para LFB, que precisa ser grande.
    C0800000 |           BackBuffer               |  			 
		     +------------------------------------+	
	         |           Kernel land              |	 Mem�ria da placa de v�deo.
		     |             (4MB)                  |  @todo: 
			 |             ...                    |  Ampliar (TER O TAMANHO DA MEM�RIA DA PLACA DE V�DEO) 
	C0400000 |          FrontBuffer(LFB)          |  Obs: Tamanho da soma das �reas dos monitores, no m�nimo.
		     +------------------------------------+
			 +====================================+
             |           Kernel land              |
	         |                                    | 
             |  Stack = 0xC02F7FF0 ~ 0xC02FFFF0   | Total 32KB. 
	         |  Heap  = 0xC0100000 ~ 0xC02F7FF0   |	Total 2015 KB.
             |                                    | 			 
			 |  Kernel Entry point = 0xC0001000   | Entry point do kernel.
	         |  Kernel Base = 0xC0000000          |	In�cio da imagem do 
             |                                    |	processo kernel. 		 
	C0000000 |        ( Kernel Mode access )      |	 	   
	         +------------------------------------+
             |           User Land                |
		     +------------------------------------+
		     +------------------------------------+ 
		     +------------------------------------+
		     +------------------------------------+ 			 
	         |                                    |
             |                                    | @todo  In�cio da pilha em user mode do proesso.
	         |                                    | @todo: In�cio do heap em user mode do processo.
             |                                    | ### Por enquando cada processo tem sua pr�pria
             |                                    |     pilha e heap no fim da imagem do processo.   			 
             |                                    | 
			 |                                    |
			 | 00401000 = Process entry point     | Entrypoint da imagem.
			 | 00400000 = Process image base      | Onde se carrega uma imagem de processo.
             |                                    |  			 
             | 00000000 = Dinamic Library Base    |
			 | 00000000 = Dinamic Library image   |
			 |                                    |
             |              ...                   | @todo: ampliar heap.   			 
	00000000 |       User Mode access             |	 
			 +====================================+			 
			 

    ***
	
	

   mm - kernel process



*********************************   
 Mem�ria linear para o processo kernel:

 Kernel Base        = 0xC0000000
 Kernel Entry point = 0xC0001000
 Stack = 0xC02F7FF0 ~ 0xC02FFFF0   | Total 32KB. 
 Heap  = 0xC0100000 ~ 0xC02F7FF0   | Total 2015 KB.


*********************************
 Mem�ria f�sica para o processo kernel: 

 Kernel Base        = 0x00100000
 Kernel Entry point = 0x00101000 
 Heap               =      
 Stack              =  
 		 
	@todo: 
	    O layout da mem�ria virtual est� em fase de desenvolvimento.	   
	    Criar um layout dos endere�os f�sicos principais usados pelo sistema.
*/ 
 
 
 
 //
 // **  ENDERE�OS DOS PRINCIPAIS PAGE DIRECTORIES **
 //
 
 
// 
// Directory:
// ========== 
//     Endere�o f�sico de alguns diret�rios.
//     Esses diret�rios s�o dos utilit�rios principais.
//     N�o tem problema se esses diret�rios ficarem isolados.
//     Ser�o colocados em cr3.
//


//aqui seria um lugar segura para os diret�rios desses processos 
//do ambiente Gramado Core.
//para os outros ambientes podemos concatenar os diret�rios em 
//outro lugar de f�cil acesso.
//Obs: #bugbug No momento estamos usando apenas o diret�rio do 
//porcesso kernel para tosos os aplicativos do ambiente Gramado Core.
#define KERNEL_PAGEDIRECTORY  (0x0009C000)                        
#define IDLE_PAGEDIRECTORY    (0x0009C000 + 4096)                
#define SHELL_PAGEDIRECTORY   (0x0009C000 + 4096 + 4096)         
#define TASKMAN_PAGEDIRECTORY (0x0009C000 + 4096 + 4096 + 4096)  
//...

 


/* 
 * As configura��es de mem�ria foram feitas pelo Boot Loader.
 * (aqui os endere�os l�gico e f�sicos s�o iguais.)
 */


//Quantidade de diret�rios que podem ser criados.
//obs: cada processo cria seu pr�prio diret�rio. 
//@todo: Criar um array de estruturas alocado.
#define PAGEDIRECTORY_COUNT_MAX 1024  

//Quantidade de page tables criadas...
//cada diret�rio pode ter um monte de tabelas.
//@todo: Criar um array de estruturas alocado.
#define PAGETABLE_COUNT_MAX 1024 



//
// zones support
//

//mem�ria total em duas partes.
// 


//Zones.
// ** ESSA ESTRUTURA � A RAIZ DE TODA GER�NCIA DE MEM�RIA **
typedef struct mm_zones_d mm_zones_t;
struct mm_zones_d
{
    struct system_zone_d *system_zone;  //Essa zona � para o sistema.
    struct window_zone_d *window_zone;  //Essa zona toda � uma user session.
};
mm_zones_t *zones;


//system zone. 
typedef struct system_zone_d system_zone_t;
struct system_zone_d
{
    unsigned long systemzone_start;  //0x00000000. s�o os 32MB iniciais  
};
system_zone_t *systemzone;


//window zone.  
typedef struct window_zone_d window_zone_t;
struct window_zone_d
{
	unsigned long windowzone_start;
	//Endere�o onde come�a a user session.
	unsigned long usersession_start;	//ficar� dentro de uma �rea paginada.
    struct usession_d *usersession;    
};
window_zone_t *windowzone;




// vari�veis blobais de endere�os usados no gerenciamento de zonas de mem�ria.

#define SYSTEMZONE_START 0
#define SYSTEMZONE_END   0x0FFFFFFF 
#define WINDOWZONE_START 0x10000000
//#define WINDOWZONE_END ??

unsigned long systemzoneStart;
unsigned long systemzoneEnd;
unsigned long systemzoneSize;
unsigned long windowzoneStart;
unsigned long windowzoneEnd;    //?? Devemos levar em considera��o o calculo do tamanho da mem�ria
unsigned long windowzoneSize;
//
// tables support
//

//PDE - Page Directory Entry
typedef struct page_directory_entry_d page_directory_entry_t;
struct page_directory_entry_d
{
    unsigned long Present              :1;
    unsigned long ReadWrite            :1;
    unsigned long UserSupervisor       :1;
    unsigned long WriteThrough         :1;
    unsigned long CacheDisabled        :1;
    unsigned long Accessed             :1;
    unsigned long Reserved             :1;
    unsigned long PageSize             :1;
    unsigned long GlobalPage           :1;
    unsigned long Available1           :1;
    unsigned long Available2           :1;
    unsigned long Available3           :1;
    unsigned long PageTableBaseAddress :20;
};

//PTE - Page Table Entry
typedef struct page_table_entry_d page_table_entry_t;
struct page_table_entry_d
{
    unsigned long Present         :1;
    unsigned long ReadWrite       :1;
    unsigned long UserSupervisor  :1;
    unsigned long WriteThrough    :1;
    unsigned long CacheDisabled   :1;
    unsigned long Accessed        :1;
    unsigned long Reserved        :1;
    unsigned long PAT             :1;  // Page Table Attribute Index.
    unsigned long GlobalPage      :1;
    unsigned long Available1      :1;
    unsigned long Available2      :1;
    unsigned long Available3      :1;
    unsigned long PageBaseAddress :20;
};


/*
 * page_directory_d:
 *     Estrutura para o 'page directory' de um processo.
 *
 *     Todo processo tem seu pr�prio diret�rio de p�ginas.
 *     Assim v�rios processos podem usar o mesmo endere�o l�gico.
 *     Ex: 0x400000
 *     @todo: Um ponteiro para essa estrutura pode estar no PCB do processo.
 *            usar os processos criados por processos para testar a configura��o
 *           de page directory.
 *     Obs: Um diret�rio tem ponteiros para page tables. as page tables 
 * funcionam como pools de frames.
 */
typedef struct page_directory_d page_directory_t;
struct page_directory_d
{
	
	object_type_t objectType;
	object_class_t objectClass;
	
	//identificadores.
	int id;
	int used;
	int magic;
	
	//Qual processo � o dono do diret�rio de p�ginas.
	//talvez seja possivel reaproveitar o diret�rio.
	struct process_d *process;
	
	//Endere�o onde ficar� o diret�rio de p�ginas.
	//Obs: Para configurar um diret�rio de p�ginas talvez
	//tenha que colocar um endere�o f�sico em CR3. Lembre-se
	//que o malloc do kernel base aloca mem�ria no heap do 
	//processo kernel que fica no �ltimo giga da mem�ria virtual.
	unsigned long Address;
	
	
	// ?? struct page_directory_entry_d[1024] ??
	
	//@todo: Mais informa��es sobre o diret�rio de p�ginas.
	
	//Pr�ximo diret�rio, significa pr�ximo processo.
	//significa processos ligados em um job.
    struct page_directory_d *next;  
};
page_directory_t *pagedirectoryKernelProcess;    // KERNEL.
page_directory_t *pagedirectoryIdleProcess;      // IDLE.
page_directory_t *pagedirectoryTaskmanProcess;   // TASKMAN.
page_directory_t *pagedirectoryCurrent;          // Current.
page_directory_t *pagedirectoryShared;           // Shared. 
//...

//
// Lista de diret�rios. (Pois cada processo tem um diret�rio).
//

//Lista de estruturas para diret�rios de p�ginas.
unsigned long pagedirectoryList[PAGEDIRECTORY_COUNT_MAX]; 

//Linked list pode ser uma op��o.
//Deve estar em sintonia com o scheduler de threads.
//page_directory_t *pagedirectoryLinkedListHead;



/*
 * page_table_d.
 *     Page table structure.
 *     Obs: Uma page table funciona como um pool de frames.
 *          Tamb�m pode ser compartilhada entre processo.(cuidado).
 */
typedef struct page_table_d page_table_t;
struct page_table_d
{
	object_type_t objectType;
	object_class_t objectClass;
	
	int id;
	int used;
	int magic;
	
	//A qual diret�rio de p�ginas a page table perrtence.
	//se bem que talvez possamos usar a mesma pagetable
	//em mais de um diret�rio. ser�??
	struct page_directory_d *directory;
	
	//Cada pagetable pertence � um processo.
	struct process_d *process;
	
	//Travando uma pagetable inteira,
	//nenhuma de suas p�ginas poder�o se descarregadas
	//para o disco de swap.
	int locked;
	
	// ?? struct page_table_entry_d[1024] ??
	
    //@todo: Mais informa��es sobre a pagetable.
	struct page_table_d *next;
};
//page_table_t *pagetableCurrent;

page_table_t *pagetableCurrent;
//...

//
// Lista de pagetables.
//

unsigned long pagetableList[PAGETABLE_COUNT_MAX]; 

//Linked List talvez seja uma op��o.
//page_table_t *pagetableLinkedListHead;



 
/**
 **  **  SUPER IMPORTANTE  **
 **
 ** Super block.
 ** ESSAS VARI�VEIS GLOBAIS MARCAR�O O IN�CIO E O FIM 
 ** DA �REA DE MEM�RIA F�SICA DESTINADA AOS FRAMES DE MEM�RIA 
 ** F�SICA QUE SER�O USADOS PELO GERENCIADOR DE P�GINAS.
 **
 ** Pertencer�o ao banco FDB. (Free Data Base).
 ** 0x10000000 � um bom lugar pra come�ar os frames ,
 ** na verdade os blocos 4MB, pois cada bloco de 4MB pode ser mapeado 
 ** usando apenas uma pagetable.
 ** 
 ** mmFramesSuperBlockStart = 0x10000000
 ** mmFramesSuperBlockEnd   = 0x1FFFFFFF
 ** 
 ** Esses s�o endere�os f�sicos.
 ** Obs: Ficar�o nesse lugar caso se tenha mem�ria dispon�vel para isso.
 **
 **/
 
// Frames Super Block.
// Vari�veis globais parecem ser uma op��o melhor de estrutura
// para esse caso. 
// Obs: Temos listas de frames em algum lugar. 
// Criaremos listas aqui para o FSB, que ser� o nome do gerenciado, 
// para melhorar o controle dessa �rea.
unsigned long mmFramesSuperBlockStart;      //Endere�o onde come�a o FSB.
unsigned long mmFramesSuperBlockEnd;        //Endere�o onde termina o FSB.
unsigned long mmFramesSuperBlockSize;       //Tamanho do FSB dado em bytes.
unsigned long mmFramesSuperBlockTotal;      //Total de frames.
unsigned long mmFramesSuperBlockTotalFree;  //Total de frames livres. 
unsigned long mmFramesSuperBlockTotalUsed;  //Total de frames e uso. 
//Continua...

// ((0x1FFFFFFF - 0x10000000) / 512)  = QUANTIDADE DE FRAMES NESSA �REA.
#define FSB_FRAMES_MAX      (1*1024) //?? @todo: Determinar melhor isso
#define FSB_FREEFRAMES_MAX  (1*1024) //?? @todo: Determinar melhor isso

//## BUGBUG isso tornaria esse array bem grande.


//
// Lista com todos.
//


//Lista de ponteiros para as estruturas de todos os frames do FSB.
unsigned long fsbFrames[FSB_FRAMES_MAX]; 

//
// Lista de livres.
//

//Lista de ponteiros para as estruturas de todos os frames 'LIVRES' do FSB.
unsigned long fsbFreeFrames[FSB_FREEFRAMES_MAX];  
 
 
 
//
// memory:
//

//
// ## KERNEL STACK ##
//

//@todo: Crir estrutura de pilha.
//kernel stack. (Endere�o virtual da pilha do processo Kernel).
//Obs: O Heap e a Stack est�o dentro dos limites de 4MB de
//tamanho da imagem do kernel base.
//32kb
#define KERNEL_STACK_SIZE   0x8000                                      
#define KERNEL_STACK_START  0xC02FFFF0                                        
#define KERNEL_STACK_END    (KERNEL_STACK_START - KERNEL_STACK_SIZE)  



//Default stack que ser� usado em outra configura��o de pilha.
//#define DEFAULT_STACK_SIZE           KERNEL_STACK_SIZE
//#define DEFAULT_INITIAL_STACK_COMMIT ?

//
// ## KERNEL IMAGE ADDRESS ##
//

//Base da imagem do kernel.
#define KERNEL_IMAGE_BASE  0xC0000000    
 
 
 
 
//
// MM BLOCK.
// 

//Isso � usdo pelo heap.
#define MMBLOCK_HEADER_SIZE 64 
 


// Quantidade m�xima de pageframes.
// @todo: 
// #bugbug. 
// Isso t� errado. 
// Essa � a quantidade de pageframes de apenas uma page table. 
// Isso equiva � apenas um pagepool. en�o n�o poder� ser usado
// em outro lugar en�o no contexto de um pagepool.
//
#define PAGEFRAME_COUNT_MAX 1024    //??


// Quantidade m�xima de framepools.
// Um framepool � uma parti��o da mem�ria f�sica.
// Cada framepool � composto de 1024 pageframes.
//
// @todo: 
// #bugbug: 
// A quantidade de framepools deve ser equivalente � quantidade 
// de mem�ria f�sica dispon�vel.
// Por isso devemos criar �reas de mem�ria f�sica aloc�veis. O que 
// facilita a manuten��o de listas de framepools.
//
// Em outras palavra. Temos que concatenar parti��es de mem�ria f�sica
// pra facilitar. Ent�o chamaremos essa �rea onde est�o as parti��es de
// �rea pagin�vel. Pois existem �res de mem�ria que n�o s�o pafin�veis,
// elas simplesmente foram mapeadas para que alguma parte do sistema use.
//
// @todo: 
// Criar um ponteiro que indique o �n�cio da �rea p�gin�vel, assim como 
// acontece com o in�cio de um heap.
//
// � principio todo processo poderia ter acesso � apenas uma parti��o.
// Um framepool � garantido para um processo quando esse processo � criado
// mesmo antes de haver algum mapeamento. Na verdade um processo ter� uma lista
// de framepools.
//

//List.(1024 framepools de 4MB d� 4GB).
#define FRAMEPOOL_COUNT_MAX 1024

//#bugbug
//@todo: Aumentar ...
//Contagem de mmblock. 
#define MMBLOCK_COUNT_MAX  (2*4096)
 
 
#define PAGE_SIZE 0x1000    //4096.

//
// Size support.
//

#define KB  (1024)
#define MB	(1024 * 1024)
#define GB	(1024 * 1024 * 1024)

//
// ## MEMORY PARTITION ##
//

//Um framepool tem 4MB de tamanho.
#define MEMORY_PARTITION_SIZE (4 * MB)

//
// Obs: Criamos apenas algumas entradas no diret�rio de p�ginas
// do kernel, mas algumas dessas entradas ser�o iguais para todos 
// os processos. Ent�o os diret�rios de da p�ginas dos processos
// ser�o configurados de forma semelhante.
//

//
// Page frames da imagem do kernel:
// ================================
// Foram alocadas 1024 pageframes para a imagem do kernel.
// Isso equivale a um pageframe pool. Que � igual a 4MB. Pois
// s�o 1024 page frames de pag�nas de 4KB cada.
//

//
// + kernel area = 1024 page frames (4MB)
// + kernel image = 1024 pageframes (4MB)
// + user mode area = 1024 pageframes (4MB)
// + vga = 1024 pageframes (4MB) (Isso transpassa o real tamanho da vga)
// + lfb = (frontbuffer) 1024 pageframes (4MB) (Isso � muito pouco, pois 
//         uma placa de v�deo tem mais mem�ria que isso)
// + backbuffer = 1024 pageframes (4MB) (Isso realmente � pouco, o backbuffer 
//         deve caner uma imagem grande, que ser� dividade em v�rios monitores.)
//
//

//
// ## PAGE DIRECTORY ENTRIES ##
//

#define KERNEL_PAGE_DIRECTORY_ENTRY        0    // 4 primeiros megas em kernel mode.
#define USERMODE_PAGE_DIRECTORY_ENTRY      1    // 4 megas em user mode.
#define VGA_PAGE_DIRECTORY_ENTRY           2    // VGA

#define KERNEL_IMAGE_PAGE_DIRECTORY_ENTRY  768  // A imagem do kernel.
#define LFB_PAGE_DIRECTORY_ENTRY           769  // Frontbuffer.
#define BACKBUFFER_PAGE_DIRECTORY_ENTRY    770  // Backbuffer.
//...


/*
INVLPG

INVLPG is an instruction available since the i486 that invalidates a single page in the TLB. 
Intel notes that this instruction may be implemented differently on future processes,
 but that this alternate behavior must be explicitly enabled. INVLPG modifies no flags.

NASM example:

     invlpg [0]
Inline assembly for GCC (from Linux kernel source):

static inline void __native_flush_tlb_single(unsigned long addr)
{
   asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}

*/


/*
typedef enum {
    MM_SYSTEM_SMALL,
    MM_SYSTEM_MEDIUM,
	MM_SYSTEM_BIG,
}SYSTEM_SIZE_T;
*/


/*
 * mmblockCount:
 *     mm block support.
 *     Conta os blocos de mem�ria dentro de um heap.
 *     *dentro do heap usado pelo kernel eu acho ?? 
 */
unsigned long mmblockCount;         
 


/*
 * Kernel Stack suppport.
 */ 
unsigned long kernel_stack_end;        //va
unsigned long kernel_stack_start;      //va
unsigned long kernel_stack_start_pa;   //pa (endere�o indicado na TSS).

/*
 * process_memory_info_d:
 *
 *     Estrutura para informa��es sobre a mem�ria utilizada por um processo.
 * Obs: 
 * O gerenciamento de mem�ria � tarefa do m�dulo /sm portanto isso n�o deve 
 * ir para o /microkernel.
 *
 */
typedef struct process_memory_info_d process_memory_info_t;
struct process_memory_info_d
{
	object_type_t objectType;
	object_class_t objectClass;	
	
	struct process_d *process;
	
	//valor em KB. (quantidade de p�ginas + tamanho da p�gina.)
	
	unsigned long WorkingSet;  //Working Set.
    unsigned long Private;     //Mem�ria n�o compartilhada. 
    unsigned long Shared;	   //Mem�ria compartilhada.
	//...
	
	
	//unsigned long pageFaults; //Contagem de faltas.
	//Pico de Working Set.
	//??delta de conjunto de trabalho.
	//...
};
//Informa��es de mem�ria do processo atual.
process_memory_info_t *pmiCurrent;
//...


/*
 * physical_memory_info_d:
 *
 *     Informa��es sobre a mem�ria f�sica.
 *     O arquivo system.h deve usar isso. 
 */
typedef struct physical_memory_info_d physical_memory_info_t;
struct physical_memory_info_d
{
	object_type_t objectType;
	object_class_t objectClass;
	
	//?? d�vidas.
	unsigned long Total;     //Total de mem�ria f�sica.(RAM).
	unsigned long InCache;   //Parte do total que est� em cache.(foi paginada e est� em cache).
	unsigned long Free;      //Livre.(Existe na RAM mas n�o foi paginada??).
    //...	
};
physical_memory_info_t *pmiMemoryInfo;
//...


/*
 * memory_info_d:
 *     Informa��es sobre a mem�ria.
 *     Isso pode ser usado pela configura��o do sistema. 
 */
typedef struct memory_info_d memory_info_t;
struct memory_info_d
{
	object_type_t objectType;
	object_class_t objectClass;
	
    //Physical.
	unsigned long TotalP;
    unsigned long AvailableP;
    
	//Virtual
	unsigned long TotalV;
    unsigned long AvailableV;
};
memory_info_t *miMemoryInfo;
//...


/*
 *****************************************************************
 * mmblock_d:
 *     Estrutura para memory blocks.
 *     Temos v�rios blocos de mem�ria espalhados em lugares diferentes 
 * dentro de um heap.
 *     Essa estrutura � usada pelo kernel para controlar as �reas de mem�ria
 * alocadas din�micamente dentro do heap do kernel. Por�m poderemos
 * usar essa mesma estrutura para alocar mem�ria em outros heaps. Como o heap 
 * de um processo ou o heap de um desktop. @todo: Para isso essa estrutura 
 * poderia conter informa��es sobre qual heap estamos usando. Mas me parece 
 * que o tamanho do header deve ser fixo.
 *
 * @todo: 
 * Os blocos precisam de alguma organiza��o. 
 * Por enquanto, o total � 256 heaps de tamanhos diferentes.
 *
 * Os blocos formar�o uma lista encadeada que ser� percorrida para se 
 * encontrar um bloco vazio. (walk).
 *
 * *Importante: A mem�ria ser� organizada em bancos, que conter�o mem�ria 
 * privada e mem�ria compartilhada. Os blocos de mem�ria e os heaps 
 * precisam estar associadoas aos bancos, que conter�o informa��es sobre 
 * processos e usu�rios. @todo:
 * Obs: Os bancos est�o definidos em bank.h
 * 
 * Obs: Um heap de processo tem v�rios blocos dentro.
 *
 * *IMPORTANTE: 
 *     Talvez tenhamos algum limite para o tamanho dessa estrutura 
 * em especial. (N�o incluir nenhuma vari�vel por enquanto!).
 *****************************************************************
 */ 
//typedef struct mmblock_d mmblock_t;  //@todo: usar esse. 
typedef struct mmblock_d mmblock_descriptor_t;
struct mmblock_d 
{
	//
	// Essa estrutura � para gerenciar �reas de mem�ria alocadas dinamicamente 
	// dentro do heap do processo kernel. Alocadas em tempo de eecu��o.
	//
	
	//
	// @todo: 
	// Talvez n�o seja poss�vel mudar essa estrutura. �la � diferente.
	// Portanto n�o definiremos inada o tipo de objeto que ela � e nem a classe.
	
	//object_type_t objectType;
	//object_class_t objectClass;
	
    //Identificadores.	
	unsigned long Header;      //Endere�o onde come�a o header do heap. *Importante.
	unsigned long headerSize;  //Tamanho do header em bytes.
	unsigned long Id;          //Id do header.
	unsigned long Used;        //Flag 'usado' ou 'livre'.
	unsigned long Magic;       //Magic number. Ver se n�o est� corrompido.
	
	//Status.
	unsigned long Free;           //Se o bloco esta livre ou n�o.
	
	//Mensuradores. (sizes).	
	unsigned long requestSize;    //Tamanho, em bytes, da �rea solicitada.
	unsigned long unusedBytes;    //Quantidade de bytes n�o usados na �rea do cliente.	
	unsigned long userareaSize;   //Tamanho da �rea reservada para o cliente. 
	                              //(request size + unused bytes). 
								  
	//@todo: 
	//    Incluir quando poss�vel.
    // Lembrando que talvez o tamanho dessa estrutura seja fixo.
    // Talvez n�o mudaremos nada no tamanho dela.	
	//struct heap_d *heap;							  
	
    // User area. 
	// (Onde come�a a �rea solicitada).
	unsigned long userArea;    //In�cio da �rea alocada.
	
	// Footer.
	unsigned long Footer;    //Endere�o do in�cio do footer.
	
	// Process info.
    // (Pertence � qual processo?).	
	int processId;
	struct process_d *process;
	
	//
	// Continua ?? ... 
	// Talvez n�o pode.
	//
	
	//
	// *IMPORTANTE: 
	// Talvez temos algum limite para o tamanho dessa estrutura em especial. 
	// N�o inluir nada por enquanto.
	//
	
	//Encadeando.
	struct mmblock_d *Prev;
	struct mmblock_d *Next;
};
mmblock_descriptor_t *current_mmblock;
//mmblock_descriptor_t *SystemCache_mmblock;
//mmblock_descriptor_t *ProcessCache_mmblock;
//...

 
//Lista de blocos. 
//lista de blocos de mem�ria dentro de um heap.
//@todo: na verdade temos que usar lista encadeada. 
unsigned long mmblockList[MMBLOCK_COUNT_MAX];  
                                 
								 
								 							 
/*
typedef struct free_mmblock_d free_mmblock_t;
struct free_mmblock_d 
{
   struct mmblock_d *next;
   char garbage[4092];
}
*/



//
// ********  GERENCIAMENTO DE MEM�RIA F�SICA **************
//



//
// A mem�ria f�sica � dividida em parti��es.
// cada parti��o � composta de 1024 frames.
// Uma parti��o ser� chamada de framepool.
// Cada pegepool tem 1024 page frames.
//

/*
 ****************************************************************
 * page_frames_d:
 *     Guarda informa��es sobre um 'page frame' na mem�ria f�sica.
 *     @todo: Incluir todas as informa��es necess�rias.
 */
typedef struct page_frame_d page_frame_t;
struct page_frame_d
{
	object_type_t objectType;
	object_class_t objectClass;
	
	int id;
	int used;
	int magic;
	int locked;             //N�o pode ser descarregado para o disco.
	
	int free;    //se � um frame de mem�ria f�sica livre para uso.
	
	//...
	
    //unsigned long address;  //Endere�o f�sico da page frame.	
	
	//...
	
	//@todo
	//'owner' A quem pertence a page frame.???user, group, process.
	//struct process_d *process;
	//...
	
	//� qual pagetable pertence esse page frame.
	//struct page_table_d *pagetable;
	
	//aspace associado � esse pageframe.
	//struct aspace_d *aspace;
	
	//area de disco associada � esse pageframe.
	//struct dspace_d *dspace;
	
	
	//navega��o
    struct page_frame_d *next;	
};
page_frame_t *pageframeCurrent;
//...

//Lista de page frames.
unsigned long pageframeList[PAGEFRAME_COUNT_MAX];
//page_frames_t *pageframeListHead;

//
// *importante
//

//pool de mem�ria pagin�vel usado para aloca��o.
//aqui ficam os ponteiros para estrutura do tipo pageframe
unsigned long pageframeAllocList[PAGEFRAME_COUNT_MAX];


/*
 * frame_pool_d:
 *     Estrutura para uma parti��o da mem�ria f�sica.
 *     Uma parti��o da mem�ria f�sica � chamada de framepool.
 *     Cada framepool tem 1024 frames.
 *     @todo: Poderia ser framepool_d ??
 */
typedef struct frame_pool_d frame_pool_t;
struct frame_pool_d
{
	object_type_t objectType;
	object_class_t objectClass;
	
	int id;
	int used;
	int magic;
	int locked;
	
	//Endere�o do in�cio do framepool.
	unsigned long address; 
	
	//Qual processo � o dono desse framepool.
	struct process_d *process;
	
	struct frame_pool_d *next;
};

//
// Cada framepool abaixo � o framepool inicial de uma regi�o.
// Uma regis�o pode ter v�rios framepools.
//

//kernel space.
frame_pool_t *framepoolKernelSpace;            //0x00000000  Kernel Space. In�cio do kernel space.

//user space
frame_pool_t *framepoolSmallSystemUserSpace;   //0x00400000  Para um sistema pequeno o kernel space tem 4MB.
frame_pool_t *framepoolMediumSystemUserSpace;  // 
frame_pool_t *framepoolLargeSystemUserSpace;   //0x40000000  Para um sistema grande o kernel space tem um giga. 
//...

//Cada front buffer � uma placa de v�deo.
frame_pool_t *framepoolFrontBuffer1;   //In�cio do linear frame buffer 1.
frame_pool_t *framepoolFrontBuffer2;   //In�cio do linear frame buffer 2.
frame_pool_t *framepoolFrontBuffer3;   //In�cio do linear frame buffer 3.
frame_pool_t *framepoolFrontBuffer4;   //In�cio do linear frame buffer 4.
//...

//Backbuffer
frame_pool_t *framepoolBackBuffer1;   //In�cio do backbuffer.
frame_pool_t *framepoolBackBuffer2;   //In�cio do backbuffer.
frame_pool_t *framepoolBackBuffer3;   //In�cio do backbuffer.
frame_pool_t *framepoolBackBuffer4;   //In�cio do backbuffer.
//...


//�rea onde poderemos alocar frames para os processos usarem...
//Esse � o framepool inicial de usa �re grande de mem�ria.
frame_pool_t *framepoolPageableSpace;   


//Current.
frame_pool_t *framepoolCurrent;
//...

//frame_pool_t *framepoolKernelPagedPool;
//frame_pool_t *framepoolKernelNonPagedPool;
//frame_pool_t *framepoolUserPagedPool;
//frame_pool_t *framepoolUserNonPagedPool;

//
//   **** PAGEABLE AREA ****
//



unsigned long gPagedPollStart;
unsigned long gPagedPollEnd;


//Obs: ISSO � UM TESTE.
//N�mero m�ximo de framepools na �rea de aloca�ao de frames.
//Obs: Isso � uma esp�cie de cache. Por�m cache � coisa de disco.
//
// ( 1 * 4MB) =   4 MB.
// ( 2 * 4MB) =   8 MB. (Usando esse num sistema de 32mb de ram).
// ( 4 * 4MB) =  16 MB.
// ( 8 * 4MB) =  32 MB.
// (16 * 4MB) =  64 MB.
// (32 * 4MB) = 128 MB.
//

//#define PAGEABLE_FRAMEPOOL_MAX 1
#define PAGEABLE_FRAMEPOOL_MAX 2          //usando 8 MB para frames sob demanda.
//#define PAGEABLE_FRAMEPOOL_MAX 4
//#define PAGEABLE_FRAMEPOOL_MAX 8
//#define PAGEABLE_FRAMEPOOL_MAX 16
//#define PAGEABLE_FRAMEPOOL_MAX 32



//
// N�mero m�ximo de �ndices de framepool que ser�o usados nessa �rea de 
// aloca��o de frames.
// *** Uma certa quantidade de framepools ser�o usados
// para aloca��o de frames para os processos. Durante
// a aloca��o sobre demanda os frames usados vir�o dessa �rea de mem�ria.
//
//
//

int g_pageable_framepool_index_max;


//List.(1024 framepools de 4MB d� 4GB).
unsigned long framepoolList[FRAMEPOOL_COUNT_MAX];


//Lista de framepools livres.
//int framepoolFreeList[FRAMEPOOL_COUNT_MAX]
//frame_pool_t *framepoolFreeListHead;



//frame pool atual.
int g_current_framepool;


//o indice do framepool da user space para qualquer tamanho de mem�ria.
int g_user_space_framepool_index;


//O m�ximo de framepools poss�veis dado o tamanho da mem�ria f�sica.
unsigned long g_framepool_max;


//Tamanho m�ximo da mem�ria f�sica.
unsigned long g_total_physical_memory;


//Contabilidade. kernel(talvez deva fazer uma estrututra) MS.
//Mem�ria usada pelo kernel.
//+Paginada. paged
//+n�o paginada. nonpaged
//+fisica paginada
//+virtual paginada 
//+paginada limite
//+na� pagianda limite.
//
unsigned long g_kernel_paged_memory;
unsigned long g_kernel_nonpaged_memory;

// **** GERENCIA DE MEM�RIA F�SICA. ****
//MS - Windows 
// +Resevada para hardware.
// +Em uso.
// +Modificada. 
// +Em espera.
// +Livre.
// ?? Aqui n�o conta a mem�ria de v�deo, somente o tamanho da mem�ria ram f�sica.
// A mem�ria de v�deo est� normalmente no topo da mem�ria f�sica real. Endere�o 
// al�m do tamanho da mem�ria ram.
//


// **** GERENCIA DE MEM�RIA F�SICA. ****
//MS - Windows 
// + Total.
// + Em cache.
// + Dispon�vel.
// + Livre.
//



//tipo de sistema baseado no tamanho da mem�ria.
typedef enum {
	stNull,
    stSmallSystem,
    stMediumSystem,
    stLargeSystem,
}mm_system_type_t;


//salva o tipo de sistema baseado no tamanho da mem�ria.
int g_mm_system_type;

unsigned long memorysizeBaseMemory;
unsigned long memorysizeOtherMemory;
unsigned long memorysizeExtendedMemory;
unsigned long memorysizeTotal;

//
// Prot�tipos.
//
void memoryShowMemoryInfo();

//
// Init support.
//

int init_mm(); 
int init_stack();
int SetUpPaging();    //Configura pagina��o.
void SetCR3(unsigned long address);



//
// Directory and Page Tables.
//

void *CreatePageDirectory(unsigned long directory_address);

void *CreatePageTable( unsigned long directory_address, 
                       int offset, 
					   unsigned long pagetable_address );


					   
					   
					   
//
// page frame support.
//



int pfEmpty(struct page_frame_d *pf);
void freePageframe(struct page_frame_d *pf);
void notfreePageframe(struct page_frame_d *pf);	
int firstSlotForAList(int size);	
void initializeFramesAlloc();
void *newPageFrame();  //aloca apenas uma p�gina.	
void *allocPageFrames(int size);
void *newPage();   //aloca uma p�gina e retorna seu endere�o virtual inicial
void testingFrameAlloc();   //@todo: Rotina de teste. deletar.

//
// Debug support.
//

void show_memory_structs();


//
// garbage collection support
//

int gc();
int gcGRAMADO();
int gcEXECUTIVE();
int gcMICROKERNEL();
int gcHAL();




//
// End.
//


