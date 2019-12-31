/*
 * File: cedge.c 
 *
 * edge field for klibc.
 *
 * Extremidades das funções de impressão de caracteres ...
 * aqui ficará só o output de byte.
 */


#include <kernel.h>


/*
 ***************************************
 * _outbyte:
 *     @field 1
 *     Essa rotina efetivamente envia o caractere para a tela, não 
 * fazendo nenhum tipo de tratamento de caractere.
 *     Essa rotina é chamada pema rotina /outbyte/.
 *
 * Obs: 
 *     Um switch percebe o modo de vídeo e como esse caractere deve ser 
 * construído.
 * @todo: ?? Criar uma estrutura para parâmetros de vídeo. ??
 * 
 * #importante: Essa rotina de pintura deveria ser exclusiva para 
 * dentro do terminal. 
 */
 
// #importante
// Isso é rotina de terminal virtual.
 
void _outbyte (int c){
	
    unsigned long i;
	
	unsigned long x;
	unsigned long y;
	
    //O caractere.
	
	char ch = (char) c;	
	
	// #test
	// Tentando pegar as dimensões do char.
	// #importante: 
	// Não pode ser 0, pois poderíamos ter divisão por zero.
	
	
	int cWidth = get_char_width ();
	int cHeight = get_char_height ();
	
	if ( cWidth == 0 || cHeight == 0 )
	{
		//#debug
		debug_print ("_outbyte: char w h");
		printf ("_outbyte: fail w h ");
		die ();
	}
	
	// #bugbug
	// Caso estejamos em modo texto.
	// Isso ainda não é suportado.
	
	if ( VideoBlock.useGui == 0 )
	{
		debug_print ("_outbyte: kernel in text mode");
	    kprintf ("_outbyte: kernel in text mode");
		die ();
	}
	
	
	// #Importante: 
	// Essa rotina não sabe nada sobre janela, ela escreve na tela como 
	// um todo. Só está considerando as dimensões do 'char'.


    // Caso estivermos em modo gráfico.
	 
	if ( VideoBlock.useGui == 1 )
	{

        //#importante: Essa rotina de pintura deveria ser exclusiva para 
        //dentro do terminal.
        //então essa flag não faz sentido.		
 
		if ( stdio_terminalmode_flag == 1 )
		{
			
			// ## NÃO TRANPARENTE ##
            // se estamos no modo terminal então usaremos as cores 
            // configuradas na estrutura do terminal atual.
            // Branco no preto é um padrão para terminal.
			
            draw_char ( cWidth * TTY[current_vc].cursor_x, cHeight * TTY[current_vc].cursor_y, c, 
				COLOR_TERMINALTEXT, COLOR_TERMINAL2 );
			
		}else{
			
			// ## TRANSPARENTE ##
		    // se não estamos no modo terminal então usaremos
		    // char transparente.
            // Não sabemos o fundo. Vamos selecionar o foreground.
			
			drawchar_transparent ( 
			    cWidth* TTY[current_vc].cursor_x, cHeight * TTY[current_vc].cursor_y, 
				TTY[current_vc].cursor_color, c );
		};
		
		//#testando ...
		//g_cursor_x++;
     	
		//goto done;
        //return;
	};
}


/*
 *******************************************************************
 * outbyte:
 *     Trata o caractere a ser imprimido e chama a rotina /_outbyte/
 * para efetivamente colocar o caractere na tela.
 *
 * Essa rotina é chamada pelas funções: /putchar/scroll/.
 * @todo: Colocar no buffer de arquivo.
 */

// #importante
// Isso é rotina de terminal virtual.

void outbyte (int c){
	
	static char prev = 0;
	
	// Obs:
	// Podemos setar a posição do curso usando método,
	// simulando uma variável protegida.
	
//checkChar:        
      
    //Opção  
    //switch ?? 


    //form feed - Nova tela.
    if ( c == '\f' )
    {
        TTY[current_vc].cursor_y = TTY[current_vc].cursor_top;
        TTY[current_vc].cursor_x = TTY[current_vc].cursor_left;
        return;
    }

    // #m$. 
    // É normal \n retornar sem imprimir nada.
    
    //Início da próxima linha. 
    if ( c == '\n' && prev == '\r' ) 
    {
		//#todo: melhorar esse limite.
        if ( TTY[current_vc].cursor_y >= (TTY[current_vc].cursor_bottom-1) )
        {
            scroll ();

            TTY[current_vc].cursor_y = (TTY[current_vc].cursor_bottom-1);

            prev = c; 

        }else{

            TTY[current_vc].cursor_y++;
            TTY[current_vc].cursor_x = TTY[current_vc].cursor_left;
            prev = c;
        };

        return;
    };


    //Próxima linha no modo terminal.
    if ( c == '\n' && prev != '\r' ) 
    {
        if ( TTY[current_vc].cursor_y >= (TTY[current_vc].cursor_bottom-1) ){

            scroll ();
            
            TTY[current_vc].cursor_y = (TTY[current_vc].cursor_bottom-1);
            prev = c;

        }else{

            TTY[current_vc].cursor_y++;

			//Retornaremos mesmo assim ao início da linha 
			//se estivermos imprimindo no terminal.
            if ( stdio_terminalmode_flag == 1 )
            {
                TTY[current_vc].cursor_x = TTY[current_vc].cursor_left;	
            } 

			//verbose mode do kernel.
			//permite que a tela do kernel funcione igual a um 
			//terminal, imprimindo os printfs um abaixo do outro.
			//sempre reiniciando x.
            if ( stdio_verbosemode_flag == 1 )
            {
                TTY[current_vc].cursor_x = TTY[current_vc].cursor_left;
            } 

			//Obs: No caso estarmos imprimindo em um editor 
			//então não devemos voltar ao início da linha.

            prev = c;
        };

        return;
    };


	//tab
	//@todo: Criar a variável 'g_tab_size'.
    if( c == '\t' )  
    {
        TTY[current_vc].cursor_x += (8);
        //g_cursor_x += (4); 
        prev = c;
        
        return; 

		//Não adianta só avançar, tem que apagar o caminho até lá.
		
		//int tOffset;
		//tOffset = 8 - ( g_cursor_left % 8 );
		//while(tOffset--){
		//	_outbyte(' ');
		//}
		//Olha que coisa idiota, e se tOffset for 0.
		//set_up_cursor( g_cursor_x +tOffset, g_cursor_y );
		//return;        
    };


	//liberando esse limite.
	//permitindo os caracteres menores que 32.
	//if( c <  ' '  && c != '\r' && c != '\n' && c != '\t' && c != '\b' )
	//{
    //    return;
    //};
 

    //Apenas voltar ao início da linha.
    if( c == '\r' )
    {
        TTY[current_vc].cursor_x = TTY[current_vc].cursor_left;  
        prev = c;
        return;    
    }; 


    //#@todo#bugbug 
    //retirei esse retorno para o espaço, com isso 
    // o ascii 32 foi pintado, mas como todos os 
    //bits estão desligados, não pintou nada.
    //space 
    //if( c == ' ' )  
    //{
    //    g_cursor_x++; 
    //    prev = c;
    //    return;         
    //};
    
 
    //Delete. 
    //#bugbug: Limits.
    if ( c == 8 )  
    {
        TTY[current_vc].cursor_x--; 
        prev = c;
        return;
    };


	//
	// limits
	//

	// Filtra as dimensões da janela onde está pintando.
	// @todo: Esses limites precisam de variável global.
	//        mas estamos usando printf pra tudo.
	//        cada elemento terá que impor seu próprio limite.
	//        O processo shell impõe seu limite.
	//        a janela impõe seu limite etc...
	//        Esse aqui é o limite máximo de uma linha.
	// Poderia ser o limite imposto pela disciplina de linha
	// do kernel para o máximo de input. Pois o input é
	// armazenado em uma linha.	 


//checkLimits:

    //Limites para o número de caracteres numa linha.
    if ( TTY[current_vc].cursor_x >= (TTY[current_vc].cursor_right-1) )
    {
        TTY[current_vc].cursor_x = TTY[current_vc].cursor_left;
        TTY[current_vc].cursor_y++;  

    }else{   

		// Incrementando.
		// Apenas incrementa a coluna.

        TTY[current_vc].cursor_x++;  
    };

	// #bugbug
	// Tem um scroll logo acima que considera um valor
	// de limite diferente desse.

	// Número máximo de linhas. (8 pixels por linha.)
    if ( TTY[current_vc].cursor_y >= TTY[current_vc].cursor_bottom )  
    { 
        scroll ();

        TTY[current_vc].cursor_y = TTY[current_vc].cursor_bottom;
    };

	//
	// Imprime os caracteres normais.
	//

	// Nesse momento imprimiremos os caracteres.
    // Imprime os caracteres normais.
	// Atualisa o prev.

    _outbyte (c);

    prev = c;
}


