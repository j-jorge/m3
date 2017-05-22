#include "m3/boost/signals.hpp"

template class boost::signals2::signal< void() >;
template
class boost::signals2::signal< void( const std::vector< std::size_t >& ) >;
