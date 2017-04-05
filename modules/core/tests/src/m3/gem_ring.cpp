#include "m3/gem.hpp"
#include "m3/gem_ring.hpp"

#include <gtest/gtest.h>

TEST( gem_ring, initial_orientation )
{
  m3::gem_ring ring;
  
  EXPECT_FLOAT_EQ( 0, ring.get_orientation() );
}

TEST( gem_ring, set_orientation )
{
  m3::gem_ring ring;
  ring.set_orientation( 0.3 );
  
  EXPECT_FLOAT_EQ( 0.3, ring.get_orientation() );
}

TEST( gem_ring, launch )
{
  m3::gem_ring ring;

  const float direction( 0.3 );
  const m3::gem g( 2 );
  
  ring.launch( direction, g );
  const std::vector< m3::gem >& gems( ring.gems() );
  const std::vector< std::size_t >& free_gems( ring.free_gems() );

  ASSERT_EQ( 1, free_gems.size() );
  ASSERT_LT( free_gems[ 0 ], gems.size() );
  EXPECT_EQ( g, ring.gems()[ free_gems[ 0 ] ] );

  const std::vector< float >& radius( ring.free_gem_radius() );
  ASSERT_EQ( free_gems.size(), radius.size() );
  EXPECT_FLOAT_EQ( 0, radius[ 0 ] );
  
  const std::vector< float >& directions( ring.free_gem_direction() );
  ASSERT_EQ( free_gems.size(), directions.size() );
  EXPECT_FLOAT_EQ( direction, directions[ 0 ] );
}
