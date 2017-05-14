#include "m3/app/input_controller.hpp"

#include "m3/app/gem_ring.hpp"
#include "m3/app/config/get_config.hpp"

#include "engine/level.hpp"

BASE_ITEM_EXPORT( input_controller, m3::app );

m3::app::input_controller::input_controller()
  : m_ring( nullptr ),
    m_down( false )
{

}

void m3::app::input_controller::set_ring( gem_ring& ring )
{
  assert( m_ring == nullptr );
  m_ring = &ring;
}

void m3::app::input_controller::progress
( bear::universe::time_type elapsed_time )
{
  m_input_status.read();
  m_input_status.scan_inputs( *this );
}

bool m3::app::input_controller::mouse_pressed
( bear::input::mouse::mouse_code button,
  const claw::math::coordinate_2d< unsigned int >& pos )
{
  down( pos );
  return true;
}

bool m3::app::input_controller::mouse_released
( bear::input::mouse::mouse_code button,
  const claw::math::coordinate_2d< unsigned int >& pos )
{
  if ( !m_down )
    return false;

  up( pos );
  return true;
}

bool m3::app::input_controller::mouse_move
( const claw::math::coordinate_2d< unsigned int >& pos )
{
  if ( !m_down )
    return false;

  move( pos );
  return true;
}

bool m3::app::input_controller::finger_action
( const bear::input::finger_event& event )
{
  switch( event.get_type() )
    {
    case bear::input::finger_event::event_type::finger_event_pressed:
      down( event.get_position() );
      return true;
      
    case bear::input::finger_event::event_type::finger_event_motion:
      if ( ( event.get_distance().x != 0 ) || ( event.get_distance().y != 0 ) )
        move( event.get_position() );
      return true;
      
    case bear::input::finger_event::event_type::finger_event_released:
      up( event.get_position() );
      return true;
      
    default:
      assert( false );
    }
  
  return false;
}

void m3::app::input_controller::down
( const bear::universe::position_type& position )
{
  const bear::universe::position_type direction
    ( get_level().screen_to_level( position ) - m_ring->get_center_of_mass() );
  const double angle( m_ring->get_system_angle() );

  m_angle_offset = angle - std::atan2( direction.y, direction.x );
  m_speed_reference_angle = angle;
  m_speed_reference_date = get_age();
  
  m_down = true;
  m_last_move_date = m_speed_reference_date;
  m_last_move_position = position;
  m_ring->set_angular_speed( 0 );
}

void m3::app::input_controller::move
( const bear::universe::position_type& position )
{
  static const float move_threshold
    ( get_config< float >( "input.move.threshold" ) );
  
  if ( ( std::abs( position.x - m_last_move_position.x ) < move_threshold )
       && ( std::abs( position.y - m_last_move_position.y ) < move_threshold ) )
    return;
  
  const bear::universe::position_type direction
    ( get_level().screen_to_level( position ) - m_ring->get_center_of_mass() );

  const double angle( std::atan2( direction.y, direction.x ) + m_angle_offset );
  const bool clockwise( angle < m_ring->get_system_angle() );
  m_ring->set_system_angle( angle );

  const bear::universe::time_type now( get_age() );
  static const bear::universe::time_type motionless_threshold
    ( get_config< float >( "input.velocity.motionless.threshold" ) );

  if ( ( m_last_move_date - now >= motionless_threshold )
       || ( clockwise != m_moving_clockwise ) )
    {
      m_moving_clockwise = clockwise;
      m_speed_reference_date = now;
      m_speed_reference_angle = angle;
    }

  m_last_move_date = now;
  m_last_move_position = position;
}

void m3::app::input_controller::up
( const bear::universe::position_type& position )
{
  m_down = false;

  const bear::universe::time_type now( get_age() );
  const bear::universe::time_type movement_duration
    ( now - m_speed_reference_date );

  static const bear::universe::time_type motionless_threshold
    ( get_config< float >( "input.velocity.motionless.threshold" ) );

  double speed( 0 );
  
  if ( ( m_last_move_date - now <= motionless_threshold )
       && ( movement_duration > 0 ) )
    speed =
      ( m_ring->get_system_angle() - m_speed_reference_angle )
      / movement_duration;
  
  static const float min_velocity
    ( get_config< float >( "input.velocity.min" ) );
  static const float max_velocity
    ( get_config< float >( "input.velocity.max" ) );

  if ( speed > max_velocity )
    speed = max_velocity;
  else if ( speed < -max_velocity )
    speed = -max_velocity;
  else if ( std::abs( speed ) < min_velocity )
    {
      static const float max_random_velocity
        ( get_config< float >( "input.velocity.random.max" ) );

      speed =
        ( m_moving_clockwise ? -1 : 1 )
        * ( min_velocity
            + rand() / RAND_MAX * ( max_random_velocity - min_velocity ) );
    }

  m_ring->set_angular_speed( speed );
}
