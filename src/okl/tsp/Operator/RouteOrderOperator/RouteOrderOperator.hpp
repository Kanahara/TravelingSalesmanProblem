#pragma once

#include "../../../Operator/module.hpp"

namespace okl::tsp {
	/*!
	 *  @brief      ルート順序設定関数基底クラス
	 *  @details    ルート順序設定関数基底クラス
	 */
	class RouteOrderOperator : public Operator
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		RouteOrderOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~RouteOrderOperator() = default;

		using random_device_type = typename Operator::random_device_type;
		using seed_type = typename Operator::seed_type;
		using size_type = typename Operator::size_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return  "ルート順序"; }

	};
} /* namespace okl::tsp */