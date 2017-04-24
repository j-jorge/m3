#include "m3/find_adjacent_ring_gems.hpp"

namespace m3
{
  namespace detail
  {
    static void insert_range
    ( std::vector< std::size_t >& result, std::size_t begin, std::size_t end );

    static std::size_t find_first_not_of
    ( const std::vector< gem >& ring, gem key, std::size_t start );

    static std::size_t find_first_of_backward
    ( const std::vector< gem >& ring, gem key, std::size_t start );
  }
}

std::vector< std::size_t > m3::find_adjacent_ring_gems
( const std::vector< gem >& ring, std::size_t range_size )
{
  const std::size_t gem_count( ring.size() );
  std::vector< std::size_t > result;

  if ( gem_count == 0 )
    return result;

  result.reserve( gem_count );

  gem key( ring.front() );

  const std::size_t loop_end( detail::find_first_not_of( ring, key, 1 ) );
  const std::size_t loop_begin
    ( detail::find_first_of_backward( ring, key, gem_count ) );
  const bool insert_loop( gem_count - loop_begin + loop_end >= range_size );

  if ( insert_loop )
    detail::insert_range( result, 0, loop_end );
  
  std::size_t begin( loop_end );
  std::size_t end( loop_end );
  
  while ( end < loop_begin )
    {
      end = detail::find_first_not_of( ring, ring[ begin ], begin + 1 );

      if ( end - begin >= range_size )
        detail::insert_range( result, begin, end );
      
      begin = end;
    }
  
  if ( insert_loop && ( loop_begin > loop_end ) )
    detail::insert_range( result, loop_begin, gem_count );

  return result;
}

std::size_t m3::detail::find_first_not_of
( const std::vector< gem >& ring, gem key, std::size_t start )
{
  const std::size_t end( ring.size() );
  
  for ( std::size_t i( start ); i < end; ++i )
    if ( ring[ i ] != key )
      return i;

  return end;
}

std::size_t
m3::detail::find_first_of_backward
( const std::vector< gem >& ring, gem key, std::size_t start )
{
  std::size_t i( start );

  while ( i != 0 )
    {
      const std::size_t result( i );
      --i;

      if ( ring[ i ] != key )
        return result;
    }

  return 0;
}

void m3::detail::insert_range
( std::vector< std::size_t >& result, std::size_t begin, std::size_t end )
{
  for ( std::size_t i( begin ); i != end; ++i )
      result.emplace_back( i );
}
