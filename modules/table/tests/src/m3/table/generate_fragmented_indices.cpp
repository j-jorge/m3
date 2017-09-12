#include "m3/table/generate_fragmented_indices.hpp"

#include <gtest/gtest.h>

TEST( find_adjacent_ring_gems, empty )
{
  EXPECT_TRUE( m3::table::generate_fragmented_indices( 0, {} ).empty() );
}

TEST( find_adjacent_ring_gems, all_removed )
{
  const std::vector< std::size_t > indices
    ( m3::table::generate_fragmented_indices( 3, { 0, 1, 2 } ) );
  const std::vector< std::size_t > expected( { 3, 3, 3 } );
  
  EXPECT_EQ( expected, indices );
}

TEST( find_adjacent_ring_gems, none_removed )
{
  const std::vector< std::size_t > indices
    ( m3::table::generate_fragmented_indices( 3, {} ) );
  const std::vector< std::size_t > expected( { 0, 1, 2 } );
  
  EXPECT_EQ( expected, indices );
}

TEST( find_adjacent_ring_gems, some_removed )
{
  const std::vector< std::size_t > indices
    ( m3::table::generate_fragmented_indices( 5, { 1, 4 } ) );
  const std::vector< std::size_t > expected( { 0, 5, 1, 2, 5 } );
  
  EXPECT_EQ( expected, indices );
}

TEST( find_adjacent_ring_gems, remove_front )
{
  const std::vector< std::size_t > indices
    ( m3::table::generate_fragmented_indices( 5, { 0, 1 } ) );
  const std::vector< std::size_t > expected( { 5, 5, 0, 1, 2 } );
  
  EXPECT_EQ( expected, indices );
}

TEST( find_adjacent_ring_gems, remove_back )
{
  const std::vector< std::size_t > indices
    ( m3::table::generate_fragmented_indices( 5, { 3, 4 } ) );
  const std::vector< std::size_t > expected( { 0, 1, 2, 5, 5 } );
  
  EXPECT_EQ( expected, indices );
}

TEST( find_adjacent_ring_gems, remove_both_ends )
{
  const std::vector< std::size_t > indices
    ( m3::table::generate_fragmented_indices( 5, { 0, 1, 3, 4 } ) );
  const std::vector< std::size_t > expected( { 5, 5, 0, 5, 5 } );
  
  EXPECT_EQ( expected, indices );
}
