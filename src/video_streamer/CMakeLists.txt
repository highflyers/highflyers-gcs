add_library(video_streamer_client SHARED
	client/RtpClient.cpp
	client/RtpClientBuilder.cpp
)

add_library(video_streamer_server SHARED
	server/RtpServer.cpp
	server/RtpServerBuilder.cpp
)