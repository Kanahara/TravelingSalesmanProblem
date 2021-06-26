#pragma once
namespace okl {
	/*!
	* @brief			対象正則行列クラス用出力ストリーム
	* @return			std::ostream	出力ストリーム
	* @detail
	*/
	template<typename __Value, typename __Key, class __Container>
	inline std::ostream& operator<<(std::ostream& __os, const SymmetricBitMatrix<__Value, __Key, __Container>& __matrix) {
		using key_type = typename SymmetricBitMatrix<__Value, __Key, __Container>::key_type;
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