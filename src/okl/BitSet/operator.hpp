#include "BitSet.hpp"
#pragma once
namespace okl {
	/*!
	* @brief			ビット集合クラス用出力ストリーム
	* @return			std::ostream	出力ストリーム
	* @detail
	*/
	template<typename __Value, typename __Key, class __Store>
	inline std::ostream& operator<<(std::ostream& __os, const BitSet<__Value, __Key, __Store >& __set) {
		std::cout << __set.capacity() << "," << sizeof(__Value) << std::endl;
		for (std::size_t _index = 0; _index < __set.capacity(); ++_index) {
			__os << __set[_index] << ",";
		}
		return __os;
	}
}/* namespace okl */