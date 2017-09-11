#pragma once

#include "m3/game/gem_generator.hpp"

#include "m3/boost/signals.hpp"
#include "m3/stl/function.hpp"

namespace m3
{
  namespace game
  {
    class gem_ring;
  
    class game_loop
    {
    public:
      game_loop( gem_ring& ring, const gem_generator& generator );

      boost::signals2::connection connect_to_destroyed
      ( const std::function< void( const std::vector< std::size_t >& ) >& f );
      boost::signals2::connection connect_to_inserted
      ( const std::function< void() >& f );
      boost::signals2::connection connect_to_launcher_updated
      ( const std::function< void() >& f );
    
      const std::vector< gem >& coming_next() const;
      unsigned int date() const;
    
      void launch_speed_up_time_range
      ( unsigned int start_date, unsigned int end_date );
      void launch_interval_range( unsigned int min, unsigned int max );

      void launch_count_increment_time_range
      ( unsigned int start_date, unsigned int end_date );
      void launch_count_interval_range( unsigned int min, unsigned int max );

      void match_size( unsigned int size );
      unsigned int match_size() const;
    
      void start( unsigned int initial_gem_count );
      void tick( float expansion_rate );

    private:
      void update( float expansion_rate );
      void launch_gem();
      void update_coming_next();
      std::size_t launch_count() const;
      void update_next_launch_date();

    private:
      gem_ring& m_ring;
      gem_generator m_generator;
    
      unsigned int m_date;
      unsigned int m_launch_speed_up_start_date;
      unsigned int m_launch_speed_up_end_date;
      unsigned int m_launch_interval_min;
      unsigned int m_launch_interval_max;
      unsigned int m_launch_count_increment_start_date;
      unsigned int m_launch_count_increment_end_date;
      unsigned int m_launch_count_interval_min;
      unsigned int m_launch_count_interval_max;
      unsigned int m_match_size;
    
      std::vector< gem > m_coming_next;
      unsigned int m_next_launch_date;
    
      boost::signals2::signal< void( const std::vector< std::size_t >& ) >
      m_destroyed;
    
      boost::signals2::signal< void() > m_inserted;
      boost::signals2::signal< void() > m_launcher_updated;
    };
  }
}
