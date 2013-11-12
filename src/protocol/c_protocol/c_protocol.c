/*!
	\file c_protocol.c
	C GCS protocol implementation.
*/

#include "c_protocol.h"
#include "crc16.h"
#include "nanopb/pb_encode.h"
#include "nanopb/pb_decode.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define U8_TO_U16( u8_msb, u8_lsb ) ( (uint16_t)(((uint16_t)u8_msb) << 8 | u8_lsb) )
#define BYTE_HIGH( u16 ) ( (uint8_t)((u16 & 0xFF00) >> 8) )
#define BYTE_LOW( u16 ) ( (uint8_t)(u16 & 0x00FF) )
#define NELEMS(tab) (sizeof(tab)/sizeof(tab[0]))

/*!
	\brief Calculate & add crc16 to serialized data buffer (two bytes before last).
	\param serialized_data Buffer with serialized GCS Frame.
	\param size Size of serialized_data.
*/
static void serialized_add_crc16( uint8_t *serialized_data, uint32_t size )
{

	assert( serialized_data != NULL );
	assert( size > 0 );

	uint16_t crc16 = crc16_ccitt( serialized_data, size - GCS_FRAME_STATIC_BYTES_FOLLOWING ); // ignore stop byte and 2 crc16 bytes

	// Endianness ?
	serialized_data[size-3] = BYTE_HIGH( crc16 );
	serialized_data[size-2] = BYTE_LOW( crc16 );

}

/*!
	\brief Serializes GCS Frame struct.
	\param frame Frame to serialize.
	\return Serialized GCS Frame.
*/
static uint8_t *serialize_frame( struct GCSFrame *frame )
{

	assert( frame != NULL );

	uint8_t *serialized_data = (uint8_t *)malloc( 2 + frame->bytes_following ); // 1B function code, 1B bytes following byte
	memset( serialized_data, 0x00, 2 + frame->bytes_following );

	serialized_data[0] = frame->function_code;
	serialized_data[1] = frame->bytes_following;

	if( frame->gcs_data != NULL )
	{
		pb_ostream_t data_stream = pb_ostream_from_buffer( serialized_data + 2, frame->bytes_following - GCS_FRAME_STATIC_BYTES_FOLLOWING );
		pb_encode( &data_stream, GCSData_fields, frame->gcs_data );
	}

	serialized_add_crc16( serialized_data, frame->bytes_following + 2 ); // 1B function code, 1B bytes following byte

	serialized_data[ frame->bytes_following+1 ] = frame->stop_byte;

	return serialized_data;

}

/*!
	\brief Deserializes received bytes (data) to GCS Struct.
	\param data Serialized GCS Struct.
	\return GCS Frame containing deserialized data.
*/
static struct GCSFrame *deserialize_toframe( uint8_t *data )
{

	assert( data != NULL );

	struct GCSFrame *rframe = (struct GCSFrame *)malloc( sizeof( struct GCSFrame ) );

	rframe->function_code = data[0];
	rframe->bytes_following = data[1];

	if( rframe->bytes_following > GCS_FRAME_STATIC_BYTES_FOLLOWING )
	{
		pb_istream_t stream = pb_istream_from_buffer( data + 2, rframe->bytes_following - GCS_FRAME_STATIC_BYTES_FOLLOWING );
		rframe->gcs_data = (GCSData *)malloc( sizeof(GCSData) );
		pb_decode( &stream, GCSData_fields, rframe->gcs_data );
	}
	else
	{
		rframe->gcs_data = NULL;
	}

	rframe->crc16 = U8_TO_U16( data[rframe->bytes_following-2], data[rframe->bytes_following-1] );
	rframe->stop_byte = data[rframe->bytes_following];

	return rframe;

}

/*!
	\brief Creates byte stream of serialized GCS Frame, based on function code (fcode)
			and GCSData struct.
	\param fcode GCS Function code.
	\param gcs_pbstruct Pointer to GCSData structure, or NULL if GCSData not used.
	\return Serialized GCS Frame.
*/
static uint8_t *create_bytestream( enum GCSFunctionCodes fcode, GCSData *gcs_pbstruct )
{

	struct GCSFrame f;
	f.function_code = fcode;

	if( gcs_pbstruct == NULL )
	{
		f.bytes_following = GCS_FRAME_STATIC_BYTES_FOLLOWING;
	}
	else
	{
		// Get size of serialized nanopb struct.
		pb_ostream_t sizestream;
		sizestream.max_size = 0;
		pb_encode( &sizestream, GCSData_fields, gcs_pbstruct );

		f.bytes_following = GCS_FRAME_STATIC_BYTES_FOLLOWING + sizestream.bytes_written;
	}

	f.gcs_data = gcs_pbstruct;
	f.stop_byte = GCS_FRAME_STOP_BYTE;

	uint8_t *serialized_data = serialize_frame( &f );

	return serialized_data;

}

uint8_t *create_req_battery_status( void )
{

	return create_bytestream( GCS_BATTERY_STATUS, NULL );

}

uint8_t *create_resp_battery_status( uint32_t battery_status )
{

	GCSData pbstruct;
	memset( &pbstruct, 0x00, sizeof(GCSData) );
	pbstruct.has_battery = true;
	pbstruct.battery = battery_status;

	return create_bytestream( GCS_BATTERY_STATUS, &pbstruct );

}

uint32_t decode_resp_battery_status( uint8_t *resp )
{

	assert( resp != NULL );

	struct GCSFrame *f = deserialize_toframe( resp );

	assert( f->gcs_data != NULL );
	uint32_t battery_status = f->gcs_data->battery;

	free( f->gcs_data );
	free( f );

	return battery_status;

}

uint8_t *create_req_PID_set( double P, double I, double D )
{

	GCSData PID_vars;
	memset( &PID_vars, 0x00, sizeof(GCSData) );
	PID_vars.has_PID = true;

	PID_vars.PID.has_P = true;
	PID_vars.PID.P = P;

	PID_vars.PID.has_I = true;
	PID_vars.PID.I = I;

	PID_vars.PID.has_D = true;
	PID_vars.PID.D = D;

	return create_bytestream( GCS_CONTROL_PID_SET, &PID_vars );

}

void decode_req_PID_set( uint8_t *req, double *P, double *I, double *D )
{

	assert( req != NULL && P != NULL && I != NULL && D != NULL );

	struct GCSFrame *f = deserialize_toframe( req );
	*P = f->gcs_data->PID.P;
	*I = f->gcs_data->PID.I;
	*D = f->gcs_data->PID.D;

	free( f->gcs_data );
	free( f );

}
