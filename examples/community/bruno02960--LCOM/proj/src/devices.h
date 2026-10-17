/**
 * @file devices.h
 * @author Bruno Pinto and Vitor Magalhaes
 * @brief Contains functions to make devices subscriptions and unsubscriptions
 */

#ifndef _DEVICES_H_
#define _DEVICES_H_

/** @defgroup devices devices
 * @{
 *
 * Functions to make devices subscriptions and unsubscriptions
 */

/*
 * @brief Makes the subscription of all the devices needed
 *
 * @return Returns 0 upon success, 1 otherwise
 */
int devices_subscriptions();

/*
 * @brief Makes the unsubscription of all the devices used
 *
 * @return Returns 0 upon success, 1 otherwise
 */
int devices_unsubscriptions();

/** @} end of devices */

#endif
