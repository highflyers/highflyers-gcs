/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.2.3 at Thu Nov  7 17:41:49 2013. */

#include "GCSData.pb.h"



const pb_field_t EulerAngles_t_fields[4] = {
    PB_FIELD2(  1, DOUBLE  , OPTIONAL, STATIC, FIRST, EulerAngles_t, roll, roll, 0),
    PB_FIELD2(  2, DOUBLE  , OPTIONAL, STATIC, OTHER, EulerAngles_t, pitch, roll, 0),
    PB_FIELD2(  3, DOUBLE  , OPTIONAL, STATIC, OTHER, EulerAngles_t, yaw, pitch, 0),
    PB_LAST_FIELD
};

const pb_field_t Axes_t_fields[4] = {
    PB_FIELD2(  4, DOUBLE  , OPTIONAL, STATIC, FIRST, Axes_t, x, x, 0),
    PB_FIELD2(  5, DOUBLE  , OPTIONAL, STATIC, OTHER, Axes_t, y, x, 0),
    PB_FIELD2(  6, DOUBLE  , OPTIONAL, STATIC, OTHER, Axes_t, z, y, 0),
    PB_LAST_FIELD
};

const pb_field_t GPS_t_fields[5] = {
    PB_FIELD2(  8, INT32   , OPTIONAL, STATIC, FIRST, GPS_t, localization, localization, 0),
    PB_FIELD2(  9, INT32   , OPTIONAL, STATIC, OTHER, GPS_t, direction, localization, 0),
    PB_FIELD2( 10, INT32   , OPTIONAL, STATIC, OTHER, GPS_t, speed, direction, 0),
    PB_FIELD2( 11, INT32   , OPTIONAL, STATIC, OTHER, GPS_t, height, speed, 0),
    PB_LAST_FIELD
};

const pb_field_t Control_t_fields[5] = {
    PB_FIELD2( 12, ENUM    , OPTIONAL, STATIC, FIRST, Control_t, go_direction, go_direction, 0),
    PB_FIELD2( 13, ENUM    , OPTIONAL, STATIC, OTHER, Control_t, rotate, go_direction, 0),
    PB_FIELD2( 14, BOOL    , OPTIONAL, STATIC, OTHER, Control_t, start, rotate, 0),
    PB_FIELD2( 15, BOOL    , OPTIONAL, STATIC, OTHER, Control_t, land, start, 0),
    PB_LAST_FIELD
};

const pb_field_t PID_t_fields[4] = {
    PB_FIELD2( 16, DOUBLE  , OPTIONAL, STATIC, FIRST, PID_t, P, P, 0),
    PB_FIELD2( 17, DOUBLE  , OPTIONAL, STATIC, OTHER, PID_t, I, P, 0),
    PB_FIELD2( 18, DOUBLE  , OPTIONAL, STATIC, OTHER, PID_t, D, I, 0),
    PB_LAST_FIELD
};

const pb_field_t GCSData_fields[8] = {
    PB_FIELD2( 19, MESSAGE , OPTIONAL, STATIC, FIRST, GCSData, EulerAngles, EulerAngles, &EulerAngles_t_fields),
    PB_FIELD2( 20, MESSAGE , OPTIONAL, STATIC, OTHER, GCSData, Axes, EulerAngles, &Axes_t_fields),
    PB_FIELD2( 21, MESSAGE , OPTIONAL, STATIC, OTHER, GCSData, GPS, Axes, &GPS_t_fields),
    PB_FIELD2( 22, MESSAGE , OPTIONAL, STATIC, OTHER, GCSData, Control, GPS, &Control_t_fields),
    PB_FIELD2( 23, MESSAGE , OPTIONAL, STATIC, OTHER, GCSData, PID, Control, &PID_t_fields),
    PB_FIELD2( 24, INT32   , REPEATED, CALLBACK, OTHER, GCSData, waypoint, PID, 0),
    PB_FIELD2( 25, INT32   , OPTIONAL, STATIC, OTHER, GCSData, battery, waypoint, 0),
    PB_LAST_FIELD
};


/* Check that field information fits in pb_field_t */
#if !defined(PB_FIELD_16BIT) && !defined(PB_FIELD_32BIT)
STATIC_ASSERT((pb_membersize(GCSData, EulerAngles) < 256 && pb_membersize(GCSData, Axes) < 256 && pb_membersize(GCSData, GPS) < 256 && pb_membersize(GCSData, Control) < 256 && pb_membersize(GCSData, PID) < 256), YOU_MUST_DEFINE_PB_FIELD_16BIT_FOR_MESSAGES_EulerAngles_t_Axes_t_GPS_t_Control_t_PID_t_GCSData)
#endif

#if !defined(PB_FIELD_32BIT)
STATIC_ASSERT((pb_membersize(GCSData, EulerAngles) < 65536 && pb_membersize(GCSData, Axes) < 65536 && pb_membersize(GCSData, GPS) < 65536 && pb_membersize(GCSData, Control) < 65536 && pb_membersize(GCSData, PID) < 65536), YOU_MUST_DEFINE_PB_FIELD_32BIT_FOR_MESSAGES_EulerAngles_t_Axes_t_GPS_t_Control_t_PID_t_GCSData)
#endif

/* On some platforms (such as AVR), double is really float.
 * These are not directly supported by nanopb, but see example_avr_double.
 * To get rid of this error, remove any double fields from your .proto.
 */
STATIC_ASSERT(sizeof(double) == 8, DOUBLE_MUST_BE_8_BYTES)

