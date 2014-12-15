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

class DataMasterVarObserver : public DataMaster::VarObserver
{
public:
	bool changed = false;
	std::string name;

	void value_changed( IObservable<VarObserver>*, const std::string& var_name ) override
	{
		changed = true;
		name = var_name;
	}
};

class DataMasterItemObserver : public DataMasterObserver
{
public:
	bool changed = false;
	std::shared_ptr<DataMaster> node;

	void node_changed( IObservable<DataMasterObserver>*, std::shared_ptr<DataMaster> node ) override
	{
		changed = true;
		this->node = node;
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

TEST_F(DataMasterTest, ShouldProperlySubscribeAndUnsubscribeVariable)
{
	DataMasterVarObserver obs;
	const char* name = "test_var_name";

	auto path = root->register_item( "/some/path" );
	path->register_var<int>( name );
	path->subscribe_var(&obs, name );
	root->get_item( "/some/path" )->set_value( name, 12 );

	ASSERT_TRUE( obs.changed );
	ASSERT_STREQ( name, obs.name.c_str() );

	obs.changed = false;

	path->unsubscribe_var( &obs, name );
	path->set_value( name, 12 );

	ASSERT_FALSE( obs.changed );
}

TEST_F(DataMasterTest, ShouldProperlySubscribeAndUnsubscribeDataMaster)
{
	DataMasterItemObserver obs;

	auto path = root->register_item( "/quite/long/path" );
	root->get_item( "/quite/long" )->register_observer( &obs );

	path->register_var<int>( "dummy_name" );

	ASSERT_TRUE(obs.changed);
	ASSERT_EQ(root->get_item( "/quite/long"), obs.node);

	obs.changed = false;
	path->set_value( "dummy_name", 23 );

	ASSERT_TRUE(obs.changed);

	root->get_item( "/quite/long" )->unregister_observer( &obs );

	obs.changed = false;
	path->set_value( "dummy_name", 23 );

	ASSERT_FALSE(obs.changed);
}
