#include "m3/game/forward_destructions.hpp"

#include "m3/game/gem_ring.hpp"

#include <gtest/gtest.h>

TEST( forward_destructions, empty_ring )
{
  m3::game::gem_ring ring;
  
  EXPECT_TRUE( m3::game::forward_destructions( ring, 3 ).empty() );
}

TEST( forward_destructions, no_destruction )
{
  m3::game::gem_ring ring;

  ring.launch( 0, 1 );
  ring.expand( 1 );
  
  ring.launch( 0, 2 );
  ring.expand( 1 );

  ring.launch( 0, 3 );
  ring.expand( 1 );

  const std::vector< bool > expected( { false, false, false } );
  EXPECT_EQ( expected, m3::game::forward_destructions( ring, 3 ) );
}

TEST( forward_destructions, single_destruction )
{
  m3::game::gem_ring ring( { 1, 2, 2, 3, 4 } );

  const std::vector< bool > expected( { false, true, true, false, false } );
  EXPECT_EQ( expected, m3::game::forward_destructions( ring, 2 ) );
}

TEST( forward_destructions, two_simultaneous_destructions )
{
  m3::game::gem_ring ring( { 1, 2, 3, 3, 3, 4, 1, 1 } );

  const std::vector< bool > expected
    ( { true, false, true, true, true, false, true, true } );
  EXPECT_EQ( expected, m3::game::forward_destructions( ring, 3 ) );
}

TEST( forward_destructions, two_successive_destructions )
{
  m3::game::gem_ring ring( { 1, 4, 4, 2, 3, 3, 3, 4, 4, 1, 1 } );

  const std::vector< bool > expected
    ( { true, true, true, false, true, true, true, true, true, true, true } );
  EXPECT_EQ( expected, m3::game::forward_destructions( ring, 3 ) );
}

TEST( forward_destructions, all_destroyed )
{
  m3::game::gem_ring ring( { 1, 2, 2, 2, 1, 4, 4, 4, 1 } );

  const std::vector< bool > expected
    ( { true, true, true, true, true, true, true, true, true } );
  EXPECT_EQ( expected, m3::game::forward_destructions( ring, 3 ) );
}

TEST( forward_destructions, expand )
{
  m3::game::gem_ring ring( { 1, 2, 3, 3, 2, 2, 1 } );

  ring.launch( 3.14159 / 2, 3 );
  
  const std::vector< bool > expected
    ( { false, true, true, true, true, true, false } );
  EXPECT_EQ( expected, m3::game::forward_destructions( ring, 3 ) );
}

TEST( forward_destructions, expand_three )
{
  m3::game::gem_ring ring( { 1, 1, 2, 2, 3, 3, 4, 4 } );

  ring.launch( 0, 1 );
  ring.launch( 3.14159 / 2, 2 );
  ring.launch( -3.14159 / 2, 4 );

  const std::vector< bool > expected
    ( { true, true, true, true, false, false, true, true } );
  EXPECT_EQ( expected, m3::game::forward_destructions( ring, 3 ) );
}
