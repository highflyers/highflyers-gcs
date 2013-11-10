#include "Protocol.hpp"
#include "crc16.h"
#include "c_protocol.h"
#include <gtest/gtest.h>
#include <stdint.h>
#include <cstdlib>
#include <cstdio>

using namespace HighFlyers;

#define NELEMS(tab) (sizeof(tab)/sizeof(tab[0]))

TEST( Protocol, battery_status_requests )
{

	Protocol p;
	uint8_t *t = p.create_req_battery_status();

	uint8_t valid_battery_status_req[] = { GCS_BATTERY_STATUS, 0x03, 0x30, 0x63, 0xAC };

	ASSERT_EQ( NELEMS(valid_battery_status_req)-2, t[1] );

	for( uint32_t i = 0; i < NELEMS(valid_battery_status_req); ++i ) {
		EXPECT_EQ( valid_battery_status_req[i], t[i] );
	}

	uint8_t *resp_battery_status = p.create_resp_battery_status( 15 );

	EXPECT_EQ( 15, p.decode_resp_battery_status( resp_battery_status ) );

	free( resp_battery_status );
	free( t );

}

TEST( Protocol, PID_requests )
{

	Protocol pro;
	uint8_t *t = pro.create_req_PID_set( 5, 6, 7 );

	double P, I, D;
	pro.decode_req_PID_set( t, &P, &I, &D );

	EXPECT_EQ( 5, P );
	EXPECT_EQ( 6, I );
	EXPECT_EQ( 7, D );

	free( t );

}

