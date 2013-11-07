#include <gtest/gtest.h>
#include "telemetry_controls/include/battery.h"

TEST(BatteryTest, CheckSetMaxValue)
{
	float expected = 42.f;
	Battery_Control battery;
	battery.set_maxvalue(expected);
	ASSERT_FLOAT_EQ(battery.get_maxvalue(), expected);
}
