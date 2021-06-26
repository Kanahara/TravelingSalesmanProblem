#pragma once

#include "../../../Operator/module.hpp"

namespace okl::tsp {
	/*!
	 *  @brief      ランダムルート順序設定関数基底クラス
	 *  @details    ランダムルート順序設定関数基底クラス
	 */
	class RandomRouteOrderOperator : public RouteOrderOperator
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		RandomRouteOrderOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~RandomRouteOrderOperator() = default;

		using random_device_type = typename RouteOrderOperator::random_device_type;
		using seed_type = typename RouteOrderOperator::seed_type;
		using size_type = typename RouteOrderOperator::size_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return  "ランダム" + RouteOrderOperator::info(); }

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		inline void operator()(const typename __Solution::graph_type& __graph, const __SearchInfo& __search_info, __Solution& __solution, const __Solution& __initial_solution, const __Solution& __best_solution) {
			__solution = __initial_solution;
			__solution.shuffleNodes(this->random_device_.engin());
			for (const auto _node_id : __solution.nodes()) {
				__solution.setRouteOrder(__graph, _node_id);
			}
			__solution.calculateCost(__graph);
		}
	};
} /* namespace okl::tsp */