add_subdirectory(nanopb)

add_library(c_protocol c_protocol.c crc16.c GCSData.pb.c)
