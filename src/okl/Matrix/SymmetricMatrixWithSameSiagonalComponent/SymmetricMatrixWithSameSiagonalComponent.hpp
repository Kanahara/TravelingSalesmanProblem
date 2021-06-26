#pragma once

#include "../Matrix.hpp"

namespace okl {
	/*!
	 *  @brief		対象正則行列クラス
	 *  @details    対象正則正則行列クラス
	 * 				要素を格納・取得するだけの可変長行列クラス
	 *				要素がメモリ連続かつ対象要素が定数の上三角行列クラス
	 *				- __Value			要素型
	 *				- __Key				インデックス数値型
	 *				- SiagonalComponent	対角要素の値
	 *				- __Container		行列コンテナ（一次元可変長配列）
	 */
	template<
		typename __Value,
		typename __Key,
		std::size_t SiagonalComponent = 0,
		class __Container = std::vector<__Value>>
		class SymmetricMatrixWithSameSiagonalComponent : public Matrix<__Value, __Key> {
		public:
			/*!
			 * @brief				コンストラクタ
			 */
			SymmetricMatrixWithSameSiagonalComponent() = default;

			/*!
			 * @brief				デストラクタ
			 */
			virtual ~SymmetricMatrixWithSameSiagonalComponent() = default;

			using value_type = typename Matrix<__Value, __Key>::value_type;
			using key_type = typename Matrix<__Value, __Key>::key_type;
			using size_type = typename Matrix<__Value, __Key>::size_type;
			using symmetric = std::true_type;

			/*!
			 * @brief			全要素初期化
			 * @detail			全要素を初期化処理を行う
			 *					メモリ解放は行わない
			 */
			inline void clear() override
			{
				Matrix<__Value, __Key>::clear();
				std::fill(this->container_.begin(), this->container_.end(), 0);
			}

			/*!
			 * @brief			サイズ設定
			 * @param[in]		__size 正則行列の行と列のサイズ
			 * @detail			全要素をメモリ確保を行う
			 */
			inline void resize(const size_type __size) override final
			{
				Matrix<__Value, __Key>::resize(__size);
				this->alpha_ = static_cast<key_type>(2 * this->size_ - 3);
				this->container_.resize(this->size_ * (this->size_ - 1) / 2 + 1, 0);
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
				if (__row < __column) {
					this->container_[__row * (this->alpha_ - __row) / 2 + __column] = __value;
				}
				else if (__row > __column) {
					this->container_[__column * (this->alpha_ - __column) / 2 + __row] = __value;
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
			constexpr value_type operator()(const key_type __row, const key_type __column) const
			{
				if (__row < __column) {
					return this->container_[__row * (this->alpha_ - __row) / 2 + __column];
				}
				else if (__row > __column) {
					return this->container_[__column * (this->alpha_ - __column) / 2 + __row];
				}
				else {
					return SiagonalComponent;
				}
			}

		private:
			//!要素アクセスのためのサイズに依存した定数
			size_type alpha_;

			//!行列コンテナ
			__Container container_;

			//!対角要素の値
			//__Value same_ = static_cast<__Value>(SiagonalComponent);
	};
} /* namespace okl */

#include "operator.hpp"