#include "m3/app/camera.hpp"

#include "m3/app/gem_ring.hpp"
#include "m3/app/config/get_config.hpp"

#include "engine/level.hpp"

BASE_ITEM_EXPORT( camera, m3::app );

m3::app::camera::camera()
  : m_ring( nullptr )
{

}

void m3::app::camera::set_ring( gem_ring& ring )
{
  assert( m_ring == nullptr );
  m_ring = &ring;

  set_movement_reference( m_ring );
}

void m3::app::camera::build()
{
  get_level().set_camera( *this );
}

void m3::app::camera::progress( bear::universe::time_type elapsed_time )
{
  static const float height_ratio
    ( float( get_level().get_size().y ) / get_level().get_size().x );
  static const float margin( get_config< float >( "camera.margin" ) );
  static const float min_width( get_config< float >( "camera.minimal_width" ) );

  const float width
    ( std::max( min_width, ( m_ring->get_radius() + margin ) * 2 ) );

  set_size( width, width * height_ratio );
  set_center_of_mass( m_ring->get_center_of_mass() );
}
