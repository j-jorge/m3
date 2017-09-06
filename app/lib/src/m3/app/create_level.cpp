#include "m3/app/create_level.hpp"

#include "m3/app/camera.hpp"
#include "m3/app/gem_ring.hpp"
#include "m3/app/input_controller.hpp"
#include "m3/app/config/get_config.hpp"

#include "generic_items/decorative_item.hpp"
#include "generic_items/layer/action_layer.hpp"

#include "engine/level.hpp"

namespace m3
{
  namespace app
  {
    namespace detail
    {
      static bear::decorative_item* create_background
      ( bear::engine::level_globals& globals,
        const bear::universe::position_type& position );
      static gem_ring* create_ring
      ( const bear::universe::position_type& position );
      static input_controller* create_input_controller
      ( gem_ring& ring, const bear::universe::position_type& position );
      static camera* create_camera
      ( gem_ring& ring, const bear::universe::position_type& position );
    }
  }
}

bear::engine::level* m3::app::create_level()
{
  static const bear::universe::size_box_type level_size
    ( get_config< float >( "level.width" ),
      get_config< float >( "level.height" ) );

  bear::engine::level* const result
    ( new bear::engine::level( "m3", "", level_size, "", nullptr, nullptr ) );
  
  bear::engine::layer* const layer( new bear::action_layer( level_size ) );

  result->push_layer( layer );

  static const bear::universe::position_type position( level_size / 2 );
  gem_ring* const ring( detail::create_ring( position ) );
  layer->add_item( *ring );
  layer->add_item( *detail::create_input_controller( *ring, position ) );
  layer->add_item( *detail::create_camera( *ring, position ) );

  layer->add_item
    ( *detail::create_background( result->get_globals(), position ) );

  return result;
}

bear::decorative_item* m3::app::detail::create_background
( bear::engine::level_globals& globals,
  const bear::universe::position_type& position )
{
  bear::decorative_item* result( new bear::decorative_item() );

  bear::visual::sprite sprite( globals.get_image( "background.png" ) );

  result->set_z_position( -1000 );
  result->set_animation( sprite );
  result->set_bounding_box
    ( bear::universe::rectangle_type( 0, 0, sprite.width(), sprite.height() ) );
  result->set_center_of_mass( position );
  
  return result;
}

m3::app::gem_ring*
m3::app::detail::create_ring( const bear::universe::position_type& position )
{
  m3::app::gem_ring* const result( new m3::app::gem_ring() );
  result->set_center_of_mass( position );
  return result;
}

m3::app::input_controller*
m3::app::detail::create_input_controller
( gem_ring& ring, const bear::universe::position_type& position )
{
  input_controller* const result( new input_controller() );
  result->set_center_of_mass( position );
  result->set_ring( ring );
  return result;
}

m3::app::camera* m3::app::detail::create_camera
( gem_ring& ring, const bear::universe::position_type& position )
{
  camera* const result( new camera() );
  result->set_center_of_mass( position );
  result->set_ring( ring );
  return result;
}
