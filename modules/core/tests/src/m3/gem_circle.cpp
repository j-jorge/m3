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
