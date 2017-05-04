#pragma once

#include "m3/gem_ring.hpp"

#include "engine/base_item.hpp"
#include "engine/export.hpp"

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
      void fill_gem_sprites();
      void initialize_ring();
      
      float get_expansion_rate() const;
      
    private:
      std::vector< bear::visual::sprite > m_gem_sprite;
      std::uint64_t m_date;
      m3::gem_ring m_ring;
    };
  }
}
