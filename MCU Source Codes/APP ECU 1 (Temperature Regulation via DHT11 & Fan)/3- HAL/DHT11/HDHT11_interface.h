#ifndef _HDHT11_INTERFACE_
#define _HDHT11_INTERFACE_

typedef enum
{
	HDHT11_WAITING,
	HDHT11_DATA_READY,
	HDHT11_TIMEOUT
}HDHT11_status;

typedef union
{
	u8 dataArray[5];

	struct
	{
		u8 humidityInt;
		u8 humidityDec;
		u8 temperatureInt;
		u8 temperatureDec;
		u8 checksum;
	}data;

}HDHT11_data;

STD_error_t HDHT11_stderrInit(u8 ARG_u8SensorPin);
HDHT11_status HDHT11_hdht11statusReadData(HDHT11_data *ARG_phdhtdataData, u8 ARG_u8SensorPin);




#endif
