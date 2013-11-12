/*!
	\file Protocol.cpp
	C++ wrapper for c_protocol library.
*/

#include "Protocol.hpp"

uint8_t *HighFlyers::Protocol::create_req_battery_status()
{
	return ::create_req_battery_status();
}

uint8_t *HighFlyers::Protocol::create_resp_battery_status( uint32_t battery_status )
{
	return ::create_resp_battery_status( battery_status );
}

uint32_t HighFlyers::Protocol::decode_resp_battery_status( uint8_t *resp )
{
	return ::decode_resp_battery_status( resp );
}

uint8_t *HighFlyers::Protocol::create_req_PID_set( double P, double I, double D )
{
	return ::create_req_PID_set( P, I, D );
}

void HighFlyers::Protocol::decode_req_PID_set( uint8_t *req, double *P, double *I, double *D )
{
	::decode_req_PID_set( req, P, I, D );
}
