/*
 * File: crts/klibc/stdio.c 
 *
 * Descri��o:
 *     +Rotinas de input/output padr�o.
 *     +Algumas rotinas de terminal.
 *
 * Environment:
 *     >>> Ring 0 <<<
 *
 *
 * @todo: Buffering and pipes.
 *        Priorizar opera��es com disco.
 * 
 * Credits:
 *     printf support - Georges Menie's tutorial.
 *     https://www.menie.org/georges/embedded/small_printf_source_code.html
 *
 * History:
 *     2015 - Create by Fred Nora.
 *     2019 - Revision.
 */
  
 
#include <kernel.h>


// Herdadas do Boot Loader.
// De onde vem isso ?? head.s
// #todo: 
// Devemos chamar o m�dulo hal para obtermos esses valores.
// Depois salvamos em vari�veis internas usadas pela gui.

extern unsigned long SavedBootBlock;
extern unsigned long SavedLFB;
extern unsigned long SavedX;
extern unsigned long SavedY;
extern unsigned long SavedBPP; 


// Internas.

//void scroll_screen_rect (void);


/*
 *********************************************
 * fclose:
 *     Close a file. 
 */

int fclose (FILE *stream){

    if ( (void *) stream == NULL )
    {
        return (int) (-1);

    }else{

        stream->used = 1;
        stream->magic = 1234;

        stream->_p = NULL;
        stream->_cnt = 0;
        stream->_base = NULL;
        stream->_flags = 0;
        stream->_file = 0;
        stream->_charbuf = 0;
        stream->_lbfsize = 0;
        stream->_tmpfname = NULL;
        stream = NULL;

    };

	//...

    return 0;
}


/*
 ******************************************
 * fopen:
 *     Open a file.
 *     
 *     #todo: 
 *     Mas j� temos recursos para abrimos arquivos maiores 
 *     usando essa fun��o. � s� obtermos o tamanho do arquivo 
 *     e alocarmos um buffer do tamanho necess�rio
 */

//#bugbug
//ainda nao usamos o argumento mode.

//#bugbug
//na m�quina real falhou no momento de pegar o tamanho do arquivo.
//debug: vamos colocar verbose nessa rotina e olhar na m�quina real
//se o problema aparece.

//#bugbug
//problemas na string de nome.

// #todo: 
// Precisamos inicializar corretamente a estrutura antes de 
// retornarmos o ponteiro.

FILE *fopen ( const char *filename, const char *mode ){

    unsigned long fileret;

    int i;
    
	//struct _iobuf *stream;
    FILE *stream;

	// Buffer para armazenar o arquivo que vamos abrir.
    char *file_buffer;

	// #bugbug: 
	// aqui podemos usar o kmalloc ?
	// Buffer usado para colocar a estrutura, 
	// mas o problema � o free que ainda n�o funciona.

    // 1024 � desperd�cio.
    unsigned char struct_buffer[1024];

    //
    // stream;
    //

	//buffer da estrutura.
    stream = (FILE *) &struct_buffer[0];


	// #bugbug
	// Estamos com problemas com a string de nome.
	// #debug: vamos exib�-la.

	// #debug
	//printf ("before_read_fntos: %s\n", filename );
	
    read_fntos ( (char *) filename );

	// #debug
	//printf ("after_read_fntos: %s\n", filename );

	// #debug
	//refresh_screen ();
	
	
    //#bugbug
    //na m�quina real falhou no momento de pegar o tamanho do arquivo.
    //debug: vamos colocar verbose nessa rotina e olhar na m�quina real
    //se o problema aparece.
	
	//#test
	//temos que fazer isso de um jeito melhor

    size_t s = (size_t) fsGetFileSize ( (unsigned char *) filename );

    if ( s <= 0 )
    {
        printf ("fopen: file size \n");
        goto fail;

		//refresh_screen();
		//return (FILE *) 0;
    }


	// #debug
	//printf ("after_fsGetFileSize: %s\n", filename );
	//refresh_screen ();


	//#bugbug
	//Quando pegamos o tamanho do arquivo e for muito grande
	//o kmalloc vai falhar.
	//precisamos exibir o tamanho do arquivo.

	//vamos impor um limite.
	// 128 kb

	//printf ("klibc: fopen limits. size=%d \n", s);
	//refresh_screen ();


	/*
	 *usado para alocar mais p�ginas.
	Ret = (void*) allocPages(2);      //8KB. para imagem pequena.
	if( (void*) Ret == NULL ){
	    printf("Ret fail\n");
        goto fail;
        //return NULL;
	}
    */




	//alocando apenas uma p�gina.
	//4KB
	//Buffer do arquivo.
	//@todo: Deve ser maior. Do tamanho do arquivo.
	//Podemos usar outra rotina de aloca��o de p�gina.
	
	//#todo:
	//j� temos recursos para alocar mem�ria para um buffer maior.
	//obs: Essa aloca��o vai depender do tamanho do arquivo.
	


	//file_buffer = (char *) newPage();
    file_buffer = (char *) kmalloc (s);

    if ( (char *) file_buffer == NULL )
    {
        printf ("fopen: file_buffer \n");
        goto fail;

		//refresh_screen ();
		//return (FILE *) 0;
    }


	//
	// Configurando a stream;
	//

    if ( (void *) stream == NULL )
    {
        printf ("fopen: stream \n");
        goto fail;
    
		//refresh_screen();
		//return (FILE *) 0;

	}else{

        stream->used = 1;
        stream->magic = 1234;

        stream->_base = file_buffer;
        stream->_bf._base = file_buffer;
        stream->_lbfsize = s;    // File size.
        stream->_r = 0;
        stream->_w = 0;
        stream->_p = stream->_base;

		//?? #todo: 
		// precisamos de um id
		// Esse ID � um indice da estrutura de processo.

        stream->_file = 0; 

        stream->_tmpfname = (char *) filename;

        // Quanto falta para acabar o arquivo.
        stream->_cnt = s;


        // #todo
        // Talvez tenhamos que colocar o ponteiro em Streams[i]
         
		//...

    };


	// #bugbug:
	// Aten��o !!
	// Por enquanto esse esquema de pwd mais atrapalha que ajuda.

	// pwd support.
    fsUpdateWorkingDiretoryString ( (char *) filename );

    if ( current_target_dir.current_dir_address == 0 )
    {
        printf ("fopen: current_target_dir.current_dir_address fail \n");

		//reset.
        current_target_dir.current_dir_address = VOLUME1_ROOTDIR_ADDRESS;

        for ( i=0; i< 11; i++ )
        {
            current_target_dir.name[i] = '\0';
        }

        goto fail;
        //return (FILE *) 0;
    }

	//
	// Loading file.
	//


	//#debug
	//printf ("before_fsLoadFile: %s\n", filename );

    fileret = fsLoadFile ( VOLUME1_FAT_ADDRESS, 
                  current_target_dir.current_dir_address, 
                  (unsigned char *) filename, 
                  (unsigned long) stream->_base );

	//printf ("after_fsLoadFile: %s\n", filename );  

    if ( fileret != 0 )
    {
        stream = NULL;

        printf ("fopen: fsLoadFile fail\n");
        goto fail;

		//refresh_screen ();
		//return (FILE *) 0;
    }

done:
    return (FILE *) stream;

fail:

    refresh_screen ();
    return (FILE *) 0;
}



/*
 **************************************************
 * fread:  
 * 
 */
 
// #importante
// Ler uma certa quantidade de chars de uma stream e coloca-los no buffer.
// Isso vai ser usado pelo terminal por exemplo.
// Assim o terminal pode pegar uma string que esteja no arquivo.
// https://www.tutorialspoint.com/c_standard_library/c_function_fread.htm 


size_t fread (void *ptr, size_t size, size_t n, FILE *fp){

	//tamanho do bloco. char short long ...
	//if ( size <= 0 )
	    //return -1;


    if ( n <= 0 )
    {
        printf ("fread: n \n");
        refresh_screen();
        return (size_t) -1;
    }


    if ( (void *) fp == NULL )
    {
        printf ("fread: fp\n");
        refresh_screen();
        return (size_t) -1;

    }else{

       // if (s->flags & IOSTREAM_READ)
       //switch (s->node->type) arquivo, dispositivo, etc ... 


		//if (stream->offset <= file->size)

		//ai tem algo errado
        if ( fp->_p < fp->_base )
        {
            printf ("fread: position fail\n");
            refresh_screen();
            return (size_t) -1;
        }

        // dst = buffer no app.
        // src = base do buffer da stream + offset de leitura.

        //#todo: 
        // ainda n�o temos uma base do buffer devidamente inicializada. 
		
		// #hack hack: 
		// provis�rio, deletar depois.
		// precisamos inicializar quando usamos fopen
		// suspendendo esse hack pois fiz as modifica��es no fopen
		// pra inicialziar a estrutura.
		// Ent�o isso deve funcionar sem esse ajuste 
		// pelo menos para arquivos abertos com fopen.
		
		//fp->_bf._base = fp->_base; 
		//fp->_r = 0; 

        //...

		//#debug
        printf ("fread: copiando para o buffer\n");
        printf ("_r=%d \n", fp->_r );
        printf ("_bf._base=%x \n", fp->_bf._base );
        refresh_screen ();

        // #todo: 
        // _r limits

        memcpy ( (void *) ptr, 
            (const void *) (fp->_bf._base + fp->_r ), 
            (unsigned long) n );

		 // Update.        
         fp->_r = (fp->_r + n);  
    };


    return 0;
}


/*
 ************************************************
 * fwrite:  
 *     #todo
 */

size_t fwrite (const void *ptr, size_t size, size_t n, FILE *fp){

	//tamanho do bloco. char short long ...
	//if ( size <= 0 )
	    //return -1;


    if ( n <= 0 )
    {
        printf ("fwrite: n \n");
        refresh_screen ();
        return (size_t) -1;
    }


    if ( (void *) fp == NULL )
    {
        printf ("fwrite: fp\n");
        refresh_screen ();
        return (size_t) -1;

    }else{

       // if (s->flags & IOSTREAM_READ)
       //switch (s->node->type) arquivo, dispositivo, etc ... 

		//#debug
        //printf ("fwrite: copiando do buffer\n");
        //printf ("_w=%d \n", fp->_w );
        //printf ("_bf._base=%x \n", fp->_bf._base );
        //refresh_screen ();

		// #todo: 
		// _w limits

        memcpy ( (void *) (fp->_bf._base + fp->_w ), 
            (const void *) ptr, 
            (unsigned long) n );
     
		// Update.        
        fp->_w = fp->_w + n; 
    };


    return 0;
}


/* 
 *********************************************
 * fflush: (ring 0)
 *     ?? Salva o buffer no arquivo associado a ele e limpa o buffer. 
 *     ?? Exibe o conte�do do arquivo na tela do terminal.
 *     Se for NULL ent�o faz isso para todas as stream abertas.
 *     Retorna 0 se funcionar e retorna EOF se falhar.
 */

// The fflush() function is used because standard output is usually buffered 
// and the prompt may not immediately be printed on the output or terminal.

// https://linux.die.net/man/3/fflush
// http://man7.org/linux/man-pages/man3/fflush.3.html

/*
 #importante:
 Now, stdout is, by default, line buffered. This means, 
 the output sent to stdout is not sent to the screen for display 
 (or to the redirected files/streams) until it gets a newline character in it. 
 So, if you want to override the default buffering behaviour, 
 then you can use fflush to clear the buffer (and in the process, 
 send everything to the screen/file/stream).
 
 Ent�o fflush envia pra tela aquilo que est� somente no arquivo
 pois ainda n�o tem um '\n' '\r' no arquivo.
 Isso significa que fprintf n�o pode ativar a rotina de pintura
 enquanto n�o encontrar um '\n'
*/

int fflush ( FILE *stream ){
	
	//printf ("fflush: suspenso \n");
	//refresh_screen ();
	//return -1;

	// vamos pegar o conteudo dessa stream
	// e copiarmos na stream do processo.
	// se for null temos que copiar todos os stream.
	
	
	// #importante
	// o argumento pode ter passado uma stream de ring3,
	// ent�o devemos copiar essa stream para o fluxo padr�o
	// em ring0 usando as stream do fluxo padr�o na estrutura
	// do processo.
	
	//ponteiros para as streams da estrutura do processo.
	
	//FILE *p_stdin;
	//FILE *p_stdout;
	//FILE *p_stderr;


    FILE *p_stream;

    struct process_d *p;
    //struct thread_d *t;

    register int i = 0;
    int j;


    if ( (void *) stream == NULL )
    {
        panic ("fflush: stream is NULL \n");
        //refresh_screen ();
        //return -1;
    }


	// Limits.
	// Se o buffer tiver vazio ou for maior que o limite.


	/*
	if ( stream->_bufsiz == 0 || stream->_bufsiz > BUFSIZ )
	{	
		printf ("fflush: buffer size limits %d \n", stream->_bufsiz);
		stream->_bufsiz = BUFSIZ;
		refresh_screen ();
		return (int) (-1);
	}
	*/



	//
	//  process
	//

    p = (struct process_d *) processList[current_process];

    if ( (void *) p == NULL )
    {
        panic ("fflush: p\n");
        //refresh_screen ();
        //return (int) (-1);

    }else{

	    //p_stdin = (FILE *) p->Streams[0];  
	    //p_stdout = (FILE *) p->Streams[1]; 
	    //p_stderr = (FILE *) p->Streams[2]; 


        for ( i=0; i<32; i++ )
        {
            p_stream = (FILE *) p->Streams[i];

			// Found.
            if ( stream == p_stream )
            {
                goto found;   
            }
        }

        goto not_found;
    };



found:

    switch (i)
    {
        case 0:
			//printf ("fflush: The index is 0\n");
			//stdin = stream;
			goto done;
			break;

        case 1:
			//printf ("fflush: The index is 1\n");
			//fprintf (stdout, stream->_base );
			//for ( j=0; j < stdout->_bufsiz; j++ )
			//{
			//	stdout->_ptr[j] = stream->_base[j];	
			//}
			//stdout = stream;
			//stdio_file_write ( FILE *stream, char *string, int len );
			goto print_stdout;
			break;

        case 2:
			//printf ("fflush: The index is 2\n");
			//fprintf (stderr, stream->_base );
			//for ( j=0; j < stderr->_bufsiz; j++ )
			//{
			//	stderr->_ptr[j] = stream->_base[j];	
			//}			
			//stderr = stream;
			//stdio_file_write ( FILE *stream, char *string, int len );
			goto done;
			break;
			
        default:
			printf ("fflush: default index\n");
			refresh_screen ();
	        goto done;
			break;
    };
	
	
print_stdout:
    goto done;


not_found:
    printf ("fflush: not found\n");
    refresh_screen ();
    //goto done;


done:

	//vamos pintar mesmo que n�o tenhamos encontrado um '\n'
    CurrentTTY->print_pending = 1;

    return 0;
}



/*
 ***************************** 
 * update_standard_stream:
 *     Atualiza o fluxo padr�o para determinado processo
 *     #bugbug: Talvez n�o estamos mais usando isso.
 */

int 
update_standard_stream ( int PID, 
                         FILE *stream1, 
                         FILE *stream2, 
                         FILE *stream3 )
{
    struct process_d *p;

    //#debug
    //printf ("update_standard_stream: PID %d \n", PID);

	//#todo limits
	if ( PID < 0 )
		return -1;

    p = (struct process_d *) processList[PID];	

    if ( (void *) p == NULL )
    {
        panic ("update_standard_stream: p fail\n");
    }


	if ( (void *) stream1 == NULL || 
		 (void *) stream2 == NULL || 
		 (void *) stream3 == NULL )
	{
		panic ("update_standard_stream: args fail\n");
	}
	
	//p->Streams[0] = (unsigned long) stream1;  
	p->Streams[1] = (unsigned long) stream2; 
	p->Streams[2] = (unsigned long) stream3;
	
	//stdin = stream1;
	stdout = stream2;
	stderr = stream3;
	
	//
	// #bugbug
	// J� que foi passado um PID ent�o temos que atualizar 
	// o fluxo padr�o do processo e n�o o atual.
	//
	
	/*
    if ( (void *) CurrentTTY == NULL )
	{	
		panic ("update_standard_stream: CurrentTTY");
		
	}else{
	    
		//CurrentTTY->stdin = stdin;
	    CurrentTTY->stdout = stdout;
	    CurrentTTY->stderr = stderr;
	};
    */

	//#debug
	//printf ("update_standard_stream: done\n");
	//refresh_screen ();


    return 0;
}


/*
 ************************* 
 * scroll_screen_rect:
 * 
 */

//scroll test
//fun��o interna de suporta ao scroll()

void scroll_screen_rect (void){

    //unsigned long x = 0; 
    //unsigned long y = 0; 
    //unsigned long width = 0;  //800
    //unsigned long height = 0;  //600

	// #TEST
    register unsigned int i;
	// unsigned int i;

    unsigned int line_size, lines;
    unsigned int offset;
    unsigned long Width = (unsigned long) screenGetWidth ();
    unsigned long Height = (unsigned long) screenGetHeight ();

	//line_size = (unsigned int) width; 
	//lines = (unsigned int) height;

    line_size = (unsigned int) Width; 
    lines = (unsigned int) Height;
	
	// = 3; 
	//24bpp
    int bytes_count;

    switch (SavedBPP)
    {
		case 32:
		    bytes_count = 4;
		    break;
		
		case 24:
		    bytes_count = 3;
			break;
			
		//...
    };

    int cHeight = get_char_height ();

	//
	// Origem e destino.
	//


	//destino
    void *p = (void *) BACKBUFFER_ADDRESS;

	// o y � a linha da origem. o deslocamento de ter a altura de um char.
	// origem
    const void *q = (const void *) BACKBUFFER_ADDRESS + ( bytes_count * SavedX * cHeight ) ;



	// #aten��o.
	
	//offset = (unsigned int) BUFFER_PIXEL_OFFSET( x, y );
	
	//offset = (unsigned int) ( (bytes_count*SavedX*(y)) + (bytes_count*(x)) );
	
	//p = (void *) (p + offset);    
	//q = (const void *) (q + offset);    
	 
	// #bugbug
	// Isso pode nos dar problemas.
	// ?? Isso ainda � necess�rio nos dias de hoje ??
	
	//vsync ();

	//(line_size * bytes_count) � o n�mero de bytes por linha. 

    int count; 

	//#importante
	//� bem mais r�pido com m�ltiplos de 4.	
	
	//se for divis�vel por 4.
    if ( ((line_size * bytes_count) % 4) == 0 )
    {
        count = ((line_size * bytes_count) / 4); 

	    for ( i=0; i < lines; i++ )
	    {
		    //copia uma linha ou um pouco mais caso n�o seja divis�vel por 
		    memcpy32 ( p, q, count );
		    
			q += (Width * bytes_count);
	 		p += (Width * bytes_count);
	    };
    }

	//se n�o for divis�vel por 4.
    if ( ((line_size * bytes_count) % 4) != 0 )
    {
	    for ( i=0; i < lines; i++ )
	    {
		    memcpy ( (void *) p, (const void *) q, (line_size * bytes_count) );
		    
			q += (Width * bytes_count);
		    p += (Width * bytes_count);
	    };
    }
}



/*
 ********************************
 * kclear:
 *     Limpa a tela em text mode.
 *     # isso n�o faz parte da lib c. Deletar.
 */

int kclear (int color) {

	int Status = -1;
	
	if ( VideoBlock.useGui == 1 )
	{
		backgroundDraw ( COLOR_BLUE );

		TTY[current_vc].cursor_x = 0; //g_cursor_x = 0;
		TTY[current_vc].cursor_y = 0; //g_cursor_y = 0;

		Status = 0;

	}else{
		Status = -1;
	};

	return (int) Status;
}


/* 
 * kclearClientArea: 
 */

// Limpa a tela em text mode.
// Isso n�o faz parte da lib c. Deletar.

int kclearClientArea (int color)
{
    return (int) kclear (color);
}


//#todo:
//insert_char


/*
 *************************************
 * insert_line:
 * 
 */

// Incluir uma linha no buffer de linhas da estrutura do tty atual.
// vamos copiar esse esquema do edito de textos em ring3.

int insert_line ( char *string, int line ){
	
	/*
	
	int l;
	struct tty_line_d *Line;
	
	//if ( (void *) string == NULL )
	//	return -1;
	
    if ( (void *) CurrentTTY != NULL )
	{
        if ( CurrentTTY->used == 1 && CurrentTTY->magic == 1234 )
		{
			//Linha atual para se escrever no stdout do current tty
		    l = CurrentTTY->current_line;
			
			//Pega o ponteiro para estrutura de linha da linha atual.
			Line = (struct tty_line_d *) CurrentTTY->lines[l];
			
			if ( (void *) Line == NULL )
				return -1;
			
			//Buffer pra colocar chars.
		    //Line->CHARS[0]  
			//Line->ATTRIBUTES[0] 
				
			//inicio do texto dentro da linha atual
		    //CurrentTTY->left
			
			//fim do texto dentro da linha atual
			//CurrentTTY->right
			
			//posi��o do ponteiro dentro da linha atual.
			//CurrentTTY->pos
		
		}
	};
	*/
    return (int) -1; 
}



/*
 *===========================================================================
 *  ==== Segue aqui o suporte a fun��o 'printf' ====
 *
 * #obs:
 * Em user mode temos uma modelo mais tradiciona de printf,
 * talvez seja bom implementa-lo aqui tamb�m.
 */


/*
 *****************************
 * prints:
 *     Rotina de suporta a printf. 
 */

static int prints ( char **out, const char *string, int width, int pad ){

    register int pc = 0, padchar = ' ';

    if (width > 0) 
    {
	    register int len = 0;
		register const char *ptr;
		
		for (ptr = string; *ptr; ++ptr) ++len;
		if (len >= width) width = 0;
		else width -= len;
		if (pad & PAD_ZERO) padchar = '0';
    };


    if( !(pad & PAD_RIGHT) ) 
    {
		for ( ; width > 0; --width)
		{
		    printchar (out,padchar);
			++pc;
		};
    };


    for ( ; *string; ++string )
    {
		printchar (out, *string);
		++pc;
    };


    for ( ; width > 0; --width )
    {
		printchar (out,padchar);
		++pc;
    };


    return (int) pc;
}


/*
 ****************************************
 * printi:
 *     Rotina de suporta a printf.
 */

static int 
printi ( char **out, 
         int i, 
         int b, 
         int sg, 
         int width, 
         int pad, 
         int letbase )
{
    char print_buf[PRINT_BUF_LEN];
    register char *s;
    register int t, neg = 0, pc = 0;
    register unsigned int u = i;


    if ( i == 0 ) 
    {
		print_buf[0] = '0';
		print_buf[1] = '\0';
		
		return prints (out, print_buf, width, pad);
    }


    if ( sg && b == 10 && i < 0 )
    {
		neg = 1;
		u = -i;
    }


    s = ( print_buf + ( PRINT_BUF_LEN -1 ) );

    *s = '\0';


    while (u) 
    {
		t = u % b;
		
		if ( t >= 10 )
		    t += letbase - '0' - 10;
		    *--s = t + '0';
		    u /= b;
    };


    if (neg) 
    {
		if ( width && (pad & PAD_ZERO) ) 
		{
		    printchar(out, '-');
			++pc;
			--width;
		}else {
			
			*--s = '-';
		};
    };


	// #obs: 
	// retorna pc + o retorno da fun��o.

    return (int) pc + prints(out, s, width, pad);
}


/*
 ****************************************
 * print:
 *     Rotina de suporta a printf.
 */

static int print ( char **out, int *varg ){

    register int width, pad;
    register int pc = 0;
    register char *format = (char *) (*varg++);
    char scr[2];

    for ( ; *format != 0; ++format ) 
    {

		if ( *format == '%' ) 
		{
			++format;
			width = pad = 0;
			
			if ( *format == '\0' ) 
			    break;
			
			if ( *format == '%' ) 
			    goto out;
			
			if ( *format == '-' ) 
			{
				++format;
				pad = PAD_RIGHT;
			};
			
			while ( *format == '0' ) 
			{
				++format;
				pad |= PAD_ZERO;
			};
			
			for ( ; *format >= '0' && *format <= '9'; ++format )
			{
				width *= 10;
				width += *format - '0';
			};
			
			if ( *format == 's' ) 
			{
				register char *s = *((char **)varg++);
				pc += prints (out, s?s:"(null)", width, pad);
				continue;
			}
			
			if ( *format == 'd' ){
				pc += printi (out, *varg++, 10, 1, width, pad, 'a');
				continue;
			}
			
			if ( *format == 'x' ){
				pc += printi (out, *varg++, 16, 0, width, pad, 'a');
				continue;
			}
			
			if ( *format == 'X' ){
				pc += printi (out, *varg++, 16, 0, width, pad, 'A');
				continue;
			}
			
			if ( *format == 'u' ){
				pc += printi (out, *varg++, 10, 0, width, pad, 'a');
				continue;
			}
			
			if ( *format == 'c' ) 
	        {
				/* char are converted to int then pushed on the stack */
				scr[0] = *varg++;
				scr[1] = '\0';
				pc += prints (out, scr, width, pad);
				continue;
			}
		
		}else{
			
		    out:
			    printchar (out, *format);
			    ++pc;
		};
    };


    if (out) 
        **out = '\0';


    return (int) pc;
}


/*
 ***************************************************
 * printf:
 *     @field 2
 *     The printf function.
 *     Assuming sizeof(void *) == sizeof(int).
 *
 * Em user mode temos uma modelo mais tradiciona de printf,
 * talvez seja bom implementa-lo aqui tambem.
 */

int printf ( const char *format, ... ){

    register int *varg = (int *) (&format);

    return (int) print ( 0, varg );
}


/*
 ******************
 * puts: 
 *     provis�rio ...
 */

int puts ( const char *str )
{
    return (int) printf ("%s",str);
}



/*
 *************************************************
 * panic:
 *     klibc function to show a formated string and hang the system.
 */
 
void panic ( const char *format, ... ){

    register int *varg = (int *) (&format);

    printf ("panic: KERNEL PANIC\n");
    print ( 0, varg );
    die ();
}


/*
 ***********************************************************
 * sprintf:
 *     @field 2
 *     int sprintf ( char * str, const char * format, ... );
 *     Composes a string with the same text that would be 
 * printed if format was used on printf, 
 * but instead of being printed, the content is stored 
 * as a C string in the buffer pointed by str.
 */

int sprintf ( char *str, const char *format, ... ){

    register int *varg = (int *) (&format);

    return (int) print (&str, varg);
}


/*
 ************************************************
 * fprintf:
 *     A estrtutura � gerenciada em ring0.
 *     A libc em ring3 apenas chamar� essa rotina.
 *     servi�o 234.
 */

// fflush � line buffered,
// Ent�o fflush envia pra tela aquilo que est� somente no arquivo
// pois ainda n�o tem um '\n' '\r' no arquivo.
// Isso significa que fprintf n�o pode ativar a rotina de pintura
// enquanto n�o encontrar um '\n'

int fprintf ( FILE *stream, const char *format, ... ){

    register int *varg = (int *) (&format);

	//
	// Validation.
	//

	//#debug
	//kprintf ("klibc-stdio-fprintf: stream=%x \n",stream);
	//kprintf ("klibc-stdio-fprintf: stdout=%x \n",stdout);


    if ( (void *) stream == NULL )
    {
        panic ("klibc-stdio-fprintf: stream\n");
    }else{

        if ( stream->used != 1 || stream->magic != 1234 )
        {
            panic ("klibc-stdio-fprintf: stream validation\n");
        }
		//...
    };


	//
	// print 
	//

	// #obs:
	// Isso deve ser feito antes e alterarmos o ponteiro _ptr de stdout.
	
//print:
	
	
	// Se a stream for a stdout ent�o vamos ter alertar para pintar.
	//#obs: esquema antigo, n�o usaremos mais isso por enquanto.
	
	/*
	if (stream == stdout)
	{
		//Indicamos que deve pintar.
		//isso ser� ativado pelo fflush
		CurrentTTY->stdout_status = 1;
		
		//se temos um print pendente n�o precisamos mudar o last.
		//se n�o temos um print pendente ent�o precisamos mudar o last.
		if ( CurrentTTY->print_pending == 0 )
		{
			//#bugbug: Na verdade s� podemos ativar essa
			//pend�ncia quando encontrarmos um '\n' '\r'
			//ent�o ent�o o terminal pode mostrar na tela o conte�do
			//de uma linha, normalmente a �ltima do terminal,
			//mudando de linha efetua-se o scroll.
			//>> Se ainda n�o temos um '\n' s� um fflush mostraria
			//o conte�do na tela.
			
			
			//#importante
			//Mudamos isso para printchar
			//ent�o somente depois de encontrar o \n ativaremos essa flag.
			//Isso na teoria.Porque na pr�tica t� imprimindo pra todos os casos. haha
			//CurrentTTY->print_pending = 1;
			
	        // Indicamos de onde a rotina de pintura deve come�ar.
		    CurrentTTY->stdout_last_ptr = stdout->_p;
		}
	}	
	*/




	//
	// Colocando os chars dentro do arquivo.
	//

	// Colocamos no ponteiro e nao na base.
    char *str = (char *) stream->_p;

	// #todo
	// Tem que atualizar o ponteiro com uma strlen.

    size_t len = 0;
    len = (size_t) strlen ( (const char *) format);


    int status = -1;
    status = (int) print (&str, varg);

	// Depois de ter imprimido ent�o atualizamos o ponteiro de entrada 
	// no arquivo.

    stream->_p = stream->_p + len;


    return (int) status;
}


/*
 * stdio_file_write: 
 *     Escreve no arquivo uma certa quantidade de caracteres de uma 
 *     dada string.
 */

int stdio_file_write ( FILE *stream, char *string, int len ){

    int i;
    char *p;

    p = string;

	//#todo filtrar len.

    for (i=0; i<len; i++)
    {
	    fputc ( ( int ) *p, stream );
	    p++;
    };


    return 0;
}


/*
 ********************************
 * fputs:      
 */

int fputs ( const char *str, FILE *stream ){

    int size = 0;

    if ( (void *) stream == NULL )
    {
		return (int) (-1);

    } else {
		
		size = (int) strlen (str);
		
		if ( size > stream->_cnt )
		{
			return (int) (-1);
		}
		
		stream->_cnt = (int) (stream->_cnt - size);
		
		sprintf ( stream->_p, str );
		
		stream->_p = stream->_p + size;

        return 0;
    };


    return (int) (-1);
}


/*
 *********************************
 * ungetc:
 */

int ungetc ( int c, FILE *stream ){

    if (c == EOF) 
        return (int) c;


    if ( (void *) stream == NULL )
    {
		return (int) EOF;
    }

	//@todo: flag oef.
	//stream->flags = (stream->flags & ~_IOEOF);
	
	stream->_p--;
	
	stream->_p[0] = (char) c;


    return (int) c;
}


/*
 * ftell: 
 * 
 */

long ftell (FILE *stream){


    if ( (void *) stream == NULL )
	{
		printf ("ftell fail\n");
		refresh_screen();
		return (long) -1; 
    }


    return (long) (stream->_p - stream->_base);
}


/*
 * fileno:
 * 
 * 
 */
 
int fileno ( FILE *stream ){

    if ( (void *) stream == NULL )
    {
		return (long) -1; 
    }

    // fd
    return (int) stream->_file;  
}


/*
 *********************************
 * fgetc:
 *     #precisamos exportar isso como servi�o. (#136)
 */

int fgetc ( FILE *stream ){

    int ch = 0;


    if ( (void *) stream == NULL )
    {
		// #debug
		printf ("fgetc: stream struct fail\n");
		refresh_screen();
		
		return (int) (-1);

    }else{

		 //(--(p)->_r < 0 ? __srget(p) : (int)(*(p)->_p++))
		
		//#fim.
		//cnt decrementou e chegou a zero.
		//N�o h� mais caracteres dispon�veis entre 
		//stream->_ptr e o tamanho do buffer.
		
		if ( stream->_cnt <= 0 )
		{
			stream->_flags = (stream->_flags | _IOEOF); 
			stream->_cnt = 0;
			
		    //printf ("#debug: fgetc: $\n");
			
			//isso funciona, significa que a estrutura tem ponteiro e base validos.
			//printf("show fgetc:: %s @\n", stream->_base );
		    //refresh_screen();
			
			return (int) (-1);
		};
		
		//#debug
		//nao podemos acessar um ponteiro nulo... no caso endere�o.
		
		if ( stream->_p == 0 )
		{
			printf ("#debug: fgetc: stream struct fail\n");
		    refresh_screen();
			return (int) (-1);
			
		}else{
			
			// #obs: 
			// Tem que ter a op��o de pegarmos usando o posicionamento
			// no buffer. O terminal gosta dessas coisas.
			
		    //Pega o char no posicionamento absoluto do arquivo
		    ch = (int) *stream->_p;
				
			stream->_p++;
		    stream->_cnt--;
			
		    return (int) ch;
		
		}
		//fail
    };


	//#debug
    printf ("fgetc: $$\n");
	refresh_screen();


    return (int) (-1);
}


/*
 *********************************
 * feof:
 */

int feof ( FILE *stream ){

    int ch = 0;

 
    if ( (void *) stream == NULL )
    {
		return (int) (-1);
		
    } else {

	    ch = fgetc (stream);
		
        if ( ch == EOF )
		{
			return (int) 1;
		
		}else{
			
			return 0;
		};
    };


	//checar se o eof foi atingido.
	// return( (stream->_flag & _IOEOF) );

    return 0;
}


/*
 *********************************
 * ferror:
 *
 */

int ferror ( FILE *stream ){

    if ( (void *) stream == NULL )
    {
		
		return (int) (-1);
    }


    return (int) ( ( stream->_flags & _IOERR ) );
}



/*
 **************************************
 * fseek:
 *     offset argument is the position that you want to seek to,
 *     and whence is what that offset is relative to.
 */

int fseek ( FILE *stream, long offset, int whence ){

    if ( (void *) stream == NULL )
    {
        goto fail;
    }


	// Checar limites do offset.

    switch (whence){

        case SEEK_SET:
		    //printf ("SEEK_SET\n");   
		    stream->_p = (stream->_base + offset); 
			goto done;
			break;
			
        case SEEK_CUR:
		    //printf ("SEEK_CUR\n");
		    stream->_p = (stream->_p + offset);
		    goto done;
			break;

        case SEEK_END:
		    //printf ("SEEK_END stream->_lbfsize=%d \n",stream->_lbfsize);
		    stream->_p = ((stream->_base + stream->_lbfsize) + offset); 
		    goto done;
			break;

        default:
            //printf ("default:\n");
		    goto fail;
			break;
    };


fail:
	printf ("fseek fail\n");
	refresh_screen();
    return (int) (-1);

done:
    //refresh_screen();
    return 0;
}


/*
 *****************************************
 * fputc:
 */

int fputc ( int ch, FILE *stream ){

    if ( (void *) stream == NULL )
    {
	    return (int) (-1);
		
    }else{

		// se tivermos um posicionamento v�lido de escrita no buffer ou
		// se a posi��o de escrita no buffer for maior que o limite
		// do buffer e o char for diferente de fim de linha
		// ent�o usaremos ponteiro absoluto para escrever no arquivo.
		// Isso acontece poque desejamos continuar colocando coisa no arquivo
		// mesmo depois que o buffer se esgota.
		
		// caso contr�rio escreveremos no buffer.
			
			
		// Se ainda n�o esgotamos o buffer,
		// ou se esgotamos o buffer mas o caractere n�o � um 
		// caractere de fim de linha;

		// if ( stream->_w-- >= 0 || 
		//      ( stream->_w >= stream->_lbfsize && (char) ch != '\n' ) )

		
		sprintf ( stream->_p, "%c", ch);
		stream->_p++;
		stream->_w++;
		stream->_cnt--;
		
    };


    return 0;
}


/*
 ********************************** 
 * fscanf:
 * 
 * 
 * 
 */
 
// (since C99)
// int fscanf( FILE *restrict stream, const char *restrict format, ... );
// (until C99)

int fscanf (FILE *stream, const char *format, ... )
{
    // #obs:
    // Existe um scanf completo em ring3.
    // Talvez n�o precisamos de outro aqui.

    printf ("fscanf: todo \n");
    return (int) -1;
}


/*
int vfprintf ( FILE *stream, const char *format, stdio_va_list argptr );
int vfprintf ( FILE *stream, const char *format, stdio_va_list argptr )
{
}
*/


/*
 * rewind
 * 
 * 
 */

void rewind ( FILE * stream )
{
    //fseek (stream, 0L, SEEK_SET);

	if ( (void *) stream == NULL )
		return;
		
    stream->_p = stdin->_base;
}


/*
 *************************************************************
 * printchar:
 *     Coloca o caractere na string ou imprime.
 * Essa fun��o chama uma rotina que dever� tratar o caractere e em seguida 
 * envi�-lo para a tela.
 * Essa rotina � chamada pelas fun��es: /print/printi/prints.
 */

static void printchar (char **str, int c)
{
	// #importante
	// Se a string existe colocamos nela,
	// caso contr�rio imprimimos no backbuffer.
	// Vamos aproveitar esse momento para ativarmos a
	// pintura no caso dos caraters enviados para uma 
	// stream de output, como stdout.

	// Ativaremos a rotina de mostrar na tela s� no momento em que 
	// encontramos um fim de linha.

    if (str)
    {
		if ( c == '\n' ) 
		{
			CurrentTTY->print_pending = 1;
		}
		
		**str = c;
		
		++(*str);
		
    }else (void) putchar (c);
}


/*
 ********************************************************************
 * putchar:
 *     Put a char on the screen. (libC).
 *     Essa rotina chama uma rotina de tratamento de caractes, somente
 * depois � que o caractere ser� enviado para a tela.
 *     Essa rotina � chamada pelas fun��es: /printchar/input/.
 */

int putchar (int ch){ 
   
    //Em kgws/comp/cedge.c
    outbyte (ch,current_vc );


    return (int) ch;    
}


/*
 ****************************
 * getchar:
 *    #todo: 
 *    Isso deve er oferecido como servi�o pelo kernel.
 *    The getchar function is equivalent to getc with stdin as 
 *    the value of the stream argument.
 */
/* 
int getchar()
{
	//terminal.h
	
	
	//erro
	if( teminalfeedCHStatus != 1 )
	{
		return (int) -1;
	}
	
	
done:
    teminalfeedCHStatus = 0;
	return (int) teminalfeedCH;
}
*/


/*
 essa fun��o � legal ... habilitar quando der.
 
void stdio_ClearToEndOfLine();
//limpa com caracteres em branco at� antes da posi��o do cursor.
void stdio_ClearToEndOfLine()
{
    unsigned u;
    unsigned long OldX, OldY;
    
    OldX = g_cursor_x;
	OldY = g_cursor_y;
	
	//de onde o cursor est� at� o fim da linha.
	for( u = g_cursor_x; u < g_cursor_right; u++ )
	{
       _outbyte(' ', current_vc);
    }
	
    g_cursor_x = OldX;
	g_cursor_y = OldY;

}
*/


/*
 essa fun��o � legal ... habilitar quando der.
 
void stdio_ClearToStartOfLine();
//limpa com caracteres em branco at� antes da posi��o do cursor.
void stdio_ClearToStartOfLine()
{
    unsigned u;
    unsigned long OldX, OldY;
    
    OldX = g_cursor_x;
	OldY = g_cursor_y;
	
	//In�cio da linha.
    g_cursor_x = 0;
	g_cursor_y = OldY;	
	
	//de onde o cursor est� at� o fim da linha.
	for( u = g_cursor_x; u < g_cursor_right; u++ )
	{
       _outbyte(' ',current_vc);
    }
	
    g_cursor_x = OldX;
	g_cursor_y = OldY;

}
*/



/*
 ******************************************************************
 * input:
 *     Coloca os caracteres digitados em um buffer, (string). 
 * Para depois comparar a string com outra string, que � um comando.
 * 
 *     Devemos nos certificar que input(.) n�o imprima nada.
 *
 * History:
 *     2015 - Created by Fred Nora.
 *     ...
 */

unsigned long input ( unsigned long ch ){

    int i;

	// Converte 'unsigned long' em 'char'.
    char c = (char) ch;


    if ( g_inputmode == INPUT_MODE_LINE )
    {
        //Limite.
	    if(prompt_pos >= PROMPT_SIZE)
	    {
	        printf ("input: INPUT_MODE_LINE full buffer!\n");
	        refresh_screen ();
			return (unsigned long) 0; 
	    };
    };


    if (g_inputmode == INPUT_MODE_MULTIPLE_LINES )
    {
		//tem que ter o tamanho de um arquivo.
		if (prompt_pos >= PROMPT_SIZE)
		{
	        printf("input: INPUT_MODE_MULTIPLE_LINES full buffer\n");
	        refresh_screen();
			return (unsigned long) 0; 
		}
    };


	// Trata o caractere digitado. 


    switch (c)
    {

	    //Enter.
		//+se for modo comando devemos finalizar com zero.
		//+se for modo texto, devemos apenas incluir os caracteres \r \n.
		//case 0x1C:
		case VK_RETURN:
            //modo linha 
			if(g_inputmode == INPUT_MODE_LINE )
			{
			    prompt[prompt_pos] = (char )'\0'; //end of line.
			    //@todo: ?? ldiscCompare();
				//o compare est� no aplicativo.
	            for(i=0; i<PROMPT_MAX_DEFAULT;i++)
	            {
		            prompt[i] = (char) '\0';
		            prompt_out[i] = (char) '\0';
		            prompt_err[i] = (char) '\0';
	            };
                prompt_pos = 0;
				goto input_done;
			};
            //modo multiplas linhas 
		    if (g_inputmode == INPUT_MODE_MULTIPLE_LINES )
		    {
			    prompt[prompt_pos] = (char )'\r';
                prompt_pos++;
				prompt[prompt_pos] = (char )'\n';
				prompt_pos++;
			};
		    break;

	    //Backspace.
		case 0x0E:
		
            if ( prompt_pos <= 0 )
            {
			    prompt_pos = 0;
				prompt[prompt_pos] = (char ) '\0';
				break; 
			}
		    
			//Apaga o anterior (no buffer).
			prompt_pos--;
			prompt[prompt_pos] = (char ) '\0';
			break;
			
		//...	
		
        //Para qualquer caractere que n�o sejam os especiais tratados acima.
		default:
		    prompt[prompt_pos] = c;
		    prompt_pos++;          //incrementa fila
			break;
    };


input_more:
    return 0;


input_done:
    return VK_RETURN;
}


/*
 *****************
 * stdioInitialize:
 *     Inicializando stdio pertencente ao kernel base.
 *     Inicializa as estruturas do fluxo padr�o.
 *     Quem chamou essa inicializa��o ?? Em que hora ??
 *
 * #bugbug: Pelo jeito somente depois dessa inicializa��o � que temos mensagens 
 * com printf decentes. Ent�o a inicializa��o do kernel precisa disso.
 * >> precisamos antecipar essa iniciliza��o. Mas ela precisa ser depois da
 * inicializa��o da pagina��o.
 */
 
int stdioInitialize (void){

    int Status = 0;
    int i;

	// Buffers para as estruturas.
    unsigned char *buffer0;
    unsigned char *buffer1;
    unsigned char *buffer2;

    int cWidth = get_char_width ();
    int cHeight = get_char_height ();


    if ( cWidth == 0 || cHeight == 0 )
    {
		panic ("stdioInitialize: Char info");
    }


	// #todo
	// podemos usar esse alocador ?? Ainda n�o ??
	
	//stdin = (void *) kmalloc( sizeof(FILE) );
	//stdout = (void *) kmalloc( sizeof(FILE) );
	//stderr = (void *) kmalloc( sizeof(FILE) );	
	

	// #bugbug:
	//  4KB alocados para cada estrutura. Isso � muito.
	//  Mas ao mesmo tempo estamos economizando o heap 
	//  usado pelo kmalloc.
	//  Podemos alocar 4KB para o buffer. 'prompt'
	
	
	// Alocando uma p�gina para cada buffer.
	// 4KB size.
	// #importante
	// Obs: Essas p�ginas s�o alocadas em user mode.
	
	
	//4KB
    buffer0 = (unsigned char *) newPage (); 
    if ( (unsigned char *) buffer0 == NULL )
    {
		printf ("buffer0\n");
        goto fail;
    }


	//4KB
    buffer1 = (unsigned char *) newPage ();
    if ( (unsigned char *) buffer1 == NULL )
    {
		printf ("buffer1\n");
        goto fail;
    }


	//4KB
    buffer2 = (unsigned char *) newPage ();
    if ( (unsigned char *) buffer2 == NULL )
    {
		printf ("buffer2\n");
        goto fail;
    }


	//
	// Alocando mem�ria para o fluxo padr�o do 
	// processo kernel.
	// Estamos apenas alocando mem�ria para a estrutura.
	//
	
	stdin = (FILE *) &buffer0[0];
	stdout = (FILE *) &buffer1[0];
	stderr = (FILE *) &buffer2[0];


    // Configurando a estrutura de stdin. 
	stdin->used = 1;
	stdin->magic = 1234;
	stdin->_base = &prompt[0];    //See: include/kernel/stdio.h
	stdin->_p =  &prompt[0];
	stdin->_bf._base = stdin->_base;
	stdin->_lbfsize = PROMPT_SIZE; //128; //#todo
	stdin->_r = 0;
	stdin->_w = 0;
	stdin->_cnt = PROMPT_SIZE;
	stdin->_file = 0;
	stdin->_tmpfname = "k-stdin";
	//...

    // Configurando a estrutura de stdout.
	stdout->used = 1;
	stdout->magic = 1234;
	stdout->_base = &prompt_out[0];  //See: include/kernel/stdio.h
	stdout->_p = &prompt_out[0];
	stdout->_bf._base = stdout->_base;
	stdout->_lbfsize = PROMPT_SIZE; //128; //#todo
	stdout->_r = 0;
	stdout->_w = 0;
	stdout->_cnt = PROMPT_SIZE;
	stdout->_file = 1;
	stdout->_tmpfname = "k-stdout";
	//...
	
    // Configurando a estrutura de stderr.
	stderr->used = 1;
	stderr->magic = 1234;
	stderr->_base = &prompt_err[0];  //See: include/kernel/stdio.h
	stderr->_p =  &prompt_err[0];
	stderr->_bf._base = stderr->_base;
	stderr->_lbfsize = PROMPT_SIZE; //128; //#todo
	stderr->_r = 0;
	stderr->_w = 0;
	stderr->_cnt = PROMPT_SIZE;
	stderr->_file = 2;
	stderr->_tmpfname = "k-stderr";
	//...


    // #importante
    // Salvando os ponteiros na lista de arquivos.

    Streams[__KERNEL_STREAM_STDIN] = (unsigned long) stdin;
    Streams[__KERNEL_STREAM_STDOUT] = (unsigned long) stdout;
    Streams[__KERNEL_STREAM_STDERR] = (unsigned long) stderr;

	//Os pr�ximos s�o inicializados em fs.c
	//Streams[3] volume0 root dir (vfs) 
	//Streams[4] volume1 root dir (boot volume)
	//Streams[5] volume2 root dir  (system volume)
	//...



    //Configurando o array global para ser o 
    //mesmo que o array local.	
	gStreams = (unsigned long *) &Streams[0];
	
	//N�mero de streams no array global
	g_nstream = NUMBER_OF_FILES;
	
	//
	// Flag para o tipo de input.
	// # Multiplas linhas.
	//
	
	g_inputmode = INPUT_MODE_MULTIPLE_LINES;
	
	
	//  ## Cursor ##
	
	// Inicializa o cursor com margens bem abertas.
	
	TTY[current_vc].cursor_left = 0;  //g_cursor_left = (0);
	TTY[current_vc].cursor_top = 0;  //g_cursor_top = (0); 

	//@todo:
	//Isso � complicado.
	//Temos que pegar esses valores.
	//g_cursor_width = ?;
	//g_cursor_height = ?;	
	
	//#bugbug 
	//#todo: precisamos as dimens�es do char com base na fonte antes de tudo.
	//?? Talvez aqui devamos inicializar a fonte e as dimens�es de char..
	//pois estamos na inicializa��o da libc. Mas isso ja pode ter acontecido na 
	//inicializa��o do kernel, quando era necess�rio algum tipo de fonte para 
	//mensagens de debug.
	//?? ser� que j� configuramos as dimens�es da tela. Devemos usar as vari�veis.
	//?? se as dimens�es da tela foram configuradas na inicializa��o,
	//ent�o podemos usar as vari�veis aqui.
	
	//precisamos saber as dimens�es da tela e do char.
	//g_cursor_right = (800/8);
	//g_cursor_bottom = (600/8);
	
	TTY[current_vc].cursor_right = (SavedX/cWidth);    //g_cursor_right = (SavedX/cWidth);
	TTY[current_vc].cursor_bottom = (SavedY/cHeight);  //g_cursor_bottom = (SavedY/cHeight);
	
	
	
    //x e y.
	TTY[current_vc].cursor_x = TTY[current_vc].cursor_left;  //g_cursor_x = g_cursor_left; 
	TTY[current_vc].cursor_y = TTY[current_vc].cursor_top;   //g_cursor_y = g_cursor_top; 


	// Default color.
	// N�o sabemos se o esquema de cores do sistema j�
	// est� configurado, ent�o usaremos uma cor padr�o.
	// A QUALQUER HORA O KERNEL PODE ESCREVER NO TERMINAL 
	// E PARA USARMOS JANELAS PRETAS TEMOS QUE CONFIGURA A 
	// COR DA FONTE, ENT�O JANELAS TER�O FONTE PRETA.

	TTY[current_vc].cursor_color = COLOR_TERMINALTEXT;    //g_cursor_color = COLOR_TERMINALTEXT;

	
	// #importante:
	// Preenche os arquivos do fluxo padr�o do kernel base
	// com 'zeros'.

    //See: include/kernel/stdio.h
    
    //for ( i=0; i<PROMPT_MAX_DEFAULT; i++ )
    for ( i=0; i<PROMPT_SIZE; i++ )
    {
		prompt[i] = (char) '\0';
		prompt_out[i] = (char) '\0';
		prompt_err[i] = (char) '\0';
    };


	// Inicializa o deslocamento dentro do arquivo de entrada.
	// #bugbug @todo: Poder�amos ter posicionamento dentro 
	// dos 3 arquivos. Mas isso reflete apenas o posicionamento 
	// dentro de stdin por enquanto.
	
    prompt_pos = 0;
	
	
	//
	// ## keyboard support ##
	//
	
	//fluxo padrao
	
	//#importante
	//usando o buffer keybuffer coko arquivo.
	//ele esta em gws/user.
	
	
	//
	// ## stdin
	//

    unsigned char *current_stdin_struct_buffer;
    unsigned char *current_stdin_data_buffer;

	current_stdin_struct_buffer = (unsigned char *) newPage ();
	current_stdin_data_buffer = (unsigned char *) newPage ();
	
	current_stdin = (FILE *) &current_stdin_struct_buffer[0];
	
	current_stdin->used = 1;
	current_stdin->magic = 1234;
	current_stdin->_base = (unsigned char *) &current_stdin_data_buffer[0];
	current_stdin->_bf._base = current_stdin->_base;
	current_stdin->_r = 0;
	current_stdin->_w = 0;
	current_stdin->_p = (unsigned char *) &current_stdin_data_buffer[0];
	current_stdin->_cnt = 128;  //Limitando. na verdade e' 4KB.
	current_stdin->_lbfsize = 128;
	//#todo: Colocar em Streams[i]
	
	//
	// ## stdout
	//
	
	unsigned char *current_stdout_struct_buffer;
	unsigned char *current_stdout_data_buffer;
	
	current_stdout_struct_buffer = (unsigned char *) newPage ();
	current_stdout_data_buffer = (unsigned char *) newPage ();	
	
	current_stdout = (FILE *) &current_stdout_struct_buffer[0];
	
	current_stdout->used = 1;
	current_stdout->magic = 1234;		
	current_stdout->_base = (unsigned char *) &current_stdout_data_buffer[0];
	current_stdout->_bf._base = current_stdout->_base;
	current_stdout->_r = 0;
	current_stdout->_w = 0;
	current_stdout->_p  = (unsigned char *) &current_stdout_data_buffer[0];
	current_stdout->_cnt = 128;  //Limitando. na verdade e' 4KB.
	current_stdout->_lbfsize = 128;
	//#todo: Colocar em Streams[i]
	
	//
	// ## stderr
	//	
	
	unsigned char *current_stderr_struct_buffer;
	unsigned char *current_stderr_data_buffer;
	
	current_stderr_struct_buffer = (unsigned char *) newPage();
	current_stderr_data_buffer = (unsigned char *) newPage ();
	
	current_stderr = (FILE *) &current_stderr_struct_buffer[0];
	
	current_stderr->used = 1;
	current_stderr->magic = 1234;
	current_stderr->_base = (unsigned char *) &current_stderr_data_buffer[0];
	current_stderr->_bf._base = current_stderr->_base;
	current_stderr->_r = 0;
	current_stderr->_w = 0;
	current_stderr->_p  = (unsigned char *) &current_stderr_data_buffer[0];
	current_stderr->_cnt = 128;  //Limitando. na verdade e' 4KB.
	current_stderr->_lbfsize = 128;
    //#todo: Colocar em Streams[i]
    
    
	// Done !

    return 0;

fail:
    panic ("stdio-stdioInitialize: fail\n");
}


/*
 *******************************************
 * REFRESH_STREAM:
 *     #IMPORTANTE
 *     REFRESH SOME GIVEN STREAM INTO TERMINAL CLIENT WINDOW !!
 */

void REFRESH_STREAM ( FILE *stream ){

    char *c;

	 //#debug
	 //sprintf ( stream->_base, "TESTING STDOUT ..." );

    int i;
    int j;

    j = 80*25;
 
    c = stream->_base;


    int cWidth = get_char_width ();
    int cHeight = get_char_height ();


    if ( cWidth == 0 || cHeight == 0 )
    {
		panic ("stdio-REFRESH_STREAM: char w h ");
    }


    // Seleciona o modo terminal.

    //++
    stdio_terminalmode_flag = 1;  
    for ( i=0; i<j; i++ )
    {
		printf ("%c", *c );
		
	    //refresh_rectangle ( g_cursor_x * cWidth, g_cursor_y * cHeight, 
		    //cWidth, cHeight );
		    
		    
	    refresh_rectangle ( 
	        TTY[current_vc].cursor_x * cWidth,     //g_cursor_x * cWidth, 
	        TTY[current_vc].cursor_y * cHeight,    //g_cursor_y * cHeight, 
		    cWidth, cHeight );
		
		c++;
    };
    stdio_terminalmode_flag = 0;  
    //--
}



/*
 * setbuf:
 * 
 */

// see: 
// https://linux.die.net/man/3/setvbuf

void setbuf (FILE *stream, char *buf){

    if ( (void *) stream == NULL )
    {
		return;
    }else{
	
		//#todo
		//se o buffer � v�lido.
        //if (stream->_bf._base != NULL) 
        //{
            //if (stream->cnt > 0)
                //fflush (stream);
                
            //free (stream->buf);
        //}
        
        // Udate stream.
        stream->_bf._base = buf;
        //stream->_lbfsize = size;        
        // ?? stream->bufmode = mode;

        stream->_p = buf;
        // ??stream->cnt = 0;
        //...
    };
}


/*
 * setbuffer:
 * 
 */
 
void setbuffer (FILE *stream, char *buf, size_t size){

    if ( (void *) stream == NULL )
    {
		return;
    }else{

		//#todo
		//se o buffer � v�lido.
        //if (stream->_bf._base != NULL) 
        //{
            //if (stream->cnt > 0)
                //fflush (stream);
                
            //free (stream->buf);
        //}
        
        // Udate stream.
        stream->_bf._base = buf;
        stream->_lbfsize = size;        
        // ?? stream->bufmode = mode;

        stream->_p = buf;
        // ??stream->cnt = 0;
        //...
    };
}


/*
 * setlinebuf:
 * 
 */
 
void setlinebuf (FILE *stream)
{
}


/*
 * setvbuf: 
 * 
 * 
 */

int setvbuf (FILE *stream, char *buf, int mode, size_t size){

    if ( (void *) stream == NULL )
    {
		return -1;
    }else{

		//#todo
		//se o buffer � v�lido.
        //if (stream->_bf._base != NULL) 
        //{
            //if (stream->cnt > 0)
                //fflush (stream);
                
            //free (stream->buf);
        //}
        
        // Udate stream.
        stream->_bf._base = buf;
        stream->_lbfsize = size;        
        // ?? stream->bufmode = mode;

        stream->_p = buf;
        // ??stream->cnt = 0;
        //...
    };

    return 0;
}


//
// End.
//

