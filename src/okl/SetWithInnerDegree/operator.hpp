#include "SetWithInnerDegree.hpp"
#pragma once
namespace okl {
	/*!
	* @brief			次数付き頂点ID集合用出力ストリーム
	* @return			std::ostream	出力ストリーム
	* @detail
	*/
	template<typename __Id, typename __Degree, class __DegreeContainer, class __Set>
	inline std::ostream& operator<<(std::ostream& __os, const SetWithInnerDegree<__Id, __Degree, __DegreeContainer, __Set>& __set) {
		for (const auto _node_id : __set) {
			__os << std::setw(std::log10(__set.capacity()) + 2) << _node_id << ",";
		}
		if (!__set.isEmpty()) {
			__os << __os.widen('\n');
		}

		//for (const auto _node_id : __set) {
		//	if (__set.degree(_node_id) != std::numeric_limits < __Degree > ::max()) {
		//		__os << std::setw(std::log10(__set.capacity()) + 2) << __set.degree(_node_id) << ",";
		//	}
		//	else {
		//		__os << std::setw(std::log10(__set.capacity()) + 2) << "u,";
		//	}
		//}
		if (!__set.isEmpty()) {
			__os << __os.widen('\n');
		}

		return __os;
	}
} /* namespace okl */