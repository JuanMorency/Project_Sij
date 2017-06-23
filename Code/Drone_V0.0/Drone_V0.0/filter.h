/**
  ******************************************************************************
	* File Name         : filter.h
	* Description       : header for filter.cpp
	* Author			: Juan Morency Trudel
	* Version           : 1.0.0
	* Date				: June 2017
  ******************************************************************************
  */

#ifndef FILTER_H_
#define FILTER_H_

#include <stdio.h>

void updateAvg8Filter(uint8_t *pIndex, int32_t *pAvgBuffer, int32_t InVal, int32_t *OutVal);


#endif /* FILTER_H_ */