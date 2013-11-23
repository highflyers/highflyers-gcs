#include <gtest/gtest.h>
#include "telemetry_controls/Battery.h"

using namespace HighFlyers;

TEST( BatteryTest, CheckSetMaxValue )
{
	float expected = 42.f;
	BatteryWidget battery;
	battery.set_maxvalue( expected );
	ASSERT_FLOAT_EQ( battery.get_maxvalue(), expected );
}

TEST( BatteryTest, CheckReturn_procent )
{
	int expected = 0;
	BatteryWidget bat;

	for (int i=0; i < 90; i++)
	{
		bat.set_maxvalue( i + 23 );
		bat.set_minvalue( i );
		bat.set_voltage( i + 11 );
		expected = (( bat.get_value() - bat.get_minvalue() ) * 100 / ( bat.get_maxvalue() - bat.get_minvalue() ) );

		ASSERT_EQ( bat.return_percent(), expected );
	}
}

TEST( BatteryTest, CheckReturn_procentForZeros )
{
	int expected = 0;
	BatteryWidget batt;
	batt.set_maxvalue( 0 );
	batt.set_minvalue( 0 );
	batt.set_voltage( 0 );

	ASSERT_EQ( batt.return_percent(), expected );


}
