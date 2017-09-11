#include "m3/game/random_ring_without_match.hpp"

#include "m3/game/gem_generator.hpp"
#include "m3/game/gem_ring.hpp"

#include <cassert>

void m3::game::random_ring_without_match
( gem_ring& ring, const gem_generator& generator, unsigned int count,
  unsigned int match_size )
{
  assert( count >= 2 );
  assert( match_size >= 3 );
  
  const gem first_gem( generator.random() );
  ring.launch( 0, first_gem );

  gem last_gem( first_gem );
  unsigned int last_count( 1 );
  
  for ( unsigned int i( 0 ); i != count - 2; ++i )
    {
      gem g( generator.random() );

      if ( g != last_gem )
        {
          last_count = 1;
          last_gem = g;
        }
      else if ( last_count + 1 != match_size )
        ++last_count;
      else
        {
          g = generator.next( g );
          last_gem = g;
        }

      ring.launch( 0, g );
    }

  gem final_gem( generator.random() );

  if ( final_gem == first_gem )
    final_gem = generator.next( final_gem );

  if ( final_gem == last_gem )
    {
      final_gem = generator.next( final_gem );
  
      if ( final_gem == first_gem )
        final_gem = generator.next( final_gem );
    }

  ring.launch( 0, final_gem );
  ring.expand( 1 );
}
