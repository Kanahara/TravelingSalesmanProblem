#pragma once
namespace okl {
	/*!
	* @brief			ハッシュ集合クラス用出力ストリーム
	* @return			std::ostream	出力ストリーム
	* @detail
	*/
	template<class __Value, class __Set, class __SizeContainer>
	inline std::ostream& operator<<(std::ostream& __os, const TypeSet<__Value, __Set, __SizeContainer>& __set) {
		for (const auto _color : __set) {
			__os << std::setw(std::log10(__set.capacity()) + 2) << _color << ",";
		}
		if (!__set.isEmpty()) {
			__os << __os.widen('\n');
		}
		for (const auto _color : __set) {
			__os << std::setw(std::log10(__set.capacity()) + 2) << __set.type_size(_color) << ",";
		}
		if (!__set.isEmpty()) {
			__os << __os.widen('\n');
		}
		return __os;
	}
}/* namespace okl */