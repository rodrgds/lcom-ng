#ifndef _FILE_H_
#define _FILE_H_
/** @defgroup File File
 * @{
 *
 * Funções para analisar e escrever nos ficheiros.
 */

/**
 * @brief Lista ligada usada para guardar a informação sobre os varios users
 * @param name - string que guarda o nome do user
 * @param score - string que guarda o score do user
 * @param date - string que guarda a date da sessão do user
 */
typedef struct lligada {
    char name[3];
    char score[3];
    char date[8];
    struct lligada *next;
} *Users;

/**
 * @brief Inicia a struct Users alocando a memória necessária e atribuindo valores a todos os parametros
 * @return Devolve a struct User
 */
Users usersInit();

//void printUsers(Users users);

/**
 * @brief Função que inicializa e adiciona um user a lista ligada de users por ordem decrescente dos scores
 * @param users - struct que guarda a informação sobre os varios users
 * @param name - nome do utilizador
 * @param date - data da partida
 * @param score - pontuação do utilizador
 */
void addUser(Users users, char* name, char* date, char* score);

/**
 * @brief Liberta o espaço de memória préviamnete ocupado pela struct users
 * @param users - struct cuja informação sera apagada
 */
void freeUsers(Users users);

/**
 * @brief Escreve num ficheiro a informação guardada na struct users
 * @param users - struct que guarda a informação sobre os varios users
 * @param file - path para o ficheiro
 */
void write_to_file(Users users, char* file);

/**
 * @brief Lê de um ficheiro a informação guardada na struct users
 * @param users - struct que guarda a informação sobre os varios users
 * @param file - path para o ficheiro
 */
void read_from_file(Users users, char* file);

/**
 * @brief Passa o score do user que e guardado no ficheiro como uma string para um numero inteiro
 * @param score - string que guarda o score do user
 */
int scoreToInt(char* score);

/**
 * @brief Passa o score do user para uma string para que possa ser guardado no ficheiro
 * @param score - string que guarda o score do user
 * @param valor - int onde esta guardado o valor do score do user
 */
void scoreIntToString(char* score, int valor);

/**
 * @brief Passa a data da sessão do user para uma string para que possa ser guardada no ficheiro
 * @param date - string que guarda o score do user
 * @param dia - dia da data
 * @param mes - mes da data
 * @param ano - ano da data
 */
void dateIntToString(char* date, int dia, int mes, int ano);

/**@}*/
#endif
