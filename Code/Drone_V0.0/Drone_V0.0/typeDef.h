/**
******************************************************************************
* File Name         : typeDef.h
* Description       : typeDefs used throughout the code. 
* Author			: Juan Morency Trudel
* Version           : 1.0.0
* Date				: June 2017
******************************************************************************
*/

#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

typedef struct
{
	int8_t X;
	int8_t Y;
	int8_t Z;
}XYZ8_TypeDef;

typedef struct
{
	int16_t X;
	int16_t Y;
	int16_t Z;
}XYZ16_TypeDef;

typedef struct
{
	float X;
	float Y;
	float Z;
}XYZfloat_TypeDef;

typedef struct
{
	uint32_t X;
	uint32_t Y;
	uint32_t Z;
}XYZ32_TypeDef;

typedef struct
{
	uint8_t Index;
	int32_t AvgBuffer[8];
}AvgTypeDef;

#endif /* _TYPEDEF_H_ */