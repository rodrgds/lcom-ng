#pragma once
#define DEBUG 0

/** @file vector.h
 *  @brief Vector generico em C: buffer contiguo de elementos de tamanho fixo data_size.
 */

typedef struct vector Vector;

/** @brief Cria um vector vazio com capacidade inicial minima.
 *  @param[in] data_size Tamanho em bytes de cada elemento.
 *  @param[in] freeFunction Funcao chamada ao remover/libertar elementos (ou NULL se valores trivialmente copiaveis).
 *  @return Ponteiro para Vector novo.
 */
Vector* vectorCreate(unsigned int data_size, void (*freeFunction)(void*));

/** @brief Cria um vector preenchido com valores variadicos (int ou long long conforme data_size).
 *  @param[in] data_size Tamanho de cada elemento (4 ou 8 suportados na implementacao).
 *  @param[in] freeFunction Libertador por elemento ou NULL.
 *  @param[in] size Numero de elementos iniciais.
 *  @param[in] ... Valores a empilhar.
 *  @return Vector inicializado com size elementos.
 */
Vector* vectorCreateWithData(unsigned int data_size, void (*freeFunction)(void*), unsigned int size, ...);

/** @brief Cria um vector preenchido com floats ou doubles (promovidos) conforme data_size.
 *  @param[in] data_size Tamanho de cada elemento (4 para float, 8 para double na implementacao).
 *  @param[in] freeFunction Libertador ou NULL.
 *  @param[in] size Numero de elementos.
 *  @param[in] ... Valores em virgula flutuante.
 *  @return Vector inicializado.
 */
Vector* vectorCreateWithFloatingData(unsigned int data_size, void (*freeFunction)(void*), unsigned int size, ...);

/** @brief Liberta todos os elementos (se freeFunction) e o buffer interno.
 *  @param[in] v Vector a libertar.
 */
void vectorFree(Vector* v);

/** @brief Numero de elementos no vector.
 *  @param[in] v Vector valido.
 *  @return Tamanho logico (size).
 */
unsigned int vectorGetSize(Vector* v);

/** @brief Tamanho em bytes de cada elemento.
 *  @param[in] v Vector valido.
 *  @return data_size configurado na criacao.
 */
unsigned int vectorGetDataSize(Vector* v);

/** @brief Redimensiona o vector; se encolher, pode libertar elementos removidos via freeFunction.
 *  @param[in,out] vec Vector.
 *  @param[in] size Novo numero de elementos.
 */
void vectorResize(Vector* vec, unsigned int size);

/** @brief Adiciona uma copia binaria de val no fim (val aponta para data_size bytes).
 *  @param[in,out] vec Vector.
 *  @param[in] val Ponteiro para os bytes do elemento a copiar.
 */
void vectorPushBack(Vector* vec, char* val);

/** @brief Adiciona no fim um valor inteiro copiado para data_size bytes.
 *  @param[in,out] vec Vector.
 *  @param[in] val Valor numerico a copiar para o slot novo.
 */
void vectorPushBackVal(Vector* vec, long long int val);

/** @brief Insere uma copia de val na posicao pos, deslocando elementos a seguir.
 *  @param[in,out] vec Vector.
 *  @param[in] pos Indice onde inserir (0 .. size).
 *  @param[in] val Bytes do elemento a inserir.
 */
void vectorInsert(Vector* vec, unsigned int pos, char* val);

/** @brief Remove o ultimo elemento (aplica freeFunction se existir).
 *  @param[in,out] vec Vector.
 */
void vectorPopBack(Vector* vec);

/** @brief Remove o elemento no indice pos e compacta o buffer.
 *  @param[in,out] vec Vector.
 *  @param[in] pos Indice valido (< size).
 */
void vectorRemove(Vector* vec, unsigned int pos);

/** @brief Substitui o elemento em pos por uma copia de val (liberta o antigo se freeFunction).
 *  @param[in,out] vec Vector.
 *  @param[in] pos Indice.
 *  @param[in] val Novos bytes do elemento.
 */
void vectorSet(Vector* vec, unsigned int pos, char* val);

/** @brief Escreve um valor inteiro na posicao pos (memcpy de data_size bytes).
 *  @param[in,out] vec Vector.
 *  @param[in] pos Indice.
 *  @param[in] val Valor a escrever.
 */
void vectorSetVal(Vector* vec, unsigned int pos, long long int val);

/** @brief Ponteiro para o elemento na posicao pos (vista no buffer interno; nao aloca copia).
 *  @param[in] vec Vector.
 *  @param[in] pos Indice.
 *  @return Endereco dentro de vec->data; valido ate o vector ser modificado.
 */
char* vectorGet(Vector* vec, unsigned int pos);

/** @brief Igual a vectorGet no ultimo elemento.
 *  @param[in] vec Vector nao vazio.
 *  @return Ponteiro para o ultimo elemento.
 */
char* vectorGetBack(Vector* vec);

/** @brief Le o elemento em pos como valor inteiro (memcpy para long long).
 *  @param[in] vec Vector.
 *  @param[in] pos Indice.
 *  @return Valor lido dos data_size bytes.
 */
long long int vectorGetVal(Vector* vec, unsigned int pos);

/** @brief Aloca e devolve uma copia binaria do elemento em pos (o caller deve free conforme uso).
 *  @param[in] vec Vector.
 *  @param[in] pos Indice.
 *  @return Ponteiro malloc com data_size bytes copiados.
 */
char* vectorGetCopy(Vector* vec, unsigned int pos);

/** @brief Procura a primeira ocorrencia de um elemento byte-a-byte igual a val.
 *  @param[in] vec Vector.
 *  @param[in] val Ponteiro para data_size bytes a comparar.
 *  @return Indice encontrado ou -1 se nao existir.
 */
int vectorFind(Vector* vec, char* val);
