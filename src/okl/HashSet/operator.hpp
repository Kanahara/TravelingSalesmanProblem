#pragma once
namespace okl {
	/*!
	* @brief			ハッシュ集合クラス用出力ストリーム
	* @return			std::ostream	出力ストリーム
	* @detail
	*/
	template<class __Value, typename __Key, class __Hash, class __Store, class __Location>
	inline std::ostream& operator<<(std::ostream& __os, const HashSet<__Value, __Key, __Hash, __Store, __Location >& __set) {
		for (const auto _data : __set) {
			__os << std::setw(std::log10(__set.capacity()) + 2) << _data << ",";
		}
		if (!__set.isEmpty()) {
			__os << __os.widen('\n');
		}
		return __os;
	}
}/* namespace okl */