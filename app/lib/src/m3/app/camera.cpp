#include "m3/app/camera.hpp"

#include "m3/app/gem_ring.hpp"
#include "m3/app/config/get_config.hpp"

#include "engine/level.hpp"

#include <claw/tween/single_tweener.hpp>
#include <claw/tween/easing/easing_back.hpp>

BASE_ITEM_EXPORT( camera, m3::app );

m3::app::camera::camera()
  : m_ring( nullptr ),
    m_do_update( false )
{

}

void m3::app::camera::set_ring( gem_ring& ring )
{
  assert( m_ring == nullptr );
  m_ring = &ring;

  set_movement_reference( m_ring );
  m_ring->connect_to_radius_updated
    ( boost::bind( &camera::start_update, this ) );
}

void m3::app::camera::build()
{
  get_level().set_camera( *this );

  static const float margin( get_config< float >( "camera.margin" ) );
  static const float min_width( get_config< float >( "camera.minimal-width" ) );
  static const float height_ratio
    ( float( get_level().get_size().y ) / get_level().get_size().x );

  const float width
    ( std::max( min_width, ( m_ring->get_radius() + margin ) * 2 ) );

  set_size( width, width * height_ratio );
  set_center_of_mass( m_ring->get_center_of_mass() );
}

void m3::app::camera::progress( bear::universe::time_type elapsed_time )
{
  if ( !m_do_update )
    return;
  
  m_tweener.update( 1 );
  set_center_of_mass( m_ring->get_center_of_mass() );
}

void m3::app::camera::start_update()
{
  static const float margin( get_config< float >( "camera.margin" ) );
  static const float min_width( get_config< float >( "camera.minimal-width" ) );

  const float width
    ( std::max( min_width, ( m_ring->get_radius() + margin ) * 2 ) );

  static const float resize_duration
    ( get_config< float >( "camera.resize-duration" ) );
  
  m_tweener =
    claw::tween::single_tweener
    ( get_width(), width, resize_duration,
      boost::bind( &camera::update_size, this, _1 ),
      &claw::tween::easing_back::ease_out );

  m_tweener.on_finished( boost::bind( &camera::complete_update, this ) );

  m_do_update = true;
}

void m3::app::camera::update_size( float width )
{
  static const float height_ratio
    ( float( get_level().get_size().y ) / get_level().get_size().x );

  set_size( width, width * height_ratio );
}

void m3::app::camera::complete_update()
{
  m_do_update = false;
}
