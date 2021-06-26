#pragma once

#include "../Matrix.hpp"

namespace okl {
	/*!
	 *  @brief      単純な正則行列クラス
	 *  @details    単純な正則行列クラス
	 * 				要素を格納・取得するだけのシンプルな可変長行列クラス
	 *				- __Value			要素型
	 *				- __Key				インデックス数値型
	 *				- __Row				行配列コンテナ
	 *				- __Matrix			行列コンテナ
	 */
	template<
		typename __Value,
		typename __Key,
		class __Row = std::vector<__Value>,
		class __Matrix = std::vector<__Row>>
		class SimpleMatrix : public Matrix<__Value, __Key> {
		public:
			/*!
			 * @brief				コンストラクタ
			 */
			SimpleMatrix() = default;

			/*!
			 * @brief				デストラクタ
			 */
			virtual ~SimpleMatrix() = default;

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
					[](auto& _row) { std::fill(_row.begin(), _row.end(), 0); });
			}

			/*!
			 * @brief			全要素指定数で初期化
			 * @param[in]		__value 初期化要素数
			 * @detail			全要素を指定数で初期化処理を行う
			 *					メモリ解放は行わない
			 */
			inline void clear(const value_type __value)
			{
				Matrix<__Value, __Key>::clear();
				std::for_each(this->matrix_.begin(), this->matrix_.end(),
					[__value](auto& _row) { std::fill(_row.begin(), _row.end(), __value);
				});
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
					[__size](auto& _row) { _row.resize(__size, 0); });
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
				this->matrix_[__row][__column] = __value;
			}

			/*!
			 * @brief			行列の要素を返す
			 * @return			value_type	行列の要素を返す
			 * @param[in]		__row 行番号
			 * @param[in]		__column 列番号
			 * @detail			__row 行番号　__column 列番号で指定される要素を返す
			 *					 - value_typeはテンプレート__Valueによって定義される
			 */
			constexpr value_type operator()(const key_type __row, const key_type __column) const
			{
				return this->matrix_[__row][__column];
			}

		private:
			//!行列コンテナ
			__Matrix matrix_;
	};
} /* namespace okl */

#include "operator.hpp"