/*
 * utilities.c
 *
 *  Created on: Nov 27, 2022
 *      Author: axeand
 */

#include <math.h>
#include "main.h"

float soundLevel(uint16_t* buffer, uint16_t len, uint16_t stride)
{
	if (len <= 0)
		return 0.0;
	int32_t sum = 0.0;
	int64_t sq_sum = 0.0;
	int16_t size = len*stride;
	for (int i = 0; i < size; i += stride)
	{
		sum += buffer[i];
		sq_sum += buffer[i]*buffer[i];
	}

	float mean = ((float)sum) / len;
	float variance = ((float)sq_sum) / len - mean * mean;

	float rms = sqrt(variance);

	return 20*log10(rms/2048.0);
}

float averageSamples(uint16_t * buffer, uint16_t len, uint16_t stride)
{
	if (len <= 0)
		return 0.0;
	int32_t sum = 0.0;
	int16_t size = len*stride;
	for (int i = 0; i < size; i += stride)
	{
		sum += buffer[i];
	}

	float mean = ((float)sum) / len;

	return mean;
}
