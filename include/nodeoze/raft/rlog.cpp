#include <nodeoze/raft/log.h>
#include <nodeoze/test.h>
#include <experimental/type_traits>
#include <thread>
#include <chrono>

using namespace nodeoze;
using namespace raft;

#if 0

TEST_CASE("nodeoze/smoke/raft/basic")
{
	{
		std::error_code ec;
		raft::log oak(1, "logfile.log", "logfile.tmp");
		oak.initialize( 1, 1, true, ec);
		CHECK( ! ec );
		oak.update_replicant_state( raft::replicant_state{ 1, 2, 3 }, ec );
		CHECK( ! ec );
		oak.close( ec );
		CHECK( ! ec );
	}
	{
		std::error_code ec;
		raft::log oak(1, "logfile.log", "logfile.tmp");
		oak.initialize( 1, 1, false, ec);
		CHECK( ! ec );
		CHECK( oak.current_replicant_state().term() == 2 );
		CHECK( oak.current_replicant_state().vote() == 3 );
		oak.close( ec );
		CHECK( ! ec );
	}
}

TEST_CASE("nodeoze/smoke/raft/payload")
{
//	std::this_thread::sleep_for (std::chrono::seconds(10));
	{
		std::error_code ec;
		raft::log oak(1, "logfile.log", "logfile.tmp");
		oak.initialize( 1,  1, true, ec);
		CHECK( ! ec );
		oak.update_replicant_state( raft::replicant_state{ 1, 1, 0 }, ec );
		CHECK( ! ec );

		auto p1 = std::make_unique< raft::state_machine_update >( 1, 1, buffer{ "some update payload" } );
		oak.append( std::move( p1 ), ec );
		CHECK( ! ec );

		auto p2 = std::make_unique< raft::state_machine_update >( 1, 2, buffer{ "some more update payload" } );
		oak.append( std::move( p2 ), ec );
		CHECK( ! ec );
		
		auto p3 = std::make_unique< raft::state_machine_update >( 1, 3, buffer{ "even more update payload" } );
		oak.append( std::move( p3 ), ec );
		CHECK( ! ec );

		oak.close( ec );
		CHECK( ! ec );
	}
	{
		std::error_code ec;
		raft::log oak(1, "logfile.log", "logfile.tmp");
		oak.initialize( 1, 1, false, ec);
		CHECK( ! ec );
		CHECK( oak.current_replicant_state().term() == 1 );
		CHECK( oak.current_replicant_state().vote() == 0 );
		CHECK( oak.size() == 3 );
		CHECK( oak.back().get_type() == raft::frame_type::state_machine_update_frame );
		auto index = oak.back().as< raft::state_machine_update >().index();
		CHECK( index == 3 );
		buffer load = oak.back().as< raft::state_machine_update >().payload();
		CHECK( load.to_string() == "even more update payload" );
		std::cout << "back index is " << index << ", payload:" << std::endl;
		load.dump( std::cout ); std::cout.flush();
		oak.close( ec );
		CHECK( ! ec );
	}

	{
		std::error_code ec;
		raft::log oak(1, "logfile.log", "logfile.tmp");
		oak.initialize( 1, 1, false, ec);
		CHECK( ! ec );

		oak.prune_front( 2, ec );
		CHECK( ! ec );

		auto fidx = oak.front().index();
		CHECK( fidx == 2 );

		CHECK( oak.size() == 2 );

		oak.prune_back( 2, ec );
		CHECK( ! ec );

		CHECK( oak.back().index() == 2 );
		CHECK( oak.size() == 1 );

		oak.close( ec );
		CHECK( ! ec );
	}
	
}

// TEST_CASE("nodeoze/smoke/raft/basic")

#endif
