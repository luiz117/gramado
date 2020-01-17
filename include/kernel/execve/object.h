/*
 * File: object.h
 *
 * Descri��o:
 *    Header do gerenciador de objetos.
 *    Observe que esse header deve ficar em /include.
 *
 * Obs: Se cada objeto tem um Handle � porque todos objetos
 *      est�o em gavetas. O desktop � uma mesa com muitas gavetas
 *      e cada gaveta tem um puxador e dentro de cada gaveta tem um objeto.
 *
 * Obs: Teremos um vetor que � uma lista de tipos de objetos.
 *      Aplica��es em user mode solicitam um objeto atrav�s de um �ndice, 
 * que seleciona um ponteiro armazenado em algum �ndice do vetor. Cada �ndice
 * representa um tipo de objeto. Esse �ndice � chamado de descritor de objeto.
 * A estrutura tamb�m � considerada descritor. 
 *
 * Hist�rico:
 *     Vers�o 1.0, 2016 - Criado por Fred Nora.
 *     //... 
 */

 
//
// �ndices para o vetor de objetos. (provis�rio).
//
//@todo: Essa lista ser� revisada e modificada.
//       Selecioar e organizar... 
// Obs: Uma forma boa de organizar � colocar em ordem alfab�tica
//      Incluir um objeto na lista ser� f�cil.         
// @todo: Uma liste enumerada, com descri��o.
//


//
// ****   CADA �NDICE DESCREVE UM TIPO DE OBJETO   ****
//

//
// Obs: Essa lista deve estar sincronizada com a da 'enum' em gdef.h.
//

//Null.
#define OBJECT_NULL      0

#define OBJECT_ASPACE 1
#define OBJECT_DSPACE 2
#define OBJECT_BANK   3

#define OBJECT_CHANNEL       4
#define OBJECT_DIRECTORY     5       //DIRET�RIO DE ARQUIVOS.
#define OBJECT_MEMORY_HEAP   6
#define OBJECT_IOBUFFER  7
#define OBJECT_IPADDRESS 8
#define OBJECT_PROCESS_MEMORY_INFO 9
#define OBJECT_PHYSICAL_MEMORY_INFO 10
#define OBJECT_MEMORY_INFO         11
#define OBJECT_MEMORY_BLOCK   12
#define OBJECT_PAGEDIRECTORY 13
#define OBJECT_PAGETABLE 14
#define OBJECT_PAGEFRAME 15
#define OBJECT_FRAMEPOOL 16
#define OBJECT_SOCKET 17
#define OBJECT_USERINFO 18

#define OBJECT_DESKTOP    19    //Desktop. STRUCT
#define OBJECT_MENUITEM   20    //Menu ITEM
#define OBJECT_MENU 21
#define OBJECT_USESSION   22    //User Session.
#define OBJECT_WIN_PROCEDURE  23
#define OBJECT_RECTANGLE 24
#define OBJECT_RGBA 25
#define OBJECT_WINDOW     26    //Window.
#define OBJECT_WSTATION   27    //Window Station.(DESKTOP POOL)
#define OBJECT_TERMINAL 28
#define OBJECT_LINE 29
#define OBJECT_TERMINAL2 30  //TODO MUDAR

// \microkernel
#define OBJECT_PROCESS   31
#define OBJECT_SEMAPHORE   32
#define OBJECT_THREAD    33
// \microkernel.cpu
#define OBJECT_CPU 34
// \microkernel.dma
#define OBJECT_DMA 35

// \hal
#define OBJECT_PROCESSOR 36

//DEVICE
#define OBJECT_DEVICE    37 
#define OBJECT_DISK_INFO 38
#define OBJECT_PROCESSOR_BLOCK 39
#define OBJECT_HOST_INFO 40
#define OBJECT_KEYBOARD 41
#define OBJECT_MAC_ADDRESS 42
#define OBJECT_MEMORY      43  //HARDWARE, CARD.
#define OBJECT_PCIDEVICE 44
#define OBJECT_PCIDEVICEDRIVER 45
#define OBJECT_PIC   46
#define OBJECT_RTC 47
#define OBJECT_SCREEN 48
#define OBJECT_TIMER 49
#define OBJECT_VIDEO 50
#define OBJECT_VOLUME  51

//THINGS
//...

//OUTROS...
#define OBJECT_CURSOR    52
#define OBJECT_REQUEST   53 //KERNEL REQUEST.
#define OBJECT_SIGNAL    54
#define OBJECT_FILE      55
#define OBJECT_METAFILE  56
#define OBJECT_MUTEX 57
#define OBJECT_TOKEN 58
#define OBJECT_FONT 59
#define OBJECT_ICON 60
#define OBJECT_PIPE 61


#define OBJECT_GROUP     62 //Grupo de usu�rios.
#define OBJECT_USER      63 //usu�rio

#define OBJECT_COMPUTER      64  //computador (host em rede)
#define OBJECT_CPU_REGISTER  65  //REGISTRADORES DA CPU. cr0, cr1 ...

#define OBJECT_DISK          66 
#define OBJECT_DISK_PARTITION  67
#define OBJECT_GDT      68       //LIKE GDT, IDT ...
#define OBJECT_LDT 69
#define OBJECT_IDT 70
#define OBJECT_TSS 71 

#define OBJECT_PORT       72      //Porta de controlador de dispositivo
#define OBJECT_CONTROLLER  73     //Controlador de dispositivo.
#define OBJECTS_KM         74 //Kernel mode??
#define OBJECTS_UM       75  //Use mode ??
#define OBJECTS_COLOR_SCHEME 76   //ObjectTypeColorScheme
#define OBJECTS_BUTTON 77
#define OBJECTS_GRID 78
#define OBJECTS_MESSAGE 79

//...

//
// #BUGBUG:  @todo: � preciso conferir e fazer coincidir os valores em gdef.h e object.h.
//




/*

#define OBJECT_JOB     35  //Job de processos.
//#define OBJECT_PROFILE 36  
//...(30~39)

//ipc - inter process comunication.

#define OBJECT_MSG            42
#define OBJECT_EVENT          43


#define OBJECT_TOKEN  46
//...(40~49)

//Memory 

#define OBJECT_MEMORY_STACK   52

#define OBJECT_MEMORY_FRAME   54
#define OBJECT_MEMORY_PAGE    55
#define OBJECT_MEMORY_VIEW    56
#define OBJECT_MEMORY_BUFFER  57
#define OBJECT_MEMORY_CACH    58
//... (50~59)

//GUI.
#define OBJECTS_GUI            60
#define OBJECT_IMAGE           61
#define OBJECT_MENU_PROCEDURE  62
//... (60~69)

//grupo 7x
#define OBJECT_7x 70
//...(70~79)

//grupo 8x
#define OBJECT_8x 80
//...(80~89)

//Outros
#define OBJECT_LIST       90
#define OBJECT_REQUEST    91
//...(90~99)

//  *** (100~Max) ***
//Extendida (Novos objetos s�o inclu�dos).
//...
#define OBJECT_AMBIENTE  100 //boot, kernel mode, user mode.
//...(100~Max)

*/


/*
 ***********************************************
 * struct object_d:
 *     Object controle ...
 *     Estrutura para controle de objetos.
 *     Isso deve ficar no in�cio de cada estrutura 
 *     para controlar a utiliza��o do objeto por parte 
 * dos processos e threads.
 * 
 */

struct object_d 
{
    // haha !!
    // Objeto do tipo objeto.
    object_type_t objectType;
    object_class_t objectClass;


    int id; 
 
    int used;  
    int magic; 


    char *path;             // '/root/user/(name)'
    char __username[64];    // HOSTNAME_BUFFER_SIZE
    size_t userName_len;    // len 
  

    //Lista de processos que possuem o objeto.
    int pidList[32];
    int pidCount;
    int currentPID;
   
 
    //endere�os
    unsigned long obj_address;
    unsigned long obj_procedure;
   
    //
	// Status do objeto.
	//   
	 
    int token; //algum processo 'pegou' o objeto e esta usando.

    int task_id; //id da tarefa que pegou o processo.

    
	//int signaled;
	//struct process_d *waitListHead;
	
	//channel	
    //process
	//thread
	//window
	
	//continua...
};


// ??
// Repensar isso.
struct object_d objects_km[256+1];  //objetos em kernel mode.
struct object_d objects_um[256+1];  //objetos em user mode. 
struct object_d objects_gui[256+1]; //objetos gr�ficos. 





/*
 * Se o gerenciador de recursos foi inicializado.
 */
int g_object_manager_status;

//id do objeto atual
int g_current_object;

//id da lista ao qual o objeto atual pertence.
// object_km, object_um, object_gui. 
int g_current_list;




  
int init_object_manager (void);


//
// End.
//

