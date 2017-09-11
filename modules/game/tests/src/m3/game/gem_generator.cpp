#include "m3/game/gem_generator.hpp"

#include <gtest/gtest.h>

TEST( gem_generator, random_one )
{
  m3::game::gem_generator generator( 1 );

  for ( std::size_t i( 0 ); i != 1000; ++i )
    EXPECT_EQ( m3::game::gem( 1 ), generator.random() );
}

TEST( gem_generator, random_two )
{
  m3::game::gem_generator generator( 2 );

  for ( std::size_t i( 0 ); i != 1000; ++i )
    {
      EXPECT_LE( m3::game::gem( 1 ), generator.random() );
      EXPECT_GE( m3::game::gem( 2 ), generator.random() );
    }
}

TEST( gem_generator, random_three )
{
  m3::game::gem_generator generator( 3 );

  for ( std::size_t i( 0 ); i != 1000; ++i )
    {
      EXPECT_LE( m3::game::gem( 1 ), generator.random() );
      EXPECT_GE( m3::game::gem( 3 ), generator.random() );
    }
}

TEST( gem_generator, random_four )
{
  m3::game::gem_generator generator( 4 );

  for ( std::size_t i( 0 ); i != 1000; ++i )
    {
      EXPECT_LE( m3::game::gem( 1 ), generator.random() );
      EXPECT_GE( m3::game::gem( 4 ), generator.random() );
    }
}

TEST( gem_generator, next_one )
{
  m3::game::gem_generator generator( 1 );

  EXPECT_EQ( m3::game::gem( 1 ), generator.next( 1 ) );
}

TEST( gem_generator, next_two )
{
  m3::game::gem_generator generator( 2 );

  EXPECT_EQ( m3::game::gem( 2 ), generator.next( 1 ) );
  EXPECT_EQ( m3::game::gem( 1 ), generator.next( 2 ) );
}

TEST( gem_generator, next_three )
{
  m3::game::gem_generator generator( 3 );

  EXPECT_EQ( m3::game::gem( 2 ), generator.next( 1 ) );
  EXPECT_EQ( m3::game::gem( 3 ), generator.next( 2 ) );
  EXPECT_EQ( m3::game::gem( 1 ), generator.next( 3 ) );
}

TEST( gem_generator, next_four )
{
  m3::game::gem_generator generator( 4 );

  EXPECT_EQ( m3::game::gem( 2 ), generator.next( 1 ) );
  EXPECT_EQ( m3::game::gem( 3 ), generator.next( 2 ) );
  EXPECT_EQ( m3::game::gem( 4 ), generator.next( 3 ) );
  EXPECT_EQ( m3::game::gem( 1 ), generator.next( 4 ) );
}
