#pragma once
#include "../Condition.hpp"
#include <limits>

namespace okl {
	/*!
	 *  @brief      全体での最大コスト（最大化問題の最良解）が一つ前の状態より大きいか判定する
	 *  @details    全体での最大コスト（最大化問題の最良解）が一つ前の状態より大きいか判定する基底クラス
	 *				大きい場合はTrue
	 *				- __Cost				コスト数値型
	 *				- __CostComparisonFunction		解の値　比較関数型
	 */
	template<
		class __CostComparisonFunction>
		class BestCostIsBetterThenBestKnownCost : public Condition
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		BestCostIsBetterThenBestKnownCost() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~BestCostIsBetterThenBestKnownCost() = default;

		using seed_type = typename Condition::seed_type;
		using random_device_type = typename Condition::random_device_type;
		using size_type = typename Condition::size_type;

		using cost_comparison_function = __CostComparisonFunction;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "既知の最良解より解が良かったら"; }

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @return			bool	現在のTrue　Falseを返す
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		constexpr bool operator()(const typename __Solution::graph_type& __graph, const __SearchInfo& __search_info, const __Solution& __solution) const
		{
			if (cost_comparison_function()(__search_info.best_known_cost(), __search_info.best_result().get()) || __search_info.best_known_cost() == __search_info.best_result().get()) {
				return true;
			}
			else {
				return false;
			}
		}
		template<class __SearchInfo, class __Solution>
		constexpr bool operator()(const typename __Solution::graph_type& __graph, const __SearchInfo& __search_info, const __Solution& __solution, const __Solution& __initial_solution, const __Solution& __best_solution) const
		{
			if (cost_comparison_function()(__search_info.best_known_cost(), __search_info.best_result().get()) || __search_info.best_known_cost() == __search_info.best_result().get()) {
				return true;
			}
			else {
				return false;
			}
		}
	};
} /* namespace okl */