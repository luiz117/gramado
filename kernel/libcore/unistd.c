/*
 * File: unistd.c  
 * 
 */


#include <kernel.h>



/*
 * stdio_file_write: 
 *     Escreve no arquivo uma certa quantidade de caracteres de uma 
 *     dada string.
 */

int stdio_file_write ( FILE *stream, char *string, int len ){

    int i;
    char *p;

  
    p = string;

  
    if ( (void *) stream == NULL )
    {
		printf ("stdio_file_write: stream\n");
		refresh_screen();
        return EOF; 
    }

    if ( (void *) p == NULL )
    {
		printf ("stdio_file_write: p\n");
		refresh_screen();
        return EOF; 
    }



    for (i=0; i<len; i++)
    {
        fputc ( ( int ) *p, stream );
        p++;
    };


    return 0;
}



// copiar um buffer para uma stream.
// dado o fd.
int sys_write (unsigned int fd,char *buf,int count)
{
    struct process_d *__P;
    FILE *stream;
    int len;
    
    
    if (fd<0)
        return -1;
        
    if (fd >= 32)
        return -1;


    // len
    len = strlen( (const char *) buf );
    
    if (len > count )
        len = count;
    
    if (len > 64 )
        len = 64;
    
    
    
    
    __P = (struct process_d *) processList[current_process];

    if ( (void *) __P == NULL )
    {
		printf ("sys_write: __P\n");
		refresh_screen();
        return -1; 
    }


    stream = ( FILE * ) __P->Streams[fd];

    if ( (void *) stream == NULL )
    {
		printf ("sys_write: stream\n");
		refresh_screen();
        return -1; 
    }


    // Escreve em uma stream uma certa quantidade de chars.
    stdio_file_write ( (FILE *) stream, (char *) buf, (int) count );
    
    return 0;
}


//todo
int sys_read (unsigned int fd,char *buf,int count)
{
    return -1;
}



//SVr4,  4.3BSD,  POSIX.1-2001. 

int open (const char *pathname, int flags, mode_t mode ){
	
	//#obs:
	//vamos retornar o indice da tabela de arquivos abertos do processo atual.

	FILE *stream;
		
	struct process_d *p;
	int i;
	
	//int pid = -1;
	
	if (current_process < 0)
		return -1;
	
	p = (void *) processList[current_process];
	
	if ( (void *) p == NULL )
	{
	    return -1;
	}else{
		
		//#bugbug
		//o argumento 'mode' recebido e' um unsigned short.
		//e o mode de fopen 'e um ponteiro.
		
		//filename, mode
		stream = (FILE *) fopen ( (const char *) pathname, (const char *) 0 );
	
		if ( (void *) stream == NULL )
		{
			printf ("klibc-unistd-open: fail\n"); 
		    return -1;
		}else{
		
		     // agora temos uma stream va'lida. 
			 // procurar um slot vazio na lista de arquivos abertos do 
			 // processo atual.
	
			 //NUMBER_OF_FILES = número máximo de arquivos abertos em um processo.
		     for ( i=0; i< NUMBER_OF_FILES; i++ )
			 {
				 // Se encontramos um slot vazio, 
				 // colocamos o ponteiro para a stream nele e retornamos o pid.
				 // tipo: unsigned long
			     if ( p->Streams[0] == 0 )
				 {
				     p->Streams[i] = (unsigned long) stream ;
					 return (int) i;
				 }
			 }
			 		
			 //Nao encontramos um slot vazio.
			 return -1;
		}		
	};
	
//fail:
	//printf("klibc-unistd-open(): todo!\n");
    return -1;
}



// fechar um dos arquivos abertos do processo atual.
// o descritor 'e um ipndice na sua tabela dearquivos abertos.

int close ( int fd ){

	FILE *stream;
		
	struct process_d *p;
	int i;
	

	if ( fd < 0 || fd >= NUMBER_OF_FILES)
		return -1;
	
	if ( current_process < 0 )
		return -1;
	
	p = (void *) processList[current_process];
    
	if ( (void *) p == NULL )
	{
	    return -1;
	}else{
	
	     stream = (FILE *) processList[fd];
		 
		 if ( (void *) stream == NULL )
		 {
		     return -1;
		 }else{
		 
		     // #todo
			 // Fechar corretamente esse arquivo e liberar os recursos 
			 // associados a ele.
			 
			 stream = NULL;
			 processList[fd] = (unsigned long) 0;
			 
			 //ok.
			 return 0;
		 }
	
	};
	
	//printf ("klibc-unistd-close: todo\n");
    return -1;
}


long fpathconf (int fildes, int name){
    return -1;
}

long pathconf (const char *pathname, int name){
    return -1;
} 


//pegando host name
//faremos igual fizemos com usuário.
//suporte a rotina da libc.
int __gethostname (char *buffer){


    char *hostname_buffer = (char *) buffer;
 

	//Estrutura default para informações sobre o host.
	//host.h

    if ( (void *) HostInfo == NULL )
    {
        printf ("__gethostname: HostInfo\n");
        return (int) -1;
    }else{

                 //64 bytes
        strcpy ( hostname_buffer, (const char *) HostInfo->__hostname );
        
        return (int) HostInfo->hostName_len;
    };


    return (int) -1;
}


//#todo
// configurando o hostname.
//do mesmo jeito que configuramos o username,
//só que em estruturas diferentes
int __sethostname (const char *new_hostname){

    //
    // Estrtutura HostInfo em host.h (network)
    //


    if ( (void *) HostInfo == NULL )
    {
        printf ("__sethostname: HostInfo\n");
        return (int) -1;
    }else{

        HostInfo->hostName_len = (size_t) strlen (new_hostname) + 1;
        
        //64 bytes
        strcpy ( HostInfo->__hostname, (const char *) new_hostname);

		return 0;
	};


    return (int) -1;
}










