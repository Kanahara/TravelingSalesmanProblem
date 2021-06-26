#include "Result.hpp"
#pragma once

namespace okl {
	/*!
	* @brief			結果クラス用出力ストリーム
	* @return			std::ostream	出力ストリーム
	* @detail			解の値　算出時間を,区切りで表示
	*/
	template<typename  __Cost, class __CostComparisonFunction, typename __Time>
	inline std::ostream& operator<<(std::ostream& __os, const Result<__Cost, __CostComparisonFunction, __Time>& __result)
	{
		using cost_type = typename Result<__Cost, __CostComparisonFunction, __Time>::cost_type;
		if constexpr (std::is_floating_point_v<cost_type>) {
			__os << std::fixed << std::setprecision(8) << __result.get();
		}
		else if constexpr (std::is_same_v<cost_type, char>) {
			__os << static_cast<short>(__result.get());
		}
		else if constexpr (std::is_same_v<cost_type, signed char>) {
			__os << static_cast<signed short>(__result.get());
		}
		else if constexpr (std::is_same_v<cost_type, unsigned char>) {
			__os << static_cast<unsigned short>(__result.get());
		}
		else {
			__os << __result.get();
		}
		__os << ", " << __result.time();
		return __os;
	}
} /* namespace okl::mcp */