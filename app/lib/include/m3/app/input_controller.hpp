#pragma once

#include "engine/base_item.hpp"
#include "engine/export.hpp"
#include "input/input_listener.hpp"
#include "input/input_status.hpp"

namespace m3
{
  namespace app
  {
    class gem_ring;
    
    class input_controller:
      public bear::engine::base_item,
      public bear::input::input_listener      
    {
      DECLARE_BASE_ITEM( input_controller );

    public:
      input_controller();
      
      void set_ring( gem_ring& ring );

      void progress( bear::universe::time_type elapsed_time ) override;

    private:
      typedef bear::engine::base_item super;

    private:
      bool mouse_pressed
      ( bear::input::mouse::mouse_code button,
        const claw::math::coordinate_2d< unsigned int >& pos ) override;
      bool mouse_released
      ( bear::input::mouse::mouse_code button,
        const claw::math::coordinate_2d< unsigned int >& pos ) override;
      bool mouse_move
      ( const claw::math::coordinate_2d< unsigned int >& pos ) override;

      bool finger_action( const bear::input::finger_event& event ) override;

      void down( const bear::universe::position_type& position );
      void move( const bear::universe::position_type& position );
      void up( const bear::universe::position_type& position );
      
    private:
      bear::input::input_status m_input_status;
      gem_ring* m_ring;

      bool m_down;
      bear::universe::time_type m_speed_reference_date;
      double m_speed_reference_angle;
      bear::universe::time_type m_last_move_date;
      bear::universe::position_type m_last_move_position;
      double m_angle_offset;
      bool m_moving_clockwise;
    };
  }
}

