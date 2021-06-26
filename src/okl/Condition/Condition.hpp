#pragma once

#include <random>
#include "../Random/module.hpp"

namespace okl {
	/*!
	 *  @brief      条件クラス
	 *  @details    関数演算子によってbool型を返す関数クラスの基底クラス
	 */
	class Condition
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		Condition() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~Condition() = default;

		using seed_type = std::seed_seq::result_type;
		using random_device_type = Random<>;
		using size_type = std::size_t;

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 *					このクラスの実装ではなにもしない
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