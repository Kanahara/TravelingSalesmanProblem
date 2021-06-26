#pragma once

#include "../Random/module.hpp"
#include "../Stack/module.hpp"
#include "../Timer/module.hpp"

#include <cassert>

namespace okl {
	/*!
	 *  @brief      実行関数クラス
	 *  @details    関数演算子によって何らかの処理を行う関数クラスの基底クラス
	 *				このクラスではなにも処理を行わない関数が定義される
	 */
	class Operator
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		Operator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~Operator() = default;

		using seed_type = std::seed_seq::result_type;
		using random_device_type = Random<>;
		using size_type = std::size_t;

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う　このクラスではなにも行わない
		 */
		inline virtual void clear() {}

		/*!
		 * @brief			サイズ設定
		 * @param[in]		__size 頂点数
		 * @detail			全要素をメモリ確保を行う　このクラスではなにも行わない
		 */
		inline virtual void resize(const size_type __size) {}

		/*!
		 * @brief			シードを設定
		 * @param[in]		__seed シード
		 * @detail			メンバ変数のシードを設定する
		 * 					- seed_typeは標準ライブラリによって定義される
		 */
		inline virtual void seed(const seed_type __seed)
		{
			this->random_device_.seed(__seed);
		}
	protected:
		//!乱数生成機
		random_device_type random_device_;
	};
} /* namespace okl */