#pragma once

#include "../RouteSwapOperator.hpp"

namespace okl::tsp {
	/*!
	 *  @brief      最良移動戦略ルートスワップ関数クラス
	 *  @details    最良移動戦略ルートスワップ関数クラス
	 */
	class BestImprovementRouteSwapOperator : public RouteSwapOperator
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		BestImprovementRouteSwapOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~BestImprovementRouteSwapOperator() = default;

		using random_device_type = typename RouteSwapOperator::random_device_type;
		using seed_type = typename RouteSwapOperator::seed_type;
		using size_type = typename RouteSwapOperator::size_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return  "最良移動戦略" + RouteSwapOperator::info(); }

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		inline void operator()(const typename __Solution::graph_type& __graph, const __SearchInfo& __search_info, __Solution& __solution, const __Solution& __initial_solution, const __Solution& __best_solution) {
			using size_type = typename __Solution::size_type;
			using cost_type = typename __Solution::cost_type;

			__solution.shuffleNodes(this->random_device_.engin());

			cost_type _best_gaincost = 0;
			size_type _candidate_index_a = std::numeric_limits<size_type>::max();
			size_type _candidate_index_b = std::numeric_limits<size_type>::max();
			const size_type _last_index = static_cast<size_type>(__solution.nodes().size() - 1);

			for (size_type _index_a = 0; _index_a < _last_index; ++_index_a) {
				const auto _r_index_a = __solution.nodes()[_index_a];				
				for (size_type _index_b = static_cast<size_type>(_index_a + 1); _index_b < __solution.nodes().size(); ++_index_b) {
					const auto _r_index_b = __solution.nodes()[_index_b];
					if ( (_r_index_a != 0 || _r_index_b != _last_index) && (_r_index_a != _last_index || _r_index_b != 0)) {
						const auto _gain_cost = __solution.gaincostAfterRouteSwap(__graph, _r_index_a, _r_index_b);
						if (_gain_cost < _best_gaincost) {
							_best_gaincost = _gain_cost;
							_candidate_index_a = _r_index_a;
							_candidate_index_b = _r_index_b;
						}
					}
				}
			}

			if (_candidate_index_a != std::numeric_limits<size_type>::max() && _candidate_index_b != std::numeric_limits<size_type>::max()) {
				__solution.routeSwap(__graph, _candidate_index_a, _candidate_index_b, _best_gaincost);
			}
		}
	};
} /* namespace okl::tsp */