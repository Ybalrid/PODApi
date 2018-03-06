#pragma once

//Important data types
enum { PODAPI_HELLO,
	   PODAPI_SEVERHERE,
	   PODAPI_DATA };
typedef unsigned char POD_Byte;
;
typedef float POD_Float;
typedef unsigned long long POD_Time;
typedef char POD_Char;

#ifdef __cplusplus
extern "C" {
#endif

//Remove any padding in the structures
#pragma pack(push, 1)

struct POD_message
{
	POD_Byte packetType;
	POD_Char text[128];
};

struct POD_ServerOk
{
	POD_Byte packetType;
};

struct POD_Sample
{
	POD_Byte packetType;
	POD_Time timecode;
	POD_Float posX, posY, accX, accY, accZ;
};

#pragma pack(pop)

#ifdef __cplusplus
} //extern "C" linkage
#endif
