/*
 * File: ke.h
 *     Kernel External functions support.
 *     O objetivo � chamar rotinas que est�o fora do kernel base.
 * As rotinas chamadas podem estar em servidores em user mode ou
 * em m�dulos em kernel mode.
 *
 * Hist�rico:
 *     Oct 2016 - Created.
 */


/*
 * Servers and Drivers:
 * ====================
 *
 * Chamadas aos m�dulos externos que est�o em user mode.
 * S�o servers e drivers.
 * O Kernel pode cham�-los para uso pr�prio ou 
 * pode cham�-los para atender alguma solicita��o
 * de cliente, que tamb�m est� em user mode.
 * O Cliente chama solicida algum servi�o de um Server
 * atrav�z do kernel.
 *
 * @todo: Definir melhor o que deve ir para user modr e organizar.
 * por servidor por exemplo.
 */
 
 
//Informa��es sobre o kernel. m�dulo externo.
void KeInformation (void);   




//
// End.
//

