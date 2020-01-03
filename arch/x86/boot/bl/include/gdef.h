/*
 * gdef.h
 * 
 * Descri��o:
 *     Defini��es globais para o Boot Loader.
 *     Obs: Esse arquivo deve ficar no topo dos includes.
 *
 * Hist�rico:
 *     Vers�o 1.0, Oct 2016 - Created.
 */


int gdefTesting;

//+Objects.
//+Global structs.
//...


   // Essa flag ser� lida pela rotina de falta de p�gina.
   // para ignorar e n�o parar o sistema.
int ____testing_memory_size_flag;
unsigned long __last_valid_address; //salvando o �lltimo endere�o v�lido


//
//End.
//

