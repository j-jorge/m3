#pragma once

namespace m3
{
  namespace app
  {
    template< typename T >
    T get_config( const char* key );

    template<>
    float get_config< float >( const char* key );

    template<>
    unsigned int get_config< unsigned int >( const char* key );
  }
}
