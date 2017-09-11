#include "m3/game/random_ring_without_match.hpp"

#include "m3/game/gem_generator.hpp"
#include "m3/game/gem_ring.hpp"
#include "m3/game/find_adjacent_ring_gems.hpp"

#include <gtest/gtest.h>

class random_ring_without_match_test:
  public ::testing::TestWithParam
  <
    ::testing::tuple< unsigned int, unsigned int >
  >
{

};

TEST_P( random_ring_without_match_test, chain )
{
  static const unsigned int gem_type_count( ::testing::get< 0 >( GetParam() ) );
  static const unsigned int count( ::testing::get< 1 >( GetParam() ) );
  static const unsigned int match_size( 3 );

  m3::game::gem_ring ring;
  m3::game::gem_generator generator( gem_type_count );
  m3::game::random_ring_without_match( ring, generator, count, match_size );

  EXPECT_EQ( count, ring.chain().size() );
  EXPECT_TRUE
    ( m3::game::find_adjacent_ring_gems( ring.chain(), match_size ).empty() );
}

INSTANTIATE_TEST_CASE_P
( various_sizes, random_ring_without_match_test,
  ::testing::Combine
  ( ::testing::Values( 3, 4, 5, 6 ),
    ::testing::Values( 10, 15, 20, 25, 30, 33, 37, 40, 50, 100, 1000 ) ) );
