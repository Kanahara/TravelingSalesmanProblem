#pragma once

#include "../Operator.hpp"
#include "../../Stack/module.hpp"
#include <string>

namespace okl {
	/*!
	 *  @brief      戦略コントロール基底関数クラス
	 *  @details    戦略コントロール基底関数クラス
	 *				- __Stack				選択モードを一時格納するスタッククラス　
	 */
	template<class __Stack = Stack<std::uint_least8_t>>
	class StrategyControlOperator : public Operator
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		StrategyControlOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~StrategyControlOperator() = default;

		using random_device_type = typename Operator::random_device_type;
		using seed_type = typename Operator::seed_type;
		using size_type = typename Operator::size_type;
		using mode_type = std::uint_least8_t;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "戦略コントロール"; }

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 *					メモリ解放は行わない
		 */
		inline void clear() override
		{
			Operator::clear();
			this->stack_.clear();
		}

		/*!
		 * @brief			サイズ設定
		 * @param[in]		__size 頂点数
		 * @detail			全要素をメモリ確保を行う
		 */
		inline void resize(const size_type __size) override
		{
			Operator::resize(__size);
			this->stack_.reserve(static_cast<typename __Stack::size_type>(std::numeric_limits<mode_type>::max()));
		}

	protected:
		//!候補コンテナ
		__Stack stack_;
	};
} /* namespace okl */