/*
 * File: gdef.h
*
* Descri��o:
*     Defini��es globais. 
*     Ser� utilizado por todos os m�dulos. Deve ficar no in�cio.
*     Quando uma defini��o deve ser acessada por todos os m�dulos
*     do kernel base ela deve ficar aqui.
*
* @todo: FAZER O M�XIMO DE COISAS SISTEMICAMENTE N�O-IMPORTANTES PRIMEIRO.
         que n�o comprometam o sistema. como gui.
*/

/*
 * Sobre a GUI:
 * ============
 *     Um grid flutuante de janelas quadradas, 4 janelas.
 *     O message box pode ser um ret�ngulo abaixo do grid.
 *     Uma janela de terminal virtual igual � um celular. 
 * Desse modo, enquanto essas 6 janelas n�o s�o flutuantes, d� pra
 * ir usando elas desenha das no backbuffer mesmo.
 *
 *
 * divis�o da tela:   
 * quatro quadrantes de 10.
 * divide por vinte e multiplica pela quantidade desejada.
 *         (multiplicador*(total/20)) 
 */

/*
 Sobre o banco de dados:
 =======================
 
Tipos de contas:
    aspace e dspace, pessoais e conjuntas.
	
aspace:
    Um Address Space � uma conta de banco de dados em mem�ria. Uma conta 
pode ser do tipo conjunta(Shared) ou  pessoal(Personal). 

dspace:
    Um Disk Space � uma conta de banco de dados em disco. Uma conta pode 
ser do tipo conjunta(Shared) ou  pessoal(Personal). 

Donos das contas:
    Os processos s�o os clientes dos bancos de dados e s�o os donos das 
contas de bancos de dados.

Tipos de bancos:
    Kernel Data Base, Global Data Base e Local Data Base.

KDB:	
    Banco de dados do tipo kernel. � o banco de maior privil�gio, poucos
processos poder�o usar esse banco e as contas presentes nele.

GDB:
    Bancos de dados do tipo global. � o banco das contas conjuntas, para
que os processos possam compartilhar as contas e os dados que elas acessam.

LDB:
    Bancos de dados do tipo pessoal. � o banco das contas pessoais. Um 
processo pode ser dono de uma conta dessas para proteger seus dados
e revogar qualquer tipo de acesso e somente ele ter� acesso aos dados
dessa conta.
	
*/


//#define BIOS_FONT8X8 0x000FFA6E

//using gui flags.
#define GUI_ON   1
#define GUI_OFF  0


//layers
#define KERNEL      0
#define GRAMADO     1   //1 kgws
#define EXECUTIVE   2   //2
#define MICROKERNEL 3   //3
#define HAL         4   //4


//IOPL constants.
#define KernelMode 0
#define UserMode   3
#define RING0 0
#define RING1 1
#define RING2 2
#define RING3 3



//Kernel status constants.
#define  KERNEL_NULL          0
#define  KERNEL_INITIALIZED   1
#define  KERNEL_ABORTED       2
#define  KERNEL_INICIALIZADO  KERNEL_INITIALIZED
#define  KERNEL_ABORTADO      KERNEL_ABORTED


//#define LOBYTE(w) ((char)(((unsigned long )(w)) & 0xff))
//#define HIBYTE(w) ((char)((((unsigned long)(w)) >> 8) & 0xff))




// salvando o �ltimo endere�o v�lido de mem�ria ram.
//usado em head.asm
unsigned long blSavedLastValidAddress;




/*
 * Os Type:
 *     O sistema operacional pode ser usado como servidor.
 * Radando em uma m�quina real e acessado por um terminal
 * via rede LAN, ou porta serial. TTY.
 *     Qualquer sistema operacional em uma m�quina cliente pode
 * acessar o sistema operacional na maquina servidora. Pra isso
 * tem que ter um programa na m�quina cliente que envie comandos 
 * para a m�quina servidora atrav�s da placa de rede ou da porta 
 * serial.
 *     O sistema pode atuar como cliente.
 */
typedef enum {
	OsTypeClient,
	OsTypeServer,
	OsTypeHeadlessServer,  //Headless Server. ??
    //...	
}os_type_t;

//
// Enumerando os tipos de objetos.
// Obs: Deve ser o mesmo que as defini��es acima.
// As estruturas indicam o tipo de objeto que se pode criar.
// o primeiro �tem de uma estrutura deve ser a indica��o do tipo
// de objeto que ela representa.
// @todo: A ORDEM AQUI SEGUIR� A ORDEM DAS CLASSES DO SISTEMA.
//        COME�ANDO DA CLASSE RAM.
//
// Precisa ser uma defini��o global. pois todas estruturas v�o usar.
//
// Obs: Essa lista deve estar sincronizada com alista de fefini��es em object.h
//

//
//Obs: Esses poderiam ser os principais, e imut�veis
// deichando os vindouros como subsequentes � esses.
//

//#importante: Isso faz parte do m�dulo de seguran�a
//e deve estar associado ao sistema de arquivos ...
//Pois estamos lidando com concess�es de permiss�es 
//de acessoa objetos ... que pode ser 'arquivos' ou 
// dispositivos. 
//talvez isso devesse ir para object.h
typedef enum {
	
	// **** EXECUTIVE  ****
	
	//ram
	//Nothing for now.	
    ObjectTypeNull,               //0 Null. (Free)
	ObjectTypeASpace,             //1 Address Space. (memory address)
	ObjectTypeDSpace,             //2 Disk Space.
	ObjectTypeBank,               //3 Bank. (banco de dados).
	//sm
	ObjectTypeChannel,            //4 executive sm.
	ObjectTypeDirectory,          //5 diret�rio de arquivos. executive sm.
	ObjectTypeHeap,               //6 heap executive sm
	ObjectTypeIoBuffer,           //7 i/o buffer executive sm
	ObjectTypeIpAddress,          //8 ip address executivesm.
	ObjectTypeProcessMemoryInfo,  //9 process memory info, executive sm.
	ObjectTypePhysicalMemoryInfo, //10 physical memory info, executive sm.
	ObjectTypeMemoryInfo,         //11 memory info, executive sm.
	ObjectTypeMemoryBlock,        //12 ????? @todo: Ver se esse pode ser Criado sem prejudicar a aloca��o din�mica. executive sm mm.	
	ObjectTypePageDirectory,      //13 page directory. executive sm, mm.	
	ObjectTypePageTable,          //14 pagepool , page table, executive ,sm,mm.	
	ObjectTypePageFrame,          //15 page frame ,executive ,sm,mm.
	ObjectTypeFramePool,          //16
	ObjectTypeSocket,             //17 sockets, executive, sm.
    ObjectTypeUserInfo,           //18 userinfo, executive, sm.
	//uigm 
	ObjectTypeDesktop,     //19 desktop, executive,uigm.	
	ObjectTypeMenuItem,        //20 menuitem, executive uigm.
	ObjectTypeMenu,        //21 menu, executive uigm.	
	ObjectTypeUserSession, //22 User session, executive, uigm.	
	ObjectTypeWindowProcedure,  //23 window procedure, executive, uigm.	
	ObjectTypeRectangle,        //24 rectangle, executive, uigm. 	
	ObjectTypeRGBA,             //25 rgba, executive, uigm. 
	ObjectTypeWindow,           //26 window, executive ,uigm.
	ObjectTypeWindowStation,  //27 desktop pool. window station, executive, uigm.
	//uitm.
	ObjectTypeTerminal,        //28 console, executive, uitm.
    ObjectTypeLine,           //29 console line, executive, uitm.
	ObjectTypeTerminal2,       //30 Terminal, executive, uitm. //TODO MUDAR
	
    // **** MICROKERNEL  ****
	
	//microkernel
	ObjectTypeProcess,  //31 Process, microkernel.
	ObjectTypeSemaphore,  //32 semaphore, microkernel.	
	ObjectTypeThread,     //33 thread, microkernel.
	
	//io 
	//@todo: isso dever� ser criado aqui.
	ObjectTypeCpu,  //34
	ObjectTypeDma,  //35
	
	// **** HAL  ****
	
	ObjectTypeProcessor,  //36 processor, hal (talvez isso devesse ir para io de microkernel.)
	
	//device(unblocked e blocked).
	ObjectTypeDevice,      //37 device, hal
	ObjectTypeDiskInfo,    //38 disk info, hal.
	ObjectTypeProcessorBlock, //39 processor block hal...(pode ir para io,microkernel)
    ObjectTypeHostInfo,  //40 HostInfo, hal. 
	ObjectTypeKeyboard,   //41 keyboard, hal.
	ObjectTypeMacAddress,  //42 MAC Address, hal.
	ObjectTypeMemory,      //43 memory card info, hal.
	ObjectTypePciDevice,   //44 PCI device info, hal.
	ObjectTypePciDeviceDriver,   //45 PCI device driver info, hal.
	ObjectTypePIC, //46 pic controller, hal.
	ObjectTypeRTC, //47 rtc controller, hal.
	ObjectTypeScreen, //48 Screen, hal.
	ObjectTypeTimer,    //49 ?? timer ,hal.	
	ObjectTypeVideo,    //50 video ,hal.
	ObjectTypeVolume,    //51 disk volume ,hal.
	
	//things
	//@todo:
	
	// **** OUTROS  ****
	ObjectTypeCursor,    //52 /include.
	ObjectTypeRequest,   //53 request de kernel /include.
	ObjectTypeSignal,    //54
	ObjectTypeFile,      //55 file, /include. stdio.
	ObjectTypeMetafile, //56 
	ObjectTypeMutex,    //57
	ObjectTypeToken,    //58 Token de acesso � objetos. (access token)
	ObjectTypeFont,    //59
	ObjectTypeIcon,     //60
	ObjectTypePipe,     //61
	
	ObjectTypeGroup,     //62
	ObjectTypeUser,      //63
	
	ObjectTypeComputer,      //64
	ObjectTypeCpuRegister,   //65
	
	
	ObjectTypeDisk,   //66
	ObjectTypeDiskPartition,   //67 //info struct
	ObjectTypeGDT,   //68
	ObjectTypeLDT,   //69
	ObjectTypeIDT,   //70
	ObjectTypeTSS,   //71
	
	ObjectTypePort,  //72 (i/o port)
	ObjectTypeController, //73
	ObjectTypeKM, //74
	ObjectTypeUM, //75
	ObjectTypeColorScheme,  //76
	ObjectTypeButton,       //77
	ObjectTypeGrid,       //78
	ObjectTypeMessage,    //79
	//...
	
	//@todo: precisamos criar um tipo gen�rico
	//       para valida��o de objetos n�o especificados 
	// ou at� nulos, talvez. :)
	//ObjectTypeGeneric, //80
	
}object_type_t;

//
// Enumerando classes de objetos. (categorias)
//

//talvez isso devesse ir para object.h
typedef enum {
	ObjectClassKernelObjects,  // Kernel Objects.
	ObjectClassUserObjects,    // User Objects.
	ObjectClassGuiObjects,     // Graphical User Interface Objects.
	ObjectClassCliObjects,     // Command Line Interface Objects.
    //...	
}object_class_t;



// Window server and window manager.
// Somente um processo pode ser o ws.
// Somente um processo pode ser o wm.

//int g_ws_pid;
//int g_wm_pid;

//
// buffer support
//

// ??@todo: Talvez essas globais deva ir para o microkernel/pc

//frontbuffer lfb
unsigned long g_frontbuffer_va;         
unsigned long g_frontbuffer_pa;         

//backbuffer
unsigned long g_backbuffer_va;
unsigned long g_backbuffer_pa;

//pagedpool virtual address
unsigned long g_pagedpool_va;

// endere�o virtual do pool de heaps.
// os heaps nessa �rea ser�o dados para os processos.
unsigned long g_heappool_va;
int g_heap_count;
int g_heap_count_max;
unsigned long g_heap_size;
#define G_DEFAULT_PROCESSHEAP_SIZE (1024*128)
#define G_DEFAULT_PROCESSHEAP_COUNTMAX 16     //16 PROCESSOS #BUGBUG



#define  G_DEFAULT_EXTRAHEAP_SIZE  0x400000;  //4MB

unsigned long g_extraheap1_va;
unsigned long g_extraheap1_size;
unsigned long g_extraheap2_va;
unsigned long g_extraheap2_size;
unsigned long g_extraheap3_va;
unsigned long g_extraheap3_size;






int gNextKeyboardMessage;

//
// End.
//

