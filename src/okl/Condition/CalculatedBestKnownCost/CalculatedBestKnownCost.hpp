#pragma once

#include "../Condition.hpp"

namespace okl {
	/*!
	 *  @brief      既知の最良解の値が算出されたか判定する
	 *  @details    既知の最良解の値が算出されたか判定する条件クラス
	 *				算出された場合はTrue
	 */
	class CalculatedBestKnownCost : public Condition
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		CalculatedBestKnownCost() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~CalculatedBestKnownCost() = default;

		using seed_type = typename Condition::seed_type;
		using random_device_type = typename Condition::random_device_type;
		using size_type = typename Condition::size_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "既知の最良解が算出される"; }

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @return			bool	現在のTrue　Falseを返す
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		constexpr bool operator()(const typename __Solution::graph_type& __graph, const __SearchInfo& __search_info, const __Solution& __solution) const
		{
			return __search_info.best_known_cost() == __solution.cost() || __search_info.best_known_cost() == __search_info.best_result().get();
		}
		template<class __SearchInfo, class __Solution>
		constexpr bool operator()(const typename __Solution::graph_type& __graph, const __SearchInfo& __search_info, const __Solution& __solution, const __Solution& __initial_solution, const __Solution& __best_solution) const
		{
			return __search_info.best_known_cost() == __solution.cost() || __search_info.best_known_cost() == __search_info.best_result().get() || __search_info.best_known_cost() == __best_solution.cost();
		}
	};
} /* namespace okl */