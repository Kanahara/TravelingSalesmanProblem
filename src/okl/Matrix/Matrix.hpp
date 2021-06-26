#pragma once

#include <vector>
#include <iostream>

namespace okl {
	/*!
	 *  @brief      行列クラス用基底クラス
	 *  @details    行列クラス用基底クラス
	 * 				要素を格納・取得するだけのシンプルな可変長行列クラス
	 *				- __Value			要素型
	 *				- __Key				インデックス数値型
	 */
	template<
		typename __Value,
		typename __Key>
		class Matrix {
		public:
			/*!
			 * @brief				コンストラクタ
			 */
			Matrix() : size_(0) {}

			/*!
			 * @brief				デストラクタ
			 */
			virtual ~Matrix() = default;

			using value_type = __Value;
			using key_type = __Key;
			using size_type = key_type;
			/*!
			 * @brief			全要素初期化
			 * @detail			全要素を初期化処理を行う
			 *					メモリ解放は行わない
			 */
			inline virtual void clear()
			{
				//this->size_ = 0;
			}

			/*!
			 * @brief			サイズ設定
			 * @param[in]		__size 正則行列の行と列のサイズ
			 * @detail			全要素をメモリ確保を行う
			 */
			inline virtual void resize(const size_type __size)
			{
				this->size_ = __size;
			}

			/*!
			 * @brief			サイズを返す
			 * @return			std::size_t	サイズを返す
			 * @detail			要素数を返す
			 */
			constexpr size_type size() const
			{
				return this->size_;
			}

		protected:
			//!サイズ
			size_type size_;
	};
} /* namespace okl */