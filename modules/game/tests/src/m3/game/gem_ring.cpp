#include "m3/game/gem_ring.hpp"

#include <gtest/gtest.h>

TEST( gem_ring, initial_orientation )
{
  m3::game::gem_ring ring;
  
  EXPECT_FLOAT_EQ( 0, ring.get_orientation() );
}

TEST( gem_ring, set_orientation )
{
  m3::game::gem_ring ring;
  ring.set_orientation( 0.3 );
  
  EXPECT_FLOAT_EQ( 0.3, ring.get_orientation() );
}

TEST( gem_ring, launch )
{
  m3::game::gem_ring ring;

  const float direction( 0.3 );
  const m3::game::gem g( 2 );
  
  ring.launch( direction, g );
  const std::vector< m3::game::gem >& free_gems( ring.free_gems() );

  ASSERT_EQ( 1, free_gems.size() );
  EXPECT_EQ( g, free_gems[ 0 ] );

  const std::vector< float >& radius( ring.free_gem_radius() );
  ASSERT_EQ( free_gems.size(), radius.size() );
  EXPECT_FLOAT_EQ( 0, radius[ 0 ] );
  
  const std::vector< float >& directions( ring.free_gem_direction() );
  ASSERT_EQ( free_gems.size(), directions.size() );
  EXPECT_FLOAT_EQ( direction, directions[ 0 ] );
}

TEST( gem_ring, expand_one )
{
  m3::game::gem_ring ring;

  const float direction( 0.9 );
  const m3::game::gem g( 4 );
  
  ring.launch( direction, g );

  std::vector< std::size_t > inserted( ring.expand( 0.5 ) );
  EXPECT_TRUE( inserted.empty() );

  {
    const std::vector< m3::game::gem >& free_gems( ring.free_gems() );

    ASSERT_EQ( 1, free_gems.size() );
    EXPECT_EQ( g, free_gems[ 0 ] );

    const std::vector< float >& gem_radius( ring.free_gem_radius() );
    ASSERT_EQ( 1, gem_radius.size() );
    EXPECT_EQ( 0.5, gem_radius[ 0 ] );

    const std::vector< float >& gem_direction( ring.free_gem_direction() );
    ASSERT_EQ( 1, gem_direction.size() );
    EXPECT_EQ( direction, gem_direction[ 0 ] );
  }

  inserted = ring.expand( 0.5 );
  EXPECT_EQ( 1, inserted.size() );
  EXPECT_EQ( 0, inserted[ 0 ] );

  {
    EXPECT_TRUE( ring.free_gems().empty() );
    EXPECT_TRUE( ring.free_gem_radius().empty() );
    EXPECT_TRUE( ring.free_gem_direction().empty() );

    const std::vector< m3::game::gem >& chain( ring.chain() );

    ASSERT_EQ( 1, chain.size() );
    EXPECT_EQ( g, chain[ 0 ] );
  }
}

TEST( gem_ring, expand_four )
{
  m3::game::gem_ring ring;

  const float direction_1( 0.3 );
  const m3::game::gem g_1( 4 );
  
  ring.launch( direction_1, g_1 );

  std::vector< std::size_t > inserted( ring.expand( 0.3 ) );
  EXPECT_TRUE( inserted.empty() );

  const float direction_2( 0.8 );
  const m3::game::gem g_2( 1 );
  
  ring.launch( direction_2, g_2 );

  inserted = ring.expand( 0.4 );
  EXPECT_TRUE( inserted.empty() );

  const float direction_3( 2.1 );
  const m3::game::gem g_3( 3 );
  
  ring.launch( direction_3, g_3 );

  {
    const std::vector< m3::game::gem >& free_gems( ring.free_gems() );

    ASSERT_EQ( 3, free_gems.size() );

    EXPECT_EQ( g_1, free_gems[ 0 ] );
    EXPECT_EQ( g_2, free_gems[ 1 ] );
    EXPECT_EQ( g_3, free_gems[ 2 ] );

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

  inserted = ring.expand( 0.3 );
  EXPECT_EQ( 1, inserted.size() );
  EXPECT_EQ( 0, inserted[ 0 ] );
  
  {
    const std::vector< m3::game::gem >& free_gems( ring.free_gems() );

    ASSERT_EQ( 2, free_gems.size() );

    EXPECT_EQ( g_2, free_gems[ 0 ] );
    EXPECT_EQ( g_3, free_gems[ 1 ] );

    const std::vector< float >& gem_radius( ring.free_gem_radius() );
    ASSERT_EQ( 2, gem_radius.size() );
    EXPECT_FLOAT_EQ( 0.7, gem_radius[ 0 ] );
    EXPECT_FLOAT_EQ( 0.3, gem_radius[ 1 ] );

    const std::vector< float >& gem_direction( ring.free_gem_direction() );
    ASSERT_EQ( 2, gem_direction.size() );
    EXPECT_FLOAT_EQ( direction_2, gem_direction[ 0 ] );
    EXPECT_FLOAT_EQ( direction_3, gem_direction[ 1 ] );

    const std::vector< m3::game::gem >& chain( ring.chain() );

    ASSERT_EQ( 1, chain.size() );
    EXPECT_EQ( g_1, chain[ 0 ] );
  }

  inserted = ring.expand( 0.3 );
  EXPECT_EQ( 1, inserted.size() );
  EXPECT_EQ( 1, inserted[ 0 ] );

  {
    const std::vector< m3::game::gem >& free_gems( ring.free_gems() );

    ASSERT_EQ( 1, free_gems.size() );

    EXPECT_EQ( g_3, free_gems[ 0 ] );

    const std::vector< float >& gem_radius( ring.free_gem_radius() );
    ASSERT_EQ( 1, gem_radius.size() );
    EXPECT_FLOAT_EQ( 0.6, gem_radius[ 0 ] );

    const std::vector< float >& gem_direction( ring.free_gem_direction() );
    ASSERT_EQ( 1, gem_direction.size() );
    EXPECT_FLOAT_EQ( direction_3, gem_direction[ 0 ] );

    const std::vector< m3::game::gem >& chain( ring.chain() );

    ASSERT_EQ( 2, chain.size() );
    EXPECT_EQ( g_1, chain[ 0 ] );
    EXPECT_EQ( g_2, chain[ 1 ] );
  }

  inserted = ring.expand( 0.4 );
  EXPECT_EQ( 1, inserted.size() );
  EXPECT_EQ( 1, inserted[ 0 ] );

  {
    ASSERT_TRUE( ring.free_gems().empty() );
    ASSERT_TRUE( ring.free_gem_radius().empty() );
    ASSERT_TRUE( ring.free_gem_direction().empty() );
    
    const std::vector< m3::game::gem >& chain( ring.chain() );

    ASSERT_EQ( 3, chain.size() );
    EXPECT_EQ( g_1, chain[ 0 ] );
    EXPECT_EQ( g_2, chain[ 2 ] );
    EXPECT_EQ( g_3, chain[ 1 ] );
  }
}

TEST( gem_ring, expand_order )
{
  m3::game::gem_ring ring;

  const m3::game::gem g_1( 4 );

  ring.launch( 0, g_1 );
  ring.expand( 1 );

  const m3::game::gem g_2( 1 );

  ring.launch( 0, g_2 );
  ring.expand( 1 );

  const m3::game::gem g_3( 2 );

  ring.launch( 0, g_3 );
  ring.expand( 1 );

  {
    const std::vector< m3::game::gem >& chain( ring.chain() );

    EXPECT_EQ( g_1, chain[ 0 ] );
    EXPECT_EQ( g_3, chain[ 1 ] );
    EXPECT_EQ( g_2, chain[ 2 ] );
  }

  const m3::game::gem g_4( 7 );
  
  ring.set_orientation( -2.1 );
  ring.launch( 0, g_4 );
  ring.expand( 1 );

  {
    const std::vector< m3::game::gem >& chain( ring.chain() );

    EXPECT_EQ( g_1, chain[ 0 ] );
    EXPECT_EQ( g_3, chain[ 1 ] );
    EXPECT_EQ( g_4, chain[ 2 ] );
    EXPECT_EQ( g_2, chain[ 3 ] );
  }

  const m3::game::gem g_5( 55 );
  
  ring.set_orientation( 0 );
  ring.launch( 4.713, g_5 );
  ring.expand( 1 );

  {
    const std::vector< m3::game::gem >& chain( ring.chain() );

    EXPECT_EQ( g_1, chain[ 0 ] );
    EXPECT_EQ( g_3, chain[ 1 ] );
    EXPECT_EQ( g_4, chain[ 2 ] );
    EXPECT_EQ( g_2, chain[ 3 ] );
    EXPECT_EQ( g_5, chain[ 4 ] );
  }
}

TEST( gem_ring, expand_four_at_once )
{
  m3::game::gem_ring ring;

  ring.launch( 0.1, m3::game::gem( 1 ) );
  ring.launch( 0.2, m3::game::gem( 2 ) );
  ring.launch( 0.3, m3::game::gem( 3 ) );
      
  std::vector< std::size_t > inserted( ring.expand( 1 ) );
  EXPECT_EQ( 3, inserted.size() );
  EXPECT_EQ( 0, inserted[ 0 ] );
  EXPECT_EQ( 2, inserted[ 1 ] );
  EXPECT_EQ( 1, inserted[ 2 ] );

  ring.launch( 0.4, m3::game::gem( 4 ) );
  ring.launch( 0.5, m3::game::gem( 5 ) );
  ring.launch( 0.6, m3::game::gem( 6 ) );
  ring.launch( 0.7, m3::game::gem( 7 ) );
      
  inserted = ring.expand( 1 );
  EXPECT_EQ( 4, inserted.size() );
  EXPECT_EQ( 4, inserted[ 0 ] );
  EXPECT_EQ( 3, inserted[ 1 ] );
  EXPECT_EQ( 2, inserted[ 2 ] );
  EXPECT_EQ( 1, inserted[ 3 ] );
}

TEST( gem_ring, erase )
{
  m3::game::gem_ring ring;

  const m3::game::gem g_1( 4 );

  ring.launch( 0, g_1 );
  ring.expand( 1 );

  const m3::game::gem g_2( 1 );

  ring.launch( 0, g_2 );
  ring.expand( 1 );

  const m3::game::gem g_3( 2 );

  ring.launch( 0, g_3 );
  ring.expand( 1 );

  const m3::game::gem g_4( 7 );
  
  ring.launch( 2.1, g_4 );
  ring.expand( 1 );

  const m3::game::gem g_5( 55 );
  
  ring.launch( 4.713, g_5 );
  ring.expand( 1 );

  {
    const std::vector< m3::game::gem >& chain( ring.chain() );

    EXPECT_EQ( g_1, chain[ 0 ] );
    EXPECT_EQ( g_3, chain[ 1 ] );
    EXPECT_EQ( g_4, chain[ 2 ] );
    EXPECT_EQ( g_2, chain[ 3 ] );
    EXPECT_EQ( g_5, chain[ 4 ] );
  }

  ring.erase( { 1, 3 } );

  {
    const std::vector< m3::game::gem >& chain( ring.chain() );

    EXPECT_EQ( g_1, chain[ 0 ] );
    EXPECT_EQ( g_4, chain[ 1 ] );
    EXPECT_EQ( g_5, chain[ 2 ] );
  }

  ring.erase( { 0, 2 } );

  {
    const std::vector< m3::game::gem >& chain( ring.chain() );

    EXPECT_EQ( g_4, chain[ 1 ] );
  }

  ring.erase( { 0 } );

  {
    const std::vector< m3::game::gem >& chain( ring.chain() );

    EXPECT_TRUE( chain.empty() );
  }
}
