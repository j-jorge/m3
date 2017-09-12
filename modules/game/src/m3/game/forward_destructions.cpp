#include "m3/game/forward_destructions.hpp"

#include "m3/game/find_adjacent_ring_gems.hpp"
#include "m3/game/gem_ring.hpp"
#include "m3/table/generate_fragmented_indices.hpp"

#include <algorithm>
#include <cassert>

namespace m3
{
  namespace game
  {
    namespace detail
    {
      static std::vector< std::size_t > indices_after_expansion
      ( gem_ring& ring );
    }
  }
}
#include <iostream>
std::vector< bool > m3::game::forward_destructions
( const gem_ring& source, unsigned int match_size )
{
  std::vector< bool > result( source.chain().size(), false );

  gem_ring ring( source );
  std::vector< std::size_t > index( detail::indices_after_expansion( ring ) );
  const std::size_t guard( index.size() );

  std::vector< std::size_t > removed;

  do
    {
      const std::vector< gem >& chain( ring.chain() );
      assert( index.size() == chain.size() );
      
      removed = find_adjacent_ring_gems( chain, match_size );

      const auto rend( removed.rend() );
      
      for ( auto it( removed.rbegin() ); it != rend; ++it )
        {
          const auto rit( index.begin() + *it );

          if ( *rit != guard )
            result[ *rit ] = true;
          
          index.erase( rit );
        }

      ring.erase( removed );
      assert( index.size() == ring.chain().size() );
    }
  while( !removed.empty() );

  return result;
}

std::vector< std::size_t > m3::game::detail::indices_after_expansion
( gem_ring& ring )
{
  std::vector< std::size_t > inserted( ring.expand( 1 ) );
  std::sort( inserted.begin(), inserted.end() );
  const std::size_t ring_size( ring.chain().size() );
  
  return m3::table::generate_fragmented_indices( ring_size, inserted );
}
