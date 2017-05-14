#pragma once

#include "engine/base_item.hpp"
#include "engine/export.hpp"

namespace m3
{
  namespace app
  {
    class gem_ring;
    
    class camera:
      public bear::engine::base_item
    {
      DECLARE_BASE_ITEM( camera );

    public:
      camera();
      
      void set_ring( gem_ring& ring );

      void build() override;
      void progress( bear::universe::time_type elapsed_time ) override;

    private:
      typedef bear::engine::base_item super;

    private:
      gem_ring* m_ring;
    };
  }
}

