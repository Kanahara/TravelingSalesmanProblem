#pragma once

#include "../../../Operator/module.hpp"

namespace okl::tsp {
	/*!
	 *  @brief      ルートスワップ関数基底クラス
	 *  @details    ルートスワップ関数基底クラス
	 */
	class RouteSwapOperator : public Operator
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		RouteSwapOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~RouteSwapOperator() = default;

		using random_device_type = typename Operator::random_device_type;
		using seed_type = typename Operator::seed_type;
		using size_type = typename Operator::size_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return  "ルートスワップ"; }

	};
} /* namespace okl::tsp */