#include "m3/table/generate_fragmented_indices.hpp"

#include <algorithm>
#include <cassert>

std::vector< std::size_t > m3::table::generate_fragmented_indices
( std::size_t count, const std::vector< std::size_t >& skip )
{
  assert( std::is_sorted( skip.begin(), skip.end() ) );
  const std::size_t skip_size( skip.size() );
  const std::size_t none( count );
  
  std::vector< std::size_t > result( count );
  std::size_t j( 0 );
  std::size_t offset( 0 );

  for ( std::size_t i( 0 ); i != count; ++i )
    if ( ( j != skip_size ) && ( i == skip[ j ] ) )
      {
        ++j;
        ++offset;
        result[ i ] = none;
      }
    else
      result[ i ] = i - offset;

  return result;
}
