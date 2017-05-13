#pragma once

#include "m3/gem_ring.hpp"

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
      
      void pre_cache() override;
      void build() override;
      void progress( bear::universe::time_type elapsed_time ) override;
      void get_visual
      ( std::list< bear::engine::scene_visual >& visuals ) const override;
      
    private:
      typedef bear::engine::base_item super;

    private:
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

      void launch_gem();
      void update_coming_next();
      void update_next_launch_date();

      bool remove_matches();
      
      float get_ring_radius() const;

      void fill_gem_sprites();
      void initialize_ring();
      
      float get_expansion_rate() const;
      float get_expansion_speed() const;
      
    private:
      std::vector< bear::visual::sprite > m_gem_sprite;
      std::uint64_t m_date;
      std::uint64_t m_next_launch_date;
      m3::gem_ring m_ring;
      m3::gem m_coming_next;
      bear::engine::scene_visual m_coming_next_visual;
      float m_radius;
      int m_state;
      claw::tween::tweener m_radius_tweener;
    };
  }
}
