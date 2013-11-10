/*!
	\file Protocol.hpp
	C++ wrapper for c_protocol library.
*/

#ifndef __PROTOCOL_HPP__
#define __PROTOCOL_HPP__

#include <stdint.h>

#include "c_protocol.h"

namespace HighFlyers {

	class Protocol {

		public:

			uint8_t *create_req_battery_status();

			uint8_t *create_resp_battery_status( uint32_t battery_status );
			uint32_t decode_resp_battery_status( uint8_t *resp );

			uint8_t *create_req_PID_set( double P, double I, double D );
			void decode_req_PID_set( uint8_t *req, double *P, double *I, double *D );


	};

}

#endif
