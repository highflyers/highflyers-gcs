add_library(video_streamer_client SHARED
	VideoStreamer.cpp
	client/RtpClient.cpp
	client/RtpClientBuilder.cpp
)

add_library(video_streamer_server SHARED
	VideoStreamer.cpp
	server/RtpServer.cpp
	server/RtpServerBuilder.cpp
)