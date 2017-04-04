#include "m3/gem.hpp"
#include "m3/gem_circle.hpp"

#include <gtest/gtest.h>

TEST( gem_circle, initial_launcher_angle )
{
  m3::gem_circle circle;
  
  EXPECT_FLOAT_EQ( 0, circle.get_launcher_angle() );
}

TEST( gem_circle, set_launcher_angle )
{
  m3::gem_circle circle;
  circle.set_launcher_angle( 0.3 );
  
  EXPECT_FLOAT_EQ( 0.3, circle.get_launcher_angle() );
}

TEST( gem_circle, launch )
{
  m3::gem_circle circle;
  circle.set_launcher_angle( 0.3 );

  const m3::gem g( 2 );
  
  circle.launch( g );
  const std::vector< m3::gem >& gems( circle.gems() );
  const std::vector< std::size_t >& free_gems( circle.free_gems() );

  ASSERT_EQ( 1, free_gems.size() );
  ASSERT_LT( free_gems[ 0 ], gems.size() );
  EXPECT_EQ( g, circle.gems()[ free_gems[ 0 ] ] );
}
