#ifndef _HDHT11_PRIVATE_
#define _HDHT11_PRIVATE_

static void HDHT11_voidRequest(u8 DHT11_pin);
static void HDHT11_voidResponse(u8 DHT11_pin);
static u8 HDHT11_u8receiveData(u8 DHT11_pin);
static void HDHT11_voidTimeout(void);
static u8 HDHT11_u8Timeout = 0;


#endif
