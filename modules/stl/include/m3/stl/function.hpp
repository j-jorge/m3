#pragma once

#include "m3/stl/vector.hpp"

#include <functional>

extern template class std::function< void() >;
extern template
class std::function< void( const std::vector< std::size_t >& ) >;
