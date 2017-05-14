#include "m3/app/config/get_config.hpp"

#include "engine/resource_pool.hpp"

#include <claw/logger.hpp>

#include <rapidjson/document.h>

#include <sstream>

namespace m3
{
  namespace app
  {
    namespace detail
    {
      static const rapidjson::Document& get_config();
      static rapidjson::Document load_config();
    }
  }
}

template<>
float m3::app::get_config< float >( const char* key )
{
    return detail::get_config()[ key ].GetDouble();
}

template<>
unsigned int m3::app::get_config< unsigned int >( const char* key )
{
    return detail::get_config()[ key ].GetUint();
}

const rapidjson::Document& m3::app::detail::get_config()
{
  static rapidjson::Document result( detail::load_config() );
  return result;
}

rapidjson::Document m3::app::detail::load_config()
{
  std::stringstream s;
  bear::engine::resource_pool::get_instance().get_file
    ( "config/global.json", s );

  rapidjson::Document result;

  if ( result.Parse( s.str().c_str() ).HasParseError() )
    claw::logger << claw::log_error << "Failed to parse the config file.\n";

  return result;
}

