#pragma once
#include <vector>
#include <limits>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>

namespace okl {
	/*!
	 *  @brief      ビット集合クラス
	 *  @details    動的確保ビット集合クラス
	 *				__Valueにはunsigned型を指定すること
	 *				- __Value				要素型
	 *				- __Key					インデックス数値型
	 *				- __Store				要素格納コンテナ
	 */
	template<
		typename __Value,
		typename __Key = __Value,
		class __Store = std::vector<__Value >>
		class BitSet {
		public:
			/*!
			 * @brief				コンストラクタ
			 */
			BitSet() = default;

			/*!
			 * @brief				デストラクタ
			 */
			virtual ~BitSet() = default;

			using value_type = __Value;
			using key_type = __Key;
			using size_type = key_type;

			/*!
			 * @brief			任意の位置のビットを設定する
			 * @param[in]		__index 位置
			 * @detail			任意の位置のビットを設定する
			 */
			inline void set(const key_type __index)
			{
				assert(0 <= __index / std::numeric_limits<value_type>::digits && __index / std::numeric_limits<value_type>::digits < this->store_.size());
				const auto _store_index = __index / std::numeric_limits<value_type>::digits;
				const auto _bit = this->base() >> (__index % std::numeric_limits<value_type>::digits);
				if (!(this->store_[_store_index] & _bit)) {
					this->store_[_store_index] |= _bit;
					++this->size_;
				}
			}

			/*!
			 * @brief			任意の位置のビットを0にする
			 * @param[in]		__index 位置
			 * @detail			任意の位置のビットを0にする
			 */
			inline void reset(const key_type __index)
			{
				assert(0 <= __index / std::numeric_limits<value_type>::digits && __index / std::numeric_limits<value_type>::digits < this->store_.size());
				const auto _store_index = __index / std::numeric_limits<value_type>::digits;
				const auto _bit = this->base() >> (__index % std::numeric_limits<value_type>::digits);
				if (this->store_[_store_index] & _bit) {
					this->store_[_store_index] &= ~(_bit);
					--this->size_;
				}
			}

			/*!
			 * @brief			全要素初期化
			 * @detail			全要素を初期化処理を行う
			 *					メモリ解放は行わない
			 */
			inline void clear()
			{
				this->size_ = 0;
				std::fill(this->store_.begin(), this->store_.end(), 0);
			}

			/*!
			 * @brief			集合に含まれているか判定
			 * @return			bool	含まれているならTrue　含まれていないらFalse
			 * @detail			集合に含まれているか判定を行う
			 */
			constexpr bool contains(const key_type __index) const
			{
				assert(0 <= __index / std::numeric_limits<value_type>::digits && __index / std::numeric_limits<value_type>::digits < this->store_.size());
				return this->store_[__index / std::numeric_limits<value_type>::digits] & (this->base() >> (__index % std::numeric_limits<value_type>::digits));
			}

			/*!
			 * @brief			要素数を返す
			 * @return			size_type	要素数を返す
			 * @detail			集合内の要素数を返す
			 */
			constexpr size_type size() const
			{
				return this->size_;
			}

			/*!
			 * @brief			格納可能要素数（容量）を返す
			 * @return			size_type	格納可能要素数（容量）返す
			 * @detail			集合の容量を返す
			 */
			constexpr size_type capacity() const
			{
				return this->store_.size() * std::numeric_limits<value_type>::digits;
			}

			/*!
			 * @brief			空集合か判定
			 * @return			bool	要素数0ならTrue　空じゃないならFalse
			 * @detail			空集合か判定を行う
			 */
			constexpr bool isEmpty() const
			{
				return this->size_ == 0;
			}

			/*!
			 * @brief			格納可能要素数（容量）を設定
			 * @param[in]		__capacity 容量
			 * @detail			集合の容量を設定する
			 */
			inline void reserve(const size_type __capacity)
			{
				if (__capacity % std::numeric_limits<value_type>::digits == 0) {
					this->store_.resize((__capacity / std::numeric_limits<value_type>::digits));
				}
				else {
					this->store_.resize((__capacity / std::numeric_limits<value_type>::digits) + 1);
				}
			}

			/*!
			 * @brief			指定のインデックスに入っている要素を取得
			 * @detail			指定のインデックスに入っている要素を取得する
			 * @param[in]		__index 格納番号
			 * @return			value_type&	指定要素の参照を返す
			 * 					- 要素型value_typeはテンプレート__Valueによって定義される
			 */
			constexpr bool operator[](const key_type __index) const
			{
				return this->contains(__index);
			}

		private:
			//!サイズ変数
			size_type size_;

			//!データストア配列
			__Store store_;

			//!bit演算時の最大bitのみ1の値
			constexpr value_type base() const
			{
				return static_cast<value_type>(1) << (std::numeric_limits<value_type>::digits - 1);
			}
	};
} /* namespace okl */

#include "operator.hpp"