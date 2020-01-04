/*
 * Gramado Boot Loader - The main file for the Boot Loader.
 * (c) Copyright 2015-2016 Fred Nora.
 *
 * File: main.c 
 *
 * Descri��o:
 *    Arquivo principal do Boot Loader do sistema de 32bit para desktops.
 *    M0 - M�dulos em ring0.
 *
 * Atribui��es:
 *    + Carregar o Kernel.

 *    + Carregar os metafiles de inicializa��o.
 *    + Carregar os aplicativos.
 *    + Fazer a configura��o inicial da pagina��o.
 *    + Passar o comando para o Kernel.
 *
 * Obs:
 * O Kernel ser� carregado em 0x00100000, entry point em 0x00101000.
 * Esses s�o endere�os f�sicos. Os endere�os l�gicos do Kernel s�o 0xC0000000 
 * para base e 0xC0001000 para entry point.
 *
 * Sobre o Boot Loader:
 * O BL foi carregado em 0x00020000 com entry point em 0x00021000.
 * As fun��es normais do Boot Loader come�am com 'Bl'.
 * As fun��es que utilizem recursos importados do Boot Manager come�am 
 * com 'Ble', que significa algo como Boot Loader Extention.
 *
 * In this file:
 *     + BlMain        - Entrada da parte em C do boot loader. 
 *     + BlLoadKernel  - Carrega o kernel.
 *     + BlSetupPaging - Configura a pagina��o.
 *     ...
 *
 * History:
 *     2015 - Created by Fred Nora.
 *     2016 - Revision.
 *    ... 
 */
 
 
#include <bootloader.h>


// Prot�tipos de fun��es internas.
unsigned long init_testing_memory_size (int mb);
void BlLoadKernel();
void BlSetupPaging();



//static char *codename = "Gramado Boot";
//char kernel_file_name[] = "kernel.bin";
//static char **argv = { NULL, NULL, NULL };
//static char **envp = { NULL, NULL, NULL };


/*
 ************************************************
 * BlMain:
 *     Entrada da parte em C do Boot Loader. 
 *
 * In this function:
 *     + Inicializa��es.
 *     + Carrega kernel.
 *     + Carrega m�dulos do kernel e 
 *       metafiles de inicializa��o.
 *     + Configura a pagina��o.
 *     + Retorna para head.s, o c�digo em 
 *       assembly que chamou essa rotina.
 */
 
//void BlMain( int argc, char *argv[], char *envp[] ) 

void BlMain (){

    int Status = -1;

    //
    // Set GUI.
    //

	// Isso foi meio imperativo.
	// VideoBlock.useGui = 1;
	// VideoBlock.useGui = 0;


    //
	// INIT ~ Faz inicializa��es b�sicas.
    //
    
    Status = (int) init ();

    if (Status != 0 ){
    // #todo
    }


//
// ========================== memory ===============================
//

    // #test
    // Sondando quanta mem�ria f�sica tem instalada.
    // Faremos isso antes de mapearmos qualquer coisa e
    // antes de carregarmos qualquer arquivo.
    // #todo:
    // Temos v�rias quest�es � se considerar, como o fato de sujarmos
    // a IVT no in�cio da mem�ria.


    unsigned long __address = 0;

    // ok
    //__address = (unsigned long) init_testing_memory_size (4);
    
    
    // ok
    //__address = (unsigned long) init_testing_memory_size (64);
    
    
    //ok
    //__address = (unsigned long) init_testing_memory_size (127);    
    

    // 511
    //__address = (unsigned long) init_testing_memory_size (1024);        
    
    //para testar na m�quina real com 2048 mb instalado.
    __address = (unsigned long) init_testing_memory_size (2048);        
        
    printf ("init: address = %x \n", __address);
    refresh_screen();
    //while(1){}

//
// ========================== memory ===============================
//




    // #todo
    // Precisamos reconsiderar a necessidade de fazermos isso.
    // O timer ira atrazar a inicializa��o ?
    // Precisamos dessas interrup��es para alguma coisa como
    // o driver de ata ?

    // #debug
    // printf("habilitando as interrupcoes\n");
    // refresh_screen();

    asm ("sti");



	//#test
	//Inicializando aloca��o din�mica de mem�ria.
    init_heap ();

	//#test

	 /*
	void *tmp;
	
	tmp  = (void *) malloc (4096);
	
	if ( (void *) tmp == NULL )
	{
	    printf("\n\n\n \n\n\n \n\n\n malloc test fail\n");

	}else{
	    printf("\n\n\n\n\n malloc test ok = %x \n",tmp);
	}
	
	//#debug breakpoint
	refresh_screen();
	while(1){}
	*/

	//#importante
	//Para inicializar o IDE precisa inicializar antes a aloca�ao dinamica.

    init_hdd ();


	//
	// #todo: Clean the screen.
	//

	// Welcome Message.
	// banner

#ifdef BL_VERBOSE
    printf ("BlMain: Starting Boot Loader..\n");
	//Debug:
	//kprintf( "BlMain: Boot Loader 32 bits em C (TEXT Mode) #test. #hang", 9, 9 );
	//while(1){}
#endif



    if (g_initialized != 1)
    {
        printf ("BlMain:");
        die ();
    }


	// Debug...
	// printf("#DEBUG: *HANG\n");
	// refresh_screen();
	// while(1){}


	//*Importante:
    // ===========
    //     Daqui pra frente vamos carregar os arquivos. 
    // Lembrando que o Boot Loader ainda n�o sabe carregar de 
    // outro dispositivo se n�o IDE. 


    //
    // Inicia os carregamentos.
    //


	//Carrega arquivos.
#ifdef BL_VERBOSE	
    printf ("BlMain: Loading files..\n");
    refresh_screen ();
#endif


	// #importante:
	// Carregando o diret�rio raiz e a fat na mem�ria.
	// Evitando repeti��o de carregamento.


	// Ok isso deu certo.

    fs_load_rootdirEx ();
    fs_load_fatEx ();


    g_fat16_root_status = 1;
    g_fat16_fat_status = 1;


	//
	//  ## Loading files ... ##
	//

	//loading kernel base.

    BlLoadKernel ();


	// @Todo: (Pensando na possibilidade)
	// Aqui podemos ter acondi��o de carregarmos apenas o krnel base.
	// Essa op��o dever� ser habilitada atrav�s de um arquivo de configura��o.
	// Ou no header do kernel base poder� ter a indica��o de quais m�dulos 
	// deve-se carregar.


	// #importante
	// Precisamos de uma flag aqui, com a op��o de n�o carregarmos
	// os outros arquivos e somente carregarmos o kernel.


 


    // #todo
    // Cancelado o carregamento dos arquivos do gramado core.
    // Podemos usar esse esquema para carregarmos arquivos
    // de configura��o usados por esse bl.
    // Podemos usar um arquivo para passarmos dados para o kernel.
    // carregando o arquivo agora e escrevendo nele.

    // BlLoadConfigFiles ();   // criar isso.


	// Paging:
	//     Depois carregar o kernel e os m�dulos 
	//     nos seus endere�os f�sicos, 
	//     configura a pagina��o e 
	//     volta para o assembly para 
	//     configurar os registradores e 
	//     passar o comando para o kernel.
	//
	// Obs:
	//     Essa configura��o b�sica n�o impede
	//     que o kernel fa�a uma reconfigura��o completa.

#ifdef BL_VERBOSE	
	printf ("BlMain: Initializing pages..\n");
	//refresh_screen();
#endif	
	
    BlSetupPaging ();


	//@todo: Atualizar status.

// Done:
//     Ao retornar, head.s configura CR0 e CR3.


//done:

	// Debug message.

#ifdef BL_VERBOSE	
    printf ("BlMain: Done\n");
    //printf ("BlMain: LFB={%x} \n",g_lbf_pa);
    //printf ("#DEBUG: *HANG\n");
    refresh_screen ();
    //while(1){};
#endif

}


/*
 ***************************************************************
 * BlLoadKernel: 
 *     Carrega o Kernel.
 *     Kernel carregado em 0x00100000, entry point em 0x00101000.
 */ 

void BlLoadKernel (){

    int Status;

    Status = (int) load_kernel ();
    
    if ( Status != 0 ){
        printf ("BlLoadKernel:\n");
        die ();
    }
}


 
/*
 ********************************************************************
 * BlSetupPaging:
 *     Configura as p�ginas.
 *
 * In this function:
 *
 * @diretorio:
 *   page_directory = 0x9C000
 *   OBS: Esse diret�rio criado ser� usado pelas primeiros processos durante
 * essa fase de constru��o do sistema.
 *        O ideal � um diret�rio por processo.
 *        Toda vez que o kernel iniciar a execu��o de um processo ele deve 
 * carregar o endere�o do diretorio do processo em CR3.
 *       Por enquanto s� tem um diret�rio criado.
 *
 * @p�ginas:
 *   km_page_table  = 0x8C000 (RING 0).
 *   um_page_table  = 0x8E000 (RING 3).
 *   vga_page_table = 0x8F000 (RING 3).
 *   lfb_page_table = ?? (RING 3).
 *
 *  @todo: 
 *      Esses endere�os precisam ser registrados em vari�veis globais ou
 * dentro de uma estrutura para se passado para o Kernel.
* 
 *      Essa deve ser uma interface que chama as rotinas de configura��o
 * da pagina��o. 
 */

void BlSetupPaging (){

    SetUpPaging ();
}


/*
 ********************************************
 * BlAbort:
 *     Rotina para abortar o bootloader em caso de erro grave.
 */

void BlAbort (){


	//@todo: 
	//    Talvez poderia ter uma interface antes de chamar a rotina abort().
	//
	//ex:
	//checks()

    abort (); 
}


/*
 *****************************************************************
 * BlKernelModuleMain:
 *     Se � o kernel que est� chamando o Boot Loader na forma de 
 * m�dulo do kernel em kernel mode.
 *
 */
 
void BlKernelModuleMain (){

    printf ("BlKernelModuleMain: Boot Loader\n");
    refresh_screen ();
}



//
//================================================================
// begin - Testing memory size
//================================================================
//

//interna
unsigned long init_testing_memory_size (int mb)
{
    unsigned char *BASE = (unsigned char *) 0;  
    
    int offset; 

    int i;


    // salvando os valores durante o test
    unsigned char ____value1 = 0;             
    unsigned char ____value2 = 0;                 
    
    
    //
    // Flag.
    //

    //acionando
    ____testing_memory_size_flag = 1;




    printf ("=========================================\n");
    printf ("__testing_memory_size: Looking for %d MB base...\n", mb);
    refresh_screen();


    // Come�amos em 4MB porque o kernel est� no primeiro mega.
    for (i=1; i< (mb+1); i++)
    {

        //printf ("i=%d \n", i);
        //refresh_screen();

        offset = 0x00100000 * i;
        
        //printf ("coloca \n");
        //refresh_screen();
                    
        //coloca.
        BASE[offset +0] = 0xAA;  //1
        BASE[offset +1] = 0x55;  //2
        
        
        //printf ("retira \n");
        //refresh_screen();

        //retira dois chars.
        ____value1 = BASE[offset +0];
        ____value2 = BASE[offset +1];
        
        
        
        
        // Se retiramos os mesmos bytes que colocamos.
        if (____value1 == 0xAA && ____value2 == 0x55)
        {
            //salvamos o �ltimo endere�o de mem�ria v�lido.
            __last_valid_address =  (unsigned long) &BASE[offset];
        
            // continua sondando.

        // Se n�o conseguimos retirar os mesmos bytes que colocamos
        // e n�o tivemos uma exce��o.
        }else{

            ____testing_memory_size_flag = 0;
            
            printf ("__testing_memory_size: out of range with no exception! \n");
            printf ("__testing_memory_size: last valid address = %x \n", 
                __last_valid_address);
            refresh_screen();
            
            
            ____testing_memory_size_flag = 0;
            return __last_valid_address;
            
            /*
            while(1)
            {
                asm ("cli");
                asm ("hlt");
            }
            */
        }
    };


     ____testing_memory_size_flag = 0;        
            
    // ok temos um endere�o de mem�ria
    // tamb�m ser� salvo em uma vari�vel global para o caso de panic.
    return __last_valid_address;
}
 


//
//================================================================
// end - Testing memory size
//================================================================
//



/*
 **************************************************
 * die:
 *     A inten��o dessa rotina deixar o processador no 
 * estado hlt durante um erro fatal. Para que ele n�o fique 
 * funcionando a 100% num loop infinito.
 * Final message !
 * Bullet, Message.
 * Wait forever. 
 * Halt system.	
 * No return. 
 */

void die (){

    printf ("die: (BL.BIN) * System Halted");
    refresh_screen ();

    while (1){
        asm ("cli");
        asm ("hlt");
    };
}


//
// End.
//


