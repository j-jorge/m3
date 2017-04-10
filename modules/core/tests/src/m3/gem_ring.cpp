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
  EXPECT_EQ( g, gems[ free_gems[ 0 ] ] );

  const std::vector< float >& radius( ring.free_gem_radius() );
  ASSERT_EQ( free_gems.size(), radius.size() );
  EXPECT_FLOAT_EQ( 0, radius[ 0 ] );
  
  const std::vector< float >& directions( ring.free_gem_direction() );
  ASSERT_EQ( free_gems.size(), directions.size() );
  EXPECT_FLOAT_EQ( direction, directions[ 0 ] );
}

TEST( gem_ring, expand_one )
{
  m3::gem_ring ring;

  const float direction( 0.9 );
  const m3::gem g( 4 );
  
  ring.launch( direction, g );

  ring.expand( 0.5 );

  {
    const std::vector< m3::gem >& gems( ring.gems() );
    const std::vector< std::size_t >& free_gems( ring.free_gems() );

    ASSERT_EQ( 1, free_gems.size() );
    ASSERT_LT( free_gems[ 0 ], gems.size() );
    EXPECT_EQ( g, gems[ free_gems[ 0 ] ] );

    const std::vector< float >& gem_radius( ring.free_gem_radius() );
    ASSERT_EQ( 1, gem_radius.size() );
    EXPECT_EQ( 0.5, gem_radius[ 0 ] );

    const std::vector< float >& gem_direction( ring.free_gem_direction() );
    ASSERT_EQ( 1, gem_direction.size() );
    EXPECT_EQ( direction, gem_direction[ 0 ] );
  }

  ring.expand( 0.5 );

  {
    EXPECT_TRUE( ring.free_gems().empty() );
    EXPECT_TRUE( ring.free_gem_radius().empty() );
    EXPECT_TRUE( ring.free_gem_direction().empty() );

    const std::vector< m3::gem >& gems( ring.gems() );
    const std::vector< std::size_t >& chain( ring.chain() );

    ASSERT_EQ( 1, chain.size() );
    ASSERT_LT( chain[ 0 ], gems.size() );
    EXPECT_EQ( g, gems[ chain[ 0 ] ] );
  }
}

TEST( gem_ring, expand_four )
{
  m3::gem_ring ring;

  const float direction_1( 0.3 );
  const m3::gem g_1( 4 );
  
  ring.launch( direction_1, g_1 );
  ring.expand( 0.3 );

  const float direction_2( 0.8 );
  const m3::gem g_2( 1 );
  
  ring.launch( direction_2, g_2 );
  ring.expand( 0.4 );

  const float direction_3( 2.1 );
  const m3::gem g_3( 3 );
  
  ring.launch( direction_3, g_3 );

  {
    const std::vector< m3::gem >& gems( ring.gems() );
    const std::vector< std::size_t >& free_gems( ring.free_gems() );

    ASSERT_EQ( 3, free_gems.size() );

    ASSERT_LT( free_gems[ 0 ], gems.size() );
    ASSERT_LT( free_gems[ 1 ], gems.size() );
    ASSERT_LT( free_gems[ 2 ], gems.size() );

    EXPECT_EQ( g_1, gems[ free_gems[ 0 ] ] );
    EXPECT_EQ( g_2, gems[ free_gems[ 1 ] ] );
    EXPECT_EQ( g_3, gems[ free_gems[ 2 ] ] );

    const std::vector< float >& gem_radius( ring.free_gem_radius() );
    ASSERT_EQ( 3, gem_radius.size() );
    EXPECT_FLOAT_EQ( 0.7, gem_radius[ 0 ] );
    EXPECT_FLOAT_EQ( 0.4, gem_radius[ 1 ] );
    EXPECT_FLOAT_EQ( 0, gem_radius[ 2 ] );

    const std::vector< float >& gem_direction( ring.free_gem_direction() );
    ASSERT_EQ( 3, gem_direction.size() );
    EXPECT_FLOAT_EQ( direction_1, gem_direction[ 0 ] );
    EXPECT_FLOAT_EQ( direction_2, gem_direction[ 1 ] );
    EXPECT_FLOAT_EQ( direction_3, gem_direction[ 2 ] );
  }

  ring.expand( 0.3 );

  {
    const std::vector< m3::gem >& gems( ring.gems() );
    const std::vector< std::size_t >& free_gems( ring.free_gems() );

    ASSERT_EQ( 2, free_gems.size() );

    ASSERT_LT( free_gems[ 0 ], gems.size() );
    ASSERT_LT( free_gems[ 1 ], gems.size() );

    EXPECT_EQ( g_2, gems[ free_gems[ 0 ] ] );
    EXPECT_EQ( g_3, gems[ free_gems[ 1 ] ] );

    const std::vector< float >& gem_radius( ring.free_gem_radius() );
    ASSERT_EQ( 2, gem_radius.size() );
    EXPECT_FLOAT_EQ( 0.7, gem_radius[ 0 ] );
    EXPECT_FLOAT_EQ( 0.3, gem_radius[ 1 ] );

    const std::vector< float >& gem_direction( ring.free_gem_direction() );
    ASSERT_EQ( 2, gem_direction.size() );
    EXPECT_FLOAT_EQ( direction_2, gem_direction[ 0 ] );
    EXPECT_FLOAT_EQ( direction_3, gem_direction[ 1 ] );

    const std::vector< std::size_t >& chain( ring.chain() );

    ASSERT_EQ( 1, chain.size() );
    ASSERT_LT( chain[ 0 ], gems.size() );
    EXPECT_EQ( g_1, gems[ chain[ 0 ] ] );
  }

  ring.expand( 0.3 );

  {
    const std::vector< m3::gem >& gems( ring.gems() );
    const std::vector< std::size_t >& free_gems( ring.free_gems() );

    ASSERT_EQ( 1, free_gems.size() );

    ASSERT_LT( free_gems[ 0 ], gems.size() );

    EXPECT_EQ( g_3, gems[ free_gems[ 0 ] ] );

    const std::vector< float >& gem_radius( ring.free_gem_radius() );
    ASSERT_EQ( 1, gem_radius.size() );
    EXPECT_FLOAT_EQ( 0.6, gem_radius[ 0 ] );

    const std::vector< float >& gem_direction( ring.free_gem_direction() );
    ASSERT_EQ( 1, gem_direction.size() );
    EXPECT_FLOAT_EQ( direction_3, gem_direction[ 0 ] );

    const std::vector< std::size_t >& chain( ring.chain() );

    ASSERT_EQ( 2, chain.size() );
    ASSERT_LT( chain[ 0 ], gems.size() );
    ASSERT_LT( chain[ 1 ], gems.size() );
    EXPECT_EQ( g_1, gems[ chain[ 0 ] ] );
    EXPECT_EQ( g_2, gems[ chain[ 1 ] ] );
  }

  ring.expand( 0.4 );

  {
    ASSERT_TRUE( ring.free_gems().empty() );
    ASSERT_TRUE( ring.free_gem_radius().empty() );
    ASSERT_TRUE( ring.free_gem_direction().empty() );
    
    const std::vector< m3::gem >& gems( ring.gems() );
    const std::vector< std::size_t >& chain( ring.chain() );

    ASSERT_EQ( 3, chain.size() );
    ASSERT_LT( chain[ 0 ], gems.size() );
    ASSERT_LT( chain[ 1 ], gems.size() );
    ASSERT_LT( chain[ 2 ], gems.size() );
    EXPECT_EQ( g_1, gems[ chain[ 0 ] ] );
    EXPECT_EQ( g_2, gems[ chain[ 1 ] ] );
    EXPECT_EQ( g_3, gems[ chain[ 2 ] ] );
  }
}
