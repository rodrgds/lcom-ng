#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>

/**
 * @brief Ativa as interrupções para o mouse.
 * @param bit_no Valor inicial do hook_id
 * @returns 1 se ocorreu um erro, 0 se correu tudo bem
*/
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Desativa as interrupções para o rato.
 * @returns 1 se ocorreu um erro, 0 se correu tudo bem
*/
int (mouse_unsubscribe_int)();

/**
 * @brief Lê o status atual do KBC para verificar se podemos ler os valores correspondentes ao mouse.
 * @param status Valor do status que é lido
 * @returns 1 se ocorreu um erro, 0 se correu tudo bem
*/
int (mouse_read_status)(uint8_t *status);

/**
 * @brief Lê um byte proveniente do mouse
 * @returns 1 se ocorreu um erro, 0 se correu tudo bem
*/
int (mouse_read_byte)();

/**
 * @brief Escreve um byte/comando para o mouse
 * @param command Comando/Byte a ser enviado para o mouse
 * @returns 1 se ocorreu um erro, 0 se correu tudo bem
*/
int (mouse_write_byte)(uint8_t command);

/**
 * @brief Escreve um comando na porta selecionada.
 * @param command  Comando/Byte a ser enviado para o mouse
 * @param port Porta onde vamos escrever o comando/byte
 * @returns 1 se ocorreu um erro, 0 se correu tudo bem
*/
int (mouse_write_command)(uint8_t command, int port);

/**
 * @brief Trata das interrupções que são recebidas relativas ao mouse
*/
void (mouse_ih)();

/**
 * @brief Insere um byte lido do mouse num array
*/
void (mouse_insert_byte)();

/**
 * @brief Insere os valores do array de bytes do mouse num mouse packet
*/
void (mouse_insert_in_packet)();

/**
 * @brief Converte os valores do byte para 2's complement no caso de este ser negativo
 * @param mouse_byte valor a ser convertido
 * @returns Valor convertido
*/
uint16_t (convert_to_2sComplement)(uint8_t mouse_byte);

/**
 * @brief Obtém o hook_id para ser usado noutros documentos
 * @returns Valor do hook_id
*/
int (get_mouse_hook_id)();

/**
 * @brief Obtém o indice do byte atual para que possa ser acedido noutros documentos
 * @returns Valor do indice do byte atual
*/
uint8_t (get__mouse_byte_index)();

/**
 * @brief Obtém o valor do byte atual (último lido) para que possa ser usado noutros documentos
 * @returns Valor do byte atual (último lido)
*/
uint8_t (get_current_byte)();

/**
 * @brief Obtém o mouse packet para que possa ser usado noutros documentos
 * @returns Valor do mouse packet
*/
struct packet (get_mouse_packet)();

/**
 * @brief Reseta (colcoca a zero) o valor do indíce do byte atual
*/
void (reset_byte_index)();

/**
 * @brief Limpa o pacote de dados do mouse
 */
void clear_mouse_packet();

#endif
