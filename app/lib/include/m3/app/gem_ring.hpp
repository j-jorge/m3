#pragma once

#include "m3/game/game_loop.hpp"
#include "m3/game/gem_ring.hpp"

#include "m3/boost/signals.hpp"
#include "m3/stl/function.hpp"

#include "engine/base_item.hpp"
#include "engine/export.hpp"

#include <claw/tween/tweener.hpp>

namespace m3
{
  namespace app
  {
    class gem_ring:
      public bear::engine::base_item
    {
      DECLARE_BASE_ITEM( gem_ring );

    public:
      gem_ring();
      gem_ring( const gem_ring& that );
      
      float get_radius() const;

      boost::signals2::connection connect_to_radius_updated
      ( const std::function< void() >& f );
      
      void pre_cache() override;
      void build() override;
      void progress( bear::universe::time_type elapsed_time ) override;
      void get_visual
      ( std::list< bear::engine::scene_visual >& visuals ) const override;

    private:
      typedef bear::engine::base_item super;

    private:
      void configure_game_loop();

      void fill_gem_sprites();
      void fill_default_gem_sprites( unsigned int gem_type_count );
      void fill_glowing_gem_sprites( unsigned int gem_type_count );

      void fill_game_over_bounds_visuals();
      
      void get_game_over_bounds_visuals
      ( std::list< bear::engine::scene_visual >& visuals ) const;
      void get_ring_visuals
      ( std::list< bear::engine::scene_visual >& visuals ) const;
      void get_launched_visuals
      ( std::list< bear::engine::scene_visual >& visuals ) const;
      void get_launcher_visual
      ( std::list< bear::engine::scene_visual >& visuals ) const;

      void enter_radius_animation_state();
      void animate_radius_change();
      void update_radius();
      void complete_radius_state();
      
      void enter_expansion_state();
      void update_expansion();

      void set_launcher_visuals();
      float get_ring_radius() const;

      float get_expansion_rate() const;
      float get_expansion_speed() const;

      void update_glow();
      
    private:
      const unsigned int m_win_size;
      const unsigned int m_lose_size;
      
      std::vector< bear::engine::scene_visual > m_game_over_bounds_visuals;

      std::vector< bear::visual::sprite > m_gem_sprite;
      std::vector< bear::visual::sprite > m_glow_sprite;
      
      m3::game::gem_ring m_ring;
      m3::game::game_loop m_game_loop;
      std::vector< bear::engine::scene_visual > m_coming_next_visuals;
      std::vector< float > m_glow;
      float m_radius;
      int m_state;
      claw::tween::tweener m_radius_tweener;
      claw::tween::tweener m_glow_tweener;

      boost::signals2::signal< void() > m_radius_updated;
    };
  }
}
