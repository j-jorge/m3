#include "m3/app/create_initial_level.hpp"

#include "m3/app/gem_ring.hpp"

#include "generic_items/layer/action_layer.hpp"

#include "engine/level.hpp"

namespace m3
{
  namespace app
  {
    namespace detail
    {
      static m3::app::gem_ring* create_ring();

      static const bear::universe::size_box_type level_size( 720, 1280 );
    }
  }
}

bear::engine::level* m3::app::create_initial_level()
{
  bear::engine::level* const result
    ( new bear::engine::level
      ( "m3", "", detail::level_size, "", nullptr, nullptr ) );

  bear::engine::layer* const layer
    ( new bear::action_layer( detail::level_size ) );
  result->push_layer( layer );

  layer->add_item( *detail::create_ring() );
  
  return result;
}

m3::app::gem_ring* m3::app::detail::create_ring()
{
  m3::app::gem_ring* const result( new m3::app::gem_ring() );
  result->set_center_of_mass( level_size / 2 );
  return result;
}
