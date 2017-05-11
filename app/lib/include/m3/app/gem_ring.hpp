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
      void enter_radius_animation_state();
      void update_radius();
      
      void enter_expansion_state();
      void update_expansion();
      
      float get_ring_radius();

      void fill_gem_sprites();
      void initialize_ring();
      
      float get_expansion_rate() const;
      
    private:
      std::vector< bear::visual::sprite > m_gem_sprite;
      std::uint64_t m_date;
      m3::gem_ring m_ring;
      float m_radius;
      int m_state;
      claw::tween::tweener m_radius_tweener;
    };
  }
}
