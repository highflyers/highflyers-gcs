/*
 * streamer.cpp
 *
 *  Created on: 14 gru 2013
 *      Author: Marcin Kolny
 */

#include "video_streamer/server/RtpServer.h"

int main( int argc, char** argv )
{
	gst_init(&argc, &argv);
	GMainLoop* main_loop = g_main_loop_new ( nullptr, false );
	HighFlyers::RtpServer server;
	server.set_ip( "127.0.0.1" );
	server.set_port( 5000 );
	server.set_video_source( "ball", HighFlyers::RtpServer::VideoTest );
	server.set_caps( "video/x-raw,YUY2,width=640,height=480,framerate=24/1" );
	server.init_stream();
	server.start_stream();

	g_main_loop_run( main_loop );
	server.stop_stream();
	g_main_loop_unref( main_loop );

	return 0;
}
