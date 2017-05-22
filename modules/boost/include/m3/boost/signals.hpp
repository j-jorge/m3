#pragma once

#include "m3/stl/vector.hpp"

#include <boost/signals2/signal.hpp>

extern template class boost::signals2::signal< void() >;
extern template
class boost::signals2::signal< void( const std::vector< std::size_t >& ) >;
