#include "m3/game/gem_generator.hpp"

#include <cassert>
#include <cstdlib>

m3::game::gem_generator::gem_generator( unsigned int gem_type_count )
  : _gem_type_count( gem_type_count )
{
  assert( gem_type_count >= 1 );
}

m3::game::gem m3::game::gem_generator::random() const
{
  return std::rand() % _gem_type_count + 1;
}

m3::game::gem m3::game::gem_generator::next( gem g ) const
{
  assert( g > 0 );
  assert( g <= _gem_type_count );
  
  if ( g == _gem_type_count )
    return 1;

  return g + 1;
}
