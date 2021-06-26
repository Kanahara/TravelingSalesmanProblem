#pragma once

#include "../Matrix.hpp"
#include "../..//BitSet/module.hpp"
namespace okl {
	/*!
	 *  @brief      単純な正則Bit行列クラス
	 *  @details    単純な正則Bit行列クラス
	 * 				要素を格納・取得するだけのシンプルな可変長行列クラス
	 *				- __Value			Bit格納unsigned要素型
	 *				- __Key				インデックス数値型
	 *				- __Row				行配列コンテナ
	 *				- __Matrix			行列コンテナ
	 */
	template<
		typename __Value,
		typename __Key,
		class __Row = okl::BitSet<__Value, __Key>,
		class __Matrix = std::vector<__Row>>
		class BitMatrix : public Matrix<__Value, __Key> {
		public:
			/*!
			 * @brief				コンストラクタ
			 */
			BitMatrix() = default;

			/*!
			 * @brief				デストラクタ
			 */
			virtual ~BitMatrix() = default;

			using value_type = typename Matrix<__Value, __Key>::value_type;
			using key_type = typename Matrix<__Value, __Key>::key_type;
			using size_type = typename Matrix<__Value, __Key>::size_type;
			using symmetric = std::false_type;

			/*!
			 * @brief			全要素初期化
			 * @detail			全要素を初期化処理を行う
			 *					メモリ解放は行わない
			 */
			inline void clear() override final
			{
				Matrix<__Value, __Key>::clear();
				std::for_each(this->matrix_.begin(), this->matrix_.end(),
					[](auto& _row) { _row.clear(); });
			}

			/*!
			 * @brief			サイズ設定
			 * @param[in]		__size 正則行列の行と列のサイズ
			 * @detail			全要素をメモリ確保を行う
			 */
			inline void resize(const size_type __size) override final
			{
				Matrix<__Value, __Key>::resize(__size);
				this->matrix_.resize(__size);
				std::for_each(this->matrix_.begin(), this->matrix_.end(),
					[__size](auto& _row) { _row.reserve(__size); });
			}

			/*!
			 * @brief			行列の要素を設定する
			 * @param[in]		__row 行番号
			 * @param[in]		__column 列番号
			 * @param[in]		__value 値
			 * @detail			__row 行番号　__column 列番号で指定される要素の設定を行う
			 *					 - value_typeはテンプレート__Valueによって定義される
			 */
			inline void set(const key_type __row, const key_type __column, const value_type __value)
			{
				if (__value == false) {
					this->matrix_[__row].reset(__column);
				}
				else {
					this->matrix_[__row].set(__column);
				}
			}

			/*!
			 * @brief			行列の要素を返す
			 * @return			value_type	行列の要素を返す
			 * @param[in]		__row 行番号
			 * @param[in]		__column 列番号
			 * @detail			__row 行番号　__column 列番号で指定される要素を返す
			 *					 - value_typeはテンプレート__Valueによって定義される
			 */
			constexpr bool operator()(const key_type __row, const key_type __column) const
			{
				return this->matrix_[__row][__column];
			}

		private:
			//!行列コンテナ
			__Matrix matrix_;
	};
} /* namespace okl */

#include "operator.hpp"