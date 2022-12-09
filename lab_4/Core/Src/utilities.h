/*
 * utilities.h
 *
 *  Created on: Nov 27, 2022
 *      Author: axeand
 */

#ifndef INC_UTILITIES_H_
#define INC_UTILITIES_H_

float soundLevel(uint16_t* buffer, uint16_t len, uint16_t stride);
float averageSamples(uint16_t * buffer, uint16_t len, uint16_t stride);

#endif /* INC_UTILITIES_H_ */
