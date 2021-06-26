#include "Solution.hpp"
#pragma once

namespace okl::tsp {
	//出力ストリーム演算子
	template<
		class __Graph,
		class __Cost,
		class __Set,
		class __RouteContainer>
		inline std::ostream& operator<<(std::ostream& __os, const Solution<__Graph, __Cost, __Set, __RouteContainer>& __solution) {
		__os << __solution.cost() << __os.widen('\n');

		std::size_t _size = __solution.nodes().capacity();

		for (const auto _route_roder : __solution.route()) {
			__os << std::setw(std::log10(_size) + 2) << _route_roder << ",";
		}
		__os << __os.widen('\n');

		return __os;
	}
} /* namespace okl::tsp */