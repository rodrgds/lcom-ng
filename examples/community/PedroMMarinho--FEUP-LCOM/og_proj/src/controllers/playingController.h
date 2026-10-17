#pragma once

#include "lcom/lcf.h"
#include "../States/states.h"
#include "../model/table.h"

/**
 * @brief Handles the playing controller
 * 
 * @param table Table to be handled
 * @param interruptType Type of interrupt
 * @param packet Packet to be handled
 * @param scanCode Scan code to be handled
 * @param elapsed Time elapsed
 * @return STATE Next state
*/
STATE playingControllerHandle(Table* table, DEVICE interruptType, const struct packet *packet, uint8_t scanCode, unsigned elapsed);

void resetRound(Table * table);
