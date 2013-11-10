/*!
	\file c_protocol.h
	C GCS protocol interface.
*/

#ifndef __GCS_PROTOCOL_FUNCTIONS_H__
#define __GCS_PROTOCOL_FUNCTIONS_H__

#include "GCSData.pb.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GCS_FRAME_STOP_BYTE 0xAC //!< Last byte in serialized GCS Frame.
#define GCS_FRAME_STATIC_BYTES_FOLLOWING 3 //!< Min bytes_following in frame - 3B: 2B crc16, 1B stop byte

enum GCSFunctionCodes
{
	/* ... */
	GCS_BATTERY_STATUS = 0,
	GCS_CONTROL_PID_SET = 1,
	GCS_ACK,
	GCS_LAST_FC
};

/*!
	\brief GCS protocol frame structure.
*/
struct GCSFrame
{

	uint8_t function_code;
	uint8_t bytes_following; // u16/32 (?)

	GCSData *gcs_data; // nanopb struct

	uint16_t crc16;
	uint8_t stop_byte;

};

/*!
	\brief Creates serialized battery status request.
	\return Serialized battery status request.
*/
uint8_t *create_req_battery_status( void );

/*!
	\brief Creates serialized battery status response to request.
	\param battery_status Status of the battery.
	\return Serialized battery status response.
*/
uint8_t *create_resp_battery_status( uint32_t battery_status );

/*!
	\brief Decodes battery status response.
	\param resp Serialized GCS Frame containing battery status response.
	\return Battery status.
*/
uint32_t decode_resp_battery_status( uint8_t *resp );

/*!
	\brief Creates request for setting PID controller's params.
	\param P,I,D PID controller's params.
	\return Serialized PID set request.
*/
uint8_t *create_req_PID_set( double P, double I, double D );

/*!
	\brief Decodes PID set request.
	\param req Serialized GCS Frame containing PID set request.
	\param P,I,D Pointers to PID controller's params that will be set after request decoding.
*/
void decode_req_PID_set( uint8_t *req, double *P, double *I, double *D );

#ifdef __cplusplus
}
#endif

#endif
