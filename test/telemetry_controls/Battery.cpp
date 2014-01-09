#include <gtest/gtest.h>
#include "telemetry_controls/Battery.h"

using namespace HighFlyers;

TEST( BatteryTest, CheckSetMaxValue )
{
	float expected = 42.f;
	BatteryWidget battery( 4, expected, 2 );
	ASSERT_FLOAT_EQ( battery.get_maxvalue(), expected );
}

TEST( BatteryTest, CheckReturn_percent )
{
	int expected = 0;

	for (int i=0; i < 90; i++)
	{
		BatteryWidget bat( i+11, i + 23, i );
		expected = (( bat.get_value() - bat.get_minvalue() ) * 100 / ( bat.get_maxvalue() - bat.get_minvalue() ) );

		ASSERT_EQ( bat.return_percent(), expected );
	}
}

TEST( BatteryTest, CheckReturn_percentForZeros )
{
	int expected = 0;
	BatteryWidget batt( 0, 0, 0 );

	ASSERT_EQ( batt.return_percent(), expected );
}

TEST( BatteryTest, CheckReturn_percentForBigNumber )
{
	BatteryWidget bat( 999999999998, 999999999999, 999999999997 );
	int expected = 50;

	ASSERT_EQ( bat.return_percent(), expected );
}

TEST( BatteryTest, CheckReturn_percentForWideRange )
{
	BatteryWidget bat( 0, 999999999999, -999999999999 );
	int expected = 50;
	ASSERT_EQ( bat.return_percent(), expected );

}

TEST( BatteryTest, CheckReturn_percentWithUpSideDownWideRange ) //check not for expected but ASSERT_THROW
{
	ASSERT_ANY_THROW( BatteryWidget bat( 0, -999999999999, 999999999999 ) );
}

TEST( BatteryTest, CheckReturn_percentForMaxLowerThanMin ) // check not for expected but ASSERT_THROW
{
	ASSERT_ANY_THROW( BatteryWidget bat( 0, -1, 1 ) );
}

TEST( BatteryTest, ChechkReturn_percentForZeroMaxMinAndDiffValue )
{
	BatteryWidget bat( 1, 0, 0 );
	int expected = 0;
	ASSERT_EQ( bat.return_percent(), expected );
}


TEST( BatteryTest, CheckReturn_percentForNegativeNumbers )
{
	BatteryWidget bat( -4, -3, -10 );
	float expected = ( 600/7 );
	ASSERT_NEAR( bat.return_percent(), expected, 0.0001 );
}

TEST( BatteryTest, CheckReturn_percentForWrongNegativeNumbers )
{
	ASSERT_ANY_THROW( BatteryWidget bat( 1, -10, -2 ) );
}
