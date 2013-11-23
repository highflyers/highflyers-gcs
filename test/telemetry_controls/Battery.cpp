#include <gtest/gtest.h>
#include "telemetry_controls/Battery.h"

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
		bat.set_value( i + 11 );
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
	batt.set_value( 0 );

	ASSERT_EQ( batt.return_percent(), expected );


}

TEST( BatteryTest, CheckReturn_procentForBigNumber )
{
	Battery_Control bat;
	bat.set_maxvalue( 999999999999 );
	bat.set_minvalue( 999999999997 );
	bat.set_value( 999999999998 );
	int expected = 50;

	ASSERT_EQ( bat.return_procent(), expected );
}

TEST( BatteryTest, CheckReturn_procentForWideRange )
{
	Battery_Control bat;
	bat.set_maxvalue( 999999999999 );
	bat.set_minvalue( -999999999999 );
	bat.set_value( 0 );
	int expected = 50;
	ASSERT_EQ( bat.return_procent(), expected );

}

TEST( BatteryTest, CheckReturn_procentWithUpSideDownWideRange )
{
	Battery_Control bat;
	bat.set_maxvalue( -999999999999 );
	bat.set_minvalue( 999999999999 );
	bat.set_value( 0 );
	int expected = 50;
	ASSERT_EQ( bat.return_procent(), expected );
}

TEST( BatteryTest, CheckReturn_procentForMaxLowerThanMin )
{
	Battery_Control bat;
	bat.set_maxvalue( -1 );
	bat.set_minvalue( 1 );
	bat.set_value( 0 );
	int expected = 50;
	ASSERT_EQ( bat.return_procent(), expected );
}

TEST( BatteryTest, ChechkReturn_procentForZeroMaxMinAndDiffValue )
{
	Battery_Control bat;
	bat.set_maxvalue( 0 );
	bat.set_minvalue( 0 );
	bat.set_value( 1 );
	int expected = 0;
	ASSERT_EQ( bat.return_procent(), expected );
}

TEST( BatteryTest, CheckReturn_procentForNegativeNumbers )
{
	Battery_Control bat;
	bat.set_maxvalue( -3 );
	bat.set_minvalue( -10 );
	bat.set_value( -4 );
	float expected = ( 600/7 );
	ASSERT_EQ( bat.return_procent(), expected );
}

TEST( BatteryTest, CheckReturn_procentForWrongNegativeNumbers )
{
	Battery_Control bat;
	bat.set_maxvalue( -10 );
	bat.set_minvalue( -2 );
	bat.set_value( 1 );
	float expected = -( 75/2 );
	ASSERT_EQ( bat.return_procent(), expected );
}









