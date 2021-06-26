#pragma once
namespace okl {
	/*!
	* @brief			対象正則行列クラス用出力ストリーム
	* @return			std::ostream	出力ストリーム
	* @detail
	*/
	template<typename __Value, typename __Key, bool SiagonalComponent, class __Container>
	inline std::ostream& operator<<(std::ostream& __os, const SymmetricBitMatrixWithSameSiagonalComponent<__Value, __Key, SiagonalComponent, __Container>& __matrix) {
		using key_type = typename SymmetricBitMatrixWithSameSiagonalComponent<__Value, __Key, SiagonalComponent, __Container>::key_type;
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