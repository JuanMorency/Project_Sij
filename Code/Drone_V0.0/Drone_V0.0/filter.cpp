/**
  ******************************************************************************
	* File Name         : filter.cpp
	* Description       : methods for filtering different input data
	* Author			: Juan Morency Trudel
	* Version           : 1.0.0
	* Date				: June 2017
  ******************************************************************************
  */

#include "filter.h"


/**
  * @brief  Digital filter that does that stores the last 8 elements and gives the mean
  * @param *pIndex: index of the element in the filter
  * @param *pAvgBuffer: value of the element in the filter
  * @param InVal:
  * @param pOutVal:
  * @retval None              
  */
void updateAvg8Filter(uint8_t *pIndex, int32_t *pAvgBuffer, int32_t InVal, int32_t *OutVal)
{
	uint8_t i;
	
	*(pAvgBuffer + ((*pIndex) ++)) = InVal; // increase index by one and insert value at that position
	*pIndex &= 0x07;	// if index is higher than 7, return index to 0
	
	
	//calculate the mean of all the values in the buffer
	*OutVal = 0;
	for(i = 0; i < 8; i ++)
	{
		*OutVal += *(pAvgBuffer + i);
	}
	*OutVal >>= 3;
}