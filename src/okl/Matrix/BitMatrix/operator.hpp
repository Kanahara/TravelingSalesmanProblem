#pragma once
namespace okl {
	/*!
	* @brief			単純な正則行列クラス出力ストリーム
	* @return			std::ostream	出力ストリーム
	* @detail
	*/
	template<typename __Value, typename __Key, class __Row, class __Matrix>
	inline std::ostream& operator<<(std::ostream& __os, const BitMatrix<__Value, __Key, __Row, __Matrix >& __matrix) {
		using key_type = typename SimpleMatrix<__Value, __Key, __Row, __Matrix >::key_type;
		for (key_type _i = 0; _i < __matrix.size(); ++_i) {
			for (key_type _j = 0; _j < __matrix.size(); ++_j) {
				__os << +__matrix(_i, _j) << ", ";
			}
			__os << __os.widen('\n');
		}
		__os << __os.widen('\n');

		return __os;
	}
} /* namespace okl */