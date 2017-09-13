#include "m3/game/find_adjacent_ring_gems.hpp"

#include <gtest/gtest.h>

TEST( find_adjacent_ring_gems, empty )
{
  EXPECT_TRUE( m3::game::find_adjacent_ring_gems( {}, 0 ).empty() );
  EXPECT_TRUE( m3::game::find_adjacent_ring_gems( {}, 1 ).empty() );
  EXPECT_TRUE( m3::game::find_adjacent_ring_gems( {}, 3 ).empty() );
  EXPECT_TRUE( m3::game::find_adjacent_ring_gems( {}, 9 ).empty() );
}

TEST( find_adjacent_ring_gems, zero )
{
  const std::vector< m3::game::gem > gems( { 2, 3, 4 } );
  
  const std::vector< std::size_t > ranges
    ( m3::game::find_adjacent_ring_gems( gems, 0 ) );
  const std::vector< std::size_t > expected( { 0, 1, 2 } );
  
  EXPECT_EQ( expected, ranges );
}

TEST( find_adjacent_ring_gems, one )
{
  const std::vector< m3::game::gem > gems( { 2 } );
  
  const std::vector< std::size_t > ranges
    ( m3::game::find_adjacent_ring_gems( gems, 1 ) );
  const std::vector< std::size_t > expected( { 0 } );
  
  EXPECT_EQ( expected, ranges );
  
  EXPECT_TRUE( m3::game::find_adjacent_ring_gems( {}, 3 ).empty() );
}

TEST( find_adjacent_ring_gems, identical )
{
  const std::vector< m3::game::gem > gems( { 3, 3, 3 } );

  {
    const std::vector< std::size_t > ranges
      ( m3::game::find_adjacent_ring_gems( gems, 1 ) );
    const std::vector< std::size_t > expected( { 0, 1, 2 } );
  
    EXPECT_EQ( expected, ranges );
  }
  
  {
    const std::vector< std::size_t > ranges
      ( m3::game::find_adjacent_ring_gems( gems, 2 ) );
    const std::vector< std::size_t > expected( { 0, 1, 2 } );
  
    EXPECT_EQ( expected, ranges );
  }
  
  {
    const std::vector< std::size_t > ranges
      ( m3::game::find_adjacent_ring_gems( gems, 3 ) );
    const std::vector< std::size_t > expected( { 0, 1, 2 } );
  
    EXPECT_EQ( expected, ranges );
  }
  
  EXPECT_TRUE( m3::game::find_adjacent_ring_gems( {}, 4 ).empty() );
}

TEST( find_adjacent_ring_gems, no_loop_fit )
{
  const std::vector< m3::game::gem > gems( { 2, 3, 3, 4 } );
  
  const std::vector< std::size_t > ranges
    ( m3::game::find_adjacent_ring_gems( gems, 2 ) );
  const std::vector< std::size_t > expected( { 1, 2 } );
  
  EXPECT_EQ( expected, ranges );
}

TEST( find_adjacent_ring_gems, no_loop_larger )
{
  const std::vector< m3::game::gem > gems( { 2, 3, 3, 3, 3, 4 } );
  
  const std::vector< std::size_t > ranges
    ( m3::game::find_adjacent_ring_gems( gems, 2 ) );
  const std::vector< std::size_t > expected( { 1, 2, 3, 4 } );
  
  EXPECT_EQ( expected, ranges );
}

TEST( find_adjacent_ring_gems, loop_fit )
{
  const std::vector< m3::game::gem > gems( { 2, 3, 4, 2 } );
  
  const std::vector< std::size_t > ranges
    ( m3::game::find_adjacent_ring_gems( gems, 2 ) );
  const std::vector< std::size_t > expected( { 0, 3 } );
  
  EXPECT_EQ( expected, ranges );
}

TEST( find_adjacent_ring_gems, loop_larger_right )
{
  const std::vector< m3::game::gem > gems( { 3, 4, 3, 3 } );
  
  const std::vector< std::size_t > ranges
    ( m3::game::find_adjacent_ring_gems( gems, 2 ) );
  const std::vector< std::size_t > expected( { 0, 2, 3 } );
  
  EXPECT_EQ( expected, ranges );
}

TEST( find_adjacent_ring_gems, loop_larger_left )
{
  const std::vector< m3::game::gem > gems( { 3, 3, 4, 3 } );
  
  const std::vector< std::size_t > ranges
    ( m3::game::find_adjacent_ring_gems( gems, 2 ) );
  const std::vector< std::size_t > expected( { 0, 1, 3 } );
  
  EXPECT_EQ( expected, ranges );
}

TEST( find_adjacent_ring_gems, no_loop_multiple_solutions )
{
  const std::vector< m3::game::gem > gems
    ( {
        1, 3, 3, 3, 2,
        3, 4, 1, 1, 4,
        2, 2, 2, 2, 4
      } );
  
  const std::vector< std::size_t > ranges
    ( m3::game::find_adjacent_ring_gems( gems, 2 ) );
  const std::vector< std::size_t > expected
    ( { 1, 2, 3, 7, 8, 10, 11, 12, 13 } );
  
  EXPECT_EQ( expected, ranges );
}

TEST( find_adjacent_ring_gems, loop_multiple_solutions )
{
  const std::vector< m3::game::gem > gems
    ( {
        3, 3, 3, 1, 2,
        3, 2, 2, 2, 1,
        4, 4, 2, 3, 3
      } );
  
  const std::vector< std::size_t > ranges
    ( m3::game::find_adjacent_ring_gems( gems, 2 ) );
  const std::vector< std::size_t > expected
    ( { 0, 1, 2, 6, 7, 8, 10, 11, 13, 14 } );
  
  EXPECT_EQ( expected, ranges );
}

TEST( find_adjacent_ring_gems, too_small )
{
  EXPECT_TRUE( m3::game::find_adjacent_ring_gems( { 1 }, 2 ).empty() );
  EXPECT_TRUE( m3::game::find_adjacent_ring_gems( { 1, 1 }, 3 ).empty() );
}

