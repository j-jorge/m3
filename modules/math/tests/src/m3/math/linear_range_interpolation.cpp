#include "m3/math/linear_range_interpolation.hpp"

#include <gtest/gtest.h>

TEST( m3_math_linear_range_interpolation, positive_increasing_interval )
{
  EXPECT_FLOAT_EQ
    ( 10, m3::math::linear_range_interpolation( -2, 1, 2, 10, 20 ) );
  EXPECT_FLOAT_EQ
    ( 10, m3::math::linear_range_interpolation( 0.5, 1, 2, 10, 20 ) );
  EXPECT_FLOAT_EQ
    ( 10, m3::math::linear_range_interpolation( 1, 1, 2, 10, 20 ) );
  EXPECT_FLOAT_EQ
    ( 12.5, m3::math::linear_range_interpolation( 1.25, 1, 2, 10, 20 ) );
  EXPECT_FLOAT_EQ
    ( 15, m3::math::linear_range_interpolation( 1.5, 1, 2, 10, 20 ) );
  EXPECT_FLOAT_EQ
    ( 20, m3::math::linear_range_interpolation( 2, 1, 2, 10, 20 ) );
  EXPECT_FLOAT_EQ
    ( 20, m3::math::linear_range_interpolation( 8, 1, 2, 10, 20 ) );
}

TEST( m3_math_linear_range_interpolation, positive_decreasing_interval )
{
  EXPECT_FLOAT_EQ
    ( 20, m3::math::linear_range_interpolation( -2, 1, 2, 20, 10 ) );
  EXPECT_FLOAT_EQ
    ( 20, m3::math::linear_range_interpolation( 0.5, 1, 2, 20, 10 ) );
  EXPECT_FLOAT_EQ
    ( 20, m3::math::linear_range_interpolation( 1, 1, 2, 20, 10 ) );
  EXPECT_FLOAT_EQ
    ( 17.5, m3::math::linear_range_interpolation( 1.25, 1, 2, 20, 10 ) );
  EXPECT_FLOAT_EQ
    ( 15, m3::math::linear_range_interpolation( 1.5, 1, 2, 20, 10 ) );
  EXPECT_FLOAT_EQ
    ( 10, m3::math::linear_range_interpolation( 2, 1, 2, 20, 10 ) );
  EXPECT_FLOAT_EQ
    ( 10, m3::math::linear_range_interpolation( 8, 1, 2, 20, 10 ) );
}

TEST( m3_math_linear_range_interpolation, negative_decreasing_interval )
{
  EXPECT_FLOAT_EQ
    ( -10, m3::math::linear_range_interpolation( -2, 1, 2, -10, -20 ) );
  EXPECT_FLOAT_EQ
    ( -10, m3::math::linear_range_interpolation( 0.5, 1, 2, -10, -20 ) );
  EXPECT_FLOAT_EQ
    ( -10, m3::math::linear_range_interpolation( 1, 1, 2, -10, -20 ) );
  EXPECT_FLOAT_EQ
    ( -12.5, m3::math::linear_range_interpolation( 1.25, 1, 2, -10, -20 ) );
  EXPECT_FLOAT_EQ
    ( -15, m3::math::linear_range_interpolation( 1.5, 1, 2, -10, -20 ) );
  EXPECT_FLOAT_EQ
    ( -20, m3::math::linear_range_interpolation( 2, 1, 2, -10, -20 ) );
  EXPECT_FLOAT_EQ
    ( -20, m3::math::linear_range_interpolation( 8, 1, 2, -10, -20 ) );
}

TEST( m3_math_linear_range_interpolation, negative_increasing_interval )
{
  EXPECT_FLOAT_EQ
    ( -20, m3::math::linear_range_interpolation( -2, 1, 2, -20, -10 ) );
  EXPECT_FLOAT_EQ
    ( -20, m3::math::linear_range_interpolation( 0.5, 1, 2, -20, -10 ) );
  EXPECT_FLOAT_EQ
    ( -20, m3::math::linear_range_interpolation( 1, 1, 2, -20, -10 ) );
  EXPECT_FLOAT_EQ
    ( -17.5, m3::math::linear_range_interpolation( 1.25, 1, 2, -20, -10 ) );
  EXPECT_FLOAT_EQ
    ( -15, m3::math::linear_range_interpolation( 1.5, 1, 2, -20, -10 ) );
  EXPECT_FLOAT_EQ
    ( -10, m3::math::linear_range_interpolation( 2, 1, 2, -20, -10 ) );
  EXPECT_FLOAT_EQ
    ( -10, m3::math::linear_range_interpolation( 8, 1, 2, -20, -10 ) );
}

TEST( m3_math_linear_range_interpolation, negative_dates )
{
  EXPECT_FLOAT_EQ
    ( 20, m3::math::linear_range_interpolation( 2, -2, -1, 10, 20 ) );
  EXPECT_FLOAT_EQ
    ( 20, m3::math::linear_range_interpolation( -0.5, -2, -1, 10, 20 ) );
  EXPECT_FLOAT_EQ
    ( 20, m3::math::linear_range_interpolation( -1, -2, -1, 10, 20 ) );
  EXPECT_FLOAT_EQ
    ( 17.5, m3::math::linear_range_interpolation( -1.25, -2, -1, 10, 20 ) );
  EXPECT_FLOAT_EQ
    ( 15, m3::math::linear_range_interpolation( -1.5, -2, -1, 10, 20 ) );
  EXPECT_FLOAT_EQ
    ( 10, m3::math::linear_range_interpolation( -2, -2, -1, 10, 20 ) );
  EXPECT_FLOAT_EQ
    ( 10, m3::math::linear_range_interpolation( -8, -2, -1, 10, 20 ) );
}

TEST( m3_math_linear_range_interpolation, empty_interval )
{
  EXPECT_FLOAT_EQ
    ( 10, m3::math::linear_range_interpolation( -2, 1, 1, 10, 20 ) );
  EXPECT_FLOAT_EQ
    ( 10, m3::math::linear_range_interpolation( 1, 1, 1, 10, 20 ) );
  EXPECT_FLOAT_EQ
    ( 20, m3::math::linear_range_interpolation( 1.00001, 1, 1, 10, 20 ) );
}
