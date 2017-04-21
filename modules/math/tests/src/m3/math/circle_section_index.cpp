#include "m3/math/circle_section_index.hpp"

#include <gtest/gtest.h>

TEST( m3_math_circle_section_index, zero )
{
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( -6.283, 0 ) );
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( -1.570, 0 ) );
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( 0, 0 ) );
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( 1, 0 ) );
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( 3.2, 0 ) );
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( 6.1, 0 ) );
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( 7.3, 0 ) );
}

TEST( m3_math_circle_section_index, one )
{
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( -6.283, 1 ) );
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( -1.570, 1 ) );
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( 0, 1 ) );
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( 1.8, 1 ) );
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( 7.8, 1 ) );
}

TEST( m3_math_circle_section_index, two )
{
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( -6.283, 2 ) );
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( -3.1416, 2 ) );
  EXPECT_FLOAT_EQ( 1, m3::math::circle_section_index( -3.1415, 2 ) );

  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( 0, 2 ) );
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( 3.1415, 2 ) );
  EXPECT_FLOAT_EQ( 1, m3::math::circle_section_index( 3.1416, 2 ) );
  EXPECT_FLOAT_EQ( 1, m3::math::circle_section_index( 6.283, 2 ) );

  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( 6.284, 2 ) );
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( 9.424, 2 ) );
  EXPECT_FLOAT_EQ( 1, m3::math::circle_section_index( 9.425, 2 ) );
}

TEST( m3_math_circle_section_index, three )
{
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( -12.566, 3 ) );
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( -10.472, 3 ) );
  EXPECT_FLOAT_EQ( 1, m3::math::circle_section_index( -10.471, 3 ) );
  EXPECT_FLOAT_EQ( 1, m3::math::circle_section_index( -8.378, 3 ) );
  EXPECT_FLOAT_EQ( 2, m3::math::circle_section_index( -8.377, 3 ) );
  EXPECT_FLOAT_EQ( 2, m3::math::circle_section_index( -6.284, 3 ) );

  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( -6.283, 3 ) );
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( -4.189, 3 ) );
  EXPECT_FLOAT_EQ( 1, m3::math::circle_section_index( -4.188, 3 ) );
  EXPECT_FLOAT_EQ( 1, m3::math::circle_section_index( -2.095, 3 ) );
  EXPECT_FLOAT_EQ( 2, m3::math::circle_section_index( -2.094, 3 ) );
  EXPECT_FLOAT_EQ( 2, m3::math::circle_section_index( -0.001, 3 ) );

  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( 0, 3 ) );
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( 2.094, 3 ) );
  EXPECT_FLOAT_EQ( 1, m3::math::circle_section_index( 2.095, 3 ) );
  EXPECT_FLOAT_EQ( 1, m3::math::circle_section_index( 4.188, 3 ) );
  EXPECT_FLOAT_EQ( 2, m3::math::circle_section_index( 4.189, 3 ) );
  EXPECT_FLOAT_EQ( 2, m3::math::circle_section_index( 6.283, 3 ) );

  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( 6.284, 3 ) );
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( 8.377, 3 ) );
  EXPECT_FLOAT_EQ( 1, m3::math::circle_section_index( 8.378, 3 ) );
  EXPECT_FLOAT_EQ( 1, m3::math::circle_section_index( 10.471, 3 ) );
  EXPECT_FLOAT_EQ( 2, m3::math::circle_section_index( 10.472, 3 ) );
  EXPECT_FLOAT_EQ( 2, m3::math::circle_section_index( 12.566, 3 ) );
}

TEST( m3_math_circle_section_index, four )
{
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( -12.566, 4 ) );
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( -10.996, 4 ) );
  EXPECT_FLOAT_EQ( 1, m3::math::circle_section_index( -10.995, 4 ) );
  EXPECT_FLOAT_EQ( 1, m3::math::circle_section_index( -9.425, 4 ) );
  EXPECT_FLOAT_EQ( 2, m3::math::circle_section_index( -9.424, 4 ) );
  EXPECT_FLOAT_EQ( 2, m3::math::circle_section_index( -7.854, 4 ) );
  EXPECT_FLOAT_EQ( 3, m3::math::circle_section_index( -7.853, 4 ) );
  EXPECT_FLOAT_EQ( 3, m3::math::circle_section_index( -6.284, 4 ) );

  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( -6.283, 4 ) );
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( -4.713, 4 ) );
  EXPECT_FLOAT_EQ( 1, m3::math::circle_section_index( -4.712, 4 ) );
  EXPECT_FLOAT_EQ( 1, m3::math::circle_section_index( -3.1416, 4 ) );
  EXPECT_FLOAT_EQ( 2, m3::math::circle_section_index( -3.1415, 4 ) );
  EXPECT_FLOAT_EQ( 2, m3::math::circle_section_index( -1.571, 4 ) );
  EXPECT_FLOAT_EQ( 3, m3::math::circle_section_index( -1.570, 4 ) );
  EXPECT_FLOAT_EQ( 3, m3::math::circle_section_index( -0.001, 4 ) );

  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( 0, 4 ) );
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( 1.570, 4 ) );
  EXPECT_FLOAT_EQ( 1, m3::math::circle_section_index( 1.571, 4 ) );
  EXPECT_FLOAT_EQ( 1, m3::math::circle_section_index( 3.1415, 4 ) );
  EXPECT_FLOAT_EQ( 2, m3::math::circle_section_index( 3.1416, 4 ) );
  EXPECT_FLOAT_EQ( 2, m3::math::circle_section_index( 4.712, 4 ) );
  EXPECT_FLOAT_EQ( 3, m3::math::circle_section_index( 4.713, 4 ) );
  EXPECT_FLOAT_EQ( 3, m3::math::circle_section_index( 6.283, 4 ) );

  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( 6.284, 4 ) );
  EXPECT_FLOAT_EQ( 0, m3::math::circle_section_index( 7.853, 4 ) );
  EXPECT_FLOAT_EQ( 1, m3::math::circle_section_index( 7.854, 4 ) );
  EXPECT_FLOAT_EQ( 1, m3::math::circle_section_index( 9.424, 4 ) );
  EXPECT_FLOAT_EQ( 2, m3::math::circle_section_index( 9.425, 4 ) );
  EXPECT_FLOAT_EQ( 2, m3::math::circle_section_index( 10.995, 4 ) );
  EXPECT_FLOAT_EQ( 3, m3::math::circle_section_index( 10.996, 4 ) );
  EXPECT_FLOAT_EQ( 3, m3::math::circle_section_index( 12.566, 4 ) );
}
