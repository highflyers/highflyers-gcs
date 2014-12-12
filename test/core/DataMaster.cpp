/*
 * DataMaster.cpp
 *
 *  Created on: Dec 11, 2014
 *      Author: loganek
 */

#include <gtest/gtest.h>
#include "core/DataMaster.h"

using namespace HighFlyers;

class DataMasterTest : public ::testing::Test
{
protected:
	static std::shared_ptr<DataMaster> root;

	static void SetUpTestCase()
	{
		root = DataMaster::get_root();
	}

};

std::shared_ptr<DataMaster> DataMasterTest::root = std::shared_ptr<DataMaster>();

TEST_F(DataMasterTest, SimpleRegisterAndSettingValue)
{
	root->register_var<int>( "sample_var" );
	root->set_value<int>( "sample_var", 12 );
	ASSERT_EQ( 12, root->get_value<int>( "sample_var" ) );
}

TEST_F(DataMasterTest, ShouldThrowExceptionIfVarDoesNotExist)
{
	EXPECT_ANY_THROW( root->get_value<int>( "nonexistingvar" ) );
}

TEST_F(DataMasterTest, ShouldThrowExceptionBecauseOfInvalidType)
{
	EXPECT_ANY_THROW( root->get_value<float>( "sample_var" ) );
}

TEST_F(DataMasterTest, ShouldRegisterItemByPath)
{
	root->register_item( "this/is/sparta" );
	ASSERT_EQ( root->get_item( "this" )->get_item( "is" )->get_item( "sparta" ),
			root->get_item( "/this/is/sparta" ) );
}

TEST_F(DataMasterTest, ShouldThrowExceptionIfPathDoesNotExist)
{
	EXPECT_ANY_THROW( root->get_item( "this/is/non/existing/path" ) );
}

TEST_F(DataMasterTest, OneMoreSettingValueTest)
{
	root->register_item( "/very/complex/tree" )->register_var<std::string>( "variable" );
	root->get_item( "/very/complex/tree" )->set_value<std::string>( "variable", "test" );

	ASSERT_STREQ( "test", root->get_item( "/very/complex/tree" )->get_value<std::string>("variable").c_str() );
}

