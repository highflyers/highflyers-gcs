#include <gtest/gtest.h>
#include "telemetry_controls/Battery.h"

using namespace HighFlyers;

TEST( BatteryTest, CheckSetMaxValue )
{
	float expected = 42.f;
	BatteryWidget battery(4,6,2);
	battery.set_maxvalue( expected );
	ASSERT_FLOAT_EQ( battery.get_maxvalue(), expected );
}

TEST( BatteryTest, CheckReturn_percent )
{
	int expected = 0;
	BatteryWidget bat(4,6,2);

	for (int i=0; i < 90; i++)
	{
		bat.set_maxvalue( i + 23 );
		bat.set_minvalue( i );
		bat.set_voltage( i + 11 );
		expected = (( bat.get_value() - bat.get_minvalue() ) * 100 / ( bat.get_maxvalue() - bat.get_minvalue() ) );

		ASSERT_EQ( bat.return_percent(), expected );
	}
}

TEST( BatteryTest, CheckReturn_percentForZeros )
{
	int expected = 0;
	BatteryWidget batt(4,6,2);
	batt.set_maxvalue( 0 );
	batt.set_minvalue( 0 );
	batt.set_voltage( 0 );

	ASSERT_EQ( batt.return_percent(), expected );


}

TEST( BatteryTest, CheckReturn_percentForBigNumber )
{
	BatteryWidget bat(4,6,2);
	bat.set_maxvalue( 999999999999 );
	bat.set_minvalue( 999999999997 );
	bat.set_voltage( 999999999998 );
	int expected = 50;

	ASSERT_EQ( bat.return_percent(), expected );
}

TEST( BatteryTest, CheckReturn_percentForWideRange )
{
	BatteryWidget bat(4,6,2);
	bat.set_maxvalue( 999999999999 );
	bat.set_minvalue( -999999999999 );
	bat.set_voltage( 0 );
	int expected = 50;
	ASSERT_EQ( bat.return_percent(), expected );

}

TEST( BatteryTest, CheckReturn_percentWithUpSideDownWideRange ) //check not for expected but ASSERT_THROW
{
	BatteryWidget bat(4,6,2);
	bat.set_maxvalue( -999999999999 );
	bat.set_minvalue( 999999999999 );
	bat.set_voltage( 0 );

	ASSERT_ANY_THROW( bat.return_percent() );
}

TEST( BatteryTest, CheckReturn_percentForMaxLowerThanMin ) // check not for expected but ASSERT_THROW
{
	BatteryWidget bat(4,6,2);
	bat.set_maxvalue( -1 );
	bat.set_minvalue( 1 );
	bat.set_voltage( 0 );

	ASSERT_ANY_THROW( bat.return_percent() );
}

TEST( BatteryTest, ChechkReturn_percentForZeroMaxMinAndDiffValue )
{
	BatteryWidget bat(4,6,2);
	bat.set_maxvalue( 0 );
	bat.set_minvalue( 0 );
	bat.set_voltage( 1 );
	int expected = 0;
	ASSERT_EQ( bat.return_percent(), expected );
}


TEST( BatteryTest, CheckReturn_percentForNegativeNumbers )
{
	BatteryWidget bat(4,6,2);
	bat.set_maxvalue( -3 );
	bat.set_minvalue( -10 );
	bat.set_voltage( -4 );
	float expected = ( 600/7 );
	ASSERT_EQ( bat.return_percent(), expected );
}

TEST( BatteryTest, CheckReturn_percentForWrongNegativeNumbers )
{
	BatteryWidget bat(4,6,2);
	bat.set_maxvalue( -10 );
	bat.set_minvalue( -2 );
	bat.set_voltage( 1 );
	ASSERT_ANY_THROW( bat.return_percent() );
}
