#include "m3/app/create_initial_level.hpp"

#include "m3/app/gem_ring.hpp"
#include "m3/app/input_controller.hpp"

#include "generic_items/layer/action_layer.hpp"

#include "engine/level.hpp"

namespace m3
{
  namespace app
  {
    namespace detail
    {
      static gem_ring* create_ring();
      static input_controller* create_input_controller( gem_ring& ring );

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

  gem_ring* const ring( detail::create_ring() );
  layer->add_item( *ring );
  layer->add_item( *detail::create_input_controller( *ring ) );
  
  return result;
}

m3::app::gem_ring* m3::app::detail::create_ring()
{
  m3::app::gem_ring* const result( new m3::app::gem_ring() );
  result->set_center_of_mass( level_size / 2 );
  return result;
}

m3::app::input_controller*
m3::app::detail::create_input_controller( gem_ring& ring )
{
  input_controller* const result( new input_controller() );
  result->set_center_of_mass( level_size / 2 );
  result->set_ring( ring );
  return result;
}

