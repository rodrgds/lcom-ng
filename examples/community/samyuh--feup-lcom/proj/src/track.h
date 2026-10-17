#ifndef TRACK_H
#define TRACK_H

#include <lcom/lcf.h>
#include "images.h"
/** @defgroup track track
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

/**
 * @brief Struct to store Information about the Car's position
 */
typedef struct {
  int x; /*!< Car position in the x axis*/
  int y; /*!< Car position in the y axis*/
  int angle; /*!< Car direction*/
  int speed; /*!< Car speed to be added*/
  int lapnum; /*!< Car number of laps*/
  bool newlap; /*!< Car new lap status*/
  int state; /*!< Car current state of the lap*/
} CarPos;

/**
 * @brief Struct with Race's information
 */
typedef struct {
	int combo; /*!< Race combo for multiple correct digits*/
	bool nitro; /*!< Wether there is nitro available or not*/
  int nitroh; /*!< Height of nitro in nitro bar*/

	uint8_t key; /*!< RaceINfo key */
  int totalLaps; /*!< RaceINfo total number of laps */
	xpm_image_t dinamicImg; /*!< Current Letter image*/
	int countdown; /*!< Race countdown time */
	bool start; /*!< Wether Race has started*/
	int time; /*!< Race current time*/

	CarPos position;  /*!< Car Position*/
	void (*runTrack) (CarPos *); /*!< Track chosen to be run*/

	xpm_image_t track; /*!< Track image*/
} RaceInfo;

/**
 * @brief Creates a new Race (Race 1) from a given image
 * @param img Img of the race
 */
RaceInfo (newRace1)(xpm_image_t img);
/**
 * @brief Creates a new Race (Race 2) from a given image
 * @param img Img of the race
 */
RaceInfo (newRace2)(xpm_image_t img);

/**
 * @brief Creates a new Race (Race 3) from a given image
 * @param img Img of the race
 */
RaceInfo (newRace3)(xpm_image_t img);

/**
 * @brief Creates a new Race (Race 4) from a given image
 * @param img Img of the race
 */
RaceInfo (newRace4)(xpm_image_t img);

/**
 * @brief Creates a new Race (Race 5) from a given image
 * @param img Img of the race
 */
RaceInfo (newRace5)(xpm_image_t img);

/**
 * @brief Creates a new Race (Race 6) from a given image
 * @param img Img of the race
 */
RaceInfo (newRace6)(xpm_image_t img);

/**
 * @brief Creates a new Race (Race 7) from a given image
 * @param img Img of the race
 */
RaceInfo (newRace7)(xpm_image_t img);

/**
 * @brief Creates a new Race (Race 8) from a given image
 * @param img Img of the race
 */
RaceInfo (newRace8)(xpm_image_t img);

/**
 * @brief Executes the race with the image Track1 witha car
 * @param race CarPos pointer of the car that will be used at the race
 */
void (runTrack1)(CarPos *race);

/**
 * @brief Executes the race with the image Track2 witha car
 * @param race CarPos pointer of the car that will be used at the race
 */
void (runTrack2)(CarPos *race);

/**
 * @brief Executes the race with the image Track3 witha car
 * @param race CarPos pointer of the car that will be used at the race
 */
void (runTrack3)(CarPos *race);

/**
 * @brief Executes the race with the image Track4 witha car
 * @param race CarPos pointer of the car that will be used at the race
 */

void (runTrack4)(CarPos *race);

/**
 * @brief Executes the race with the image Track5 witha car
 * @param race CarPos pointer of the car that will be used at the race
 */
void (runTrack5)(CarPos *race);

/**
 * @brief Executes the race with the image Track6 witha car
 * @param race CarPos pointer of the car that will be used at the race
 */
void (runTrack6)(CarPos *race);

/**
 * @brief Executes the race with the image Track7 witha car
 * @param race CarPos pointer of the car that will be used at the race
 */
void (runTrack7)(CarPos *race);

/**
 * @brief Executes the race with the image Track8 witha car
 * @param race CarPos pointer of the car that will be used at the race
 */
void (runTrack8)(CarPos *race);


//Retas
/**
 * @brief Calculates the car position in a straight line to the bottom
 * @param race Carpos pointer to indicate the car's position and change it if needed
 * @param y_max Position where the car should stop
 * @param state State that will be moved to if end is reached
 */
int reta_baixo(CarPos *race , int y_max, int state);
/**
 * @brief Calculates the car position in a straight line to the top
 * @param race Carpos pointer to indicate the car's position and change it if needed
 * @param y_min Position where the car should stop
 * @param state State that will be moved to if end is reached
 */
int reta_cima(CarPos *race , int y_min, int state);
/**
 * @brief Calculates the car position in a straight line to the right
 * @param race Carpos pointer to indicate the car's position and change it if needed
 * @param x_max Position where the car should stop
 * @param state State that will be moved to if end is reached
 */
int reta_direita(CarPos *race , int x_max, int state);
/**
 * @brief Calculates the car position in a straight line to the left
 * @param race Carpos pointer to indicate the car's position and change it if needed
 * @param x_min Position where the car should stop
 * @param state State that will be moved to if end is reached
 */
int reta_esquerda(CarPos *race , int x_min, int state);

//Curvas
/**
 * @brief Calculates the position of race->y during a curve
 * @param car_x current car position in the x axis
 * @param x center of the curve's position in the x axis
 * @param y centre of the curve's position in the y axis
 * @param r radius of the curve
 * @param pos if 1, curve will make y value increase, if -1 it will decrease
 */
int curva(int car_x, int x, int y, int r, int pos);
/**
 * @brief Calculates the car position during a curve from the bottom to the right
 * @param race Carpos pointer to indicate the car's position and change it if needed
 * @param x_centro center of the curve's position in the x axis
 * @param y_centro center of the curve's position in the y axis
 * @param raio radius of the curve
 * @param state State that will be moved to if end is reached
 */
int curva_baixo_direita(CarPos *race , int x_centro, int y_centro, int raio, int state);
/**
 * @brief Calculates the car position during a curve from the right to the top
 * @param race Carpos pointer to indicate the car's position and change it if needed
 * @param x_centro center of the curve's position in the x axis
 * @param y_centro center of the curve's position in the y axis
 * @param raio radius of the curve
 * @param state State that will be moved to if end is reached
 */
int curva_direita_cima(CarPos *race , int x_centro, int y_centro, int raio, int state);
/**
 * @brief Calculates the car position during a curve from the top to the left
 * @param race Carpos pointer to indicate the car's position and change it if needed
 * @param x_centro center of the curve's position in the x axis
 * @param y_centro center of the curve's position in the y axis
 * @param raio radius of the curve
 * @param state State that will be moved to if end is reached
 */
int curva_cima_esquerda(CarPos *race , int x_centro, int y_centro, int raio, int state);
/**
 * @brief Calculates the car position during a curve from the left to the bottom
 * @param race Carpos pointer to indicate the car's position and change it if needed
 * @param x_centro center of the curve's position in the x axis
 * @param y_centro center of the curve's position in the y axis
 * @param raio radius of the curve
 * @param state State that will be moved to if end is reached
 */
int curva_esquerda_baixo(CarPos *race , int x_centro, int y_centro, int raio, int state);
/**
 * @brief Calculates the car position during a curve from the right to the bottom
 * @param race Carpos pointer to indicate the car's position and change it if needed
 * @param x_centro center of the curve's position in the x axis
 * @param y_centro center of the curve's position in the y axis
 * @param raio radius of the curve
 * @param state State that will be moved to if end is reached
 */
int curva_direita_baixo(CarPos *race , int x_centro, int y_centro, int raio, int state);
/**
 * @brief Calculates the car position during a curve from the left to the top
 * @param race Carpos pointer to indicate the car's position and change it if needed
 * @param x_centro center of the curve's position in the x axis
 * @param y_centro center of the curve's position in the y axis
 * @param raio radius of the curve
 * @param state State that will be moved to if end is reached
 */
int curva_esquerda_cima(CarPos *race , int x_centro, int y_centro, int raio, int state);
/**
 * @brief Calculates the car position during a curve from the top to the right
 * @param race Carpos pointer to indicate the car's position and change it if needed
 * @param x_centro center of the curve's position in the x axis
 * @param y_centro center of the curve's position in the y axis
 * @param raio radius of the curve
 * @param state State that will be moved to if end is reached
 */
int curva_cima_direita(CarPos *race , int x_centro, int y_centro, int raio, int state);

#endif  /* */
