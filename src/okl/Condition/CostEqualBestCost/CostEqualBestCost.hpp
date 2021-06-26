#pragma once

#include "../Condition.hpp"

namespace okl {
	/*!
	 *  @brief      現在の解が算出された最大の解の値と同じ判定する
	 *  @details    現在の解が算出された最大の解の値と同じ判定する条件クラス
	 *				同じはTrue
	 */
	class CostEqualBestCost : public Condition
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		CostEqualBestCost() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~CostEqualBestCost() = default;

		using seed_type = typename Condition::seed_type;
		using random_device_type = typename Condition::random_device_type;
		using size_type = typename Condition::size_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "現在の解が算出された最良の解の値と同じ"; }

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @return			bool	現在のTrue　Falseを返す
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		constexpr bool operator()(const typename __Solution::graph_type& __graph, const __SearchInfo& __search_info, const __Solution& __solution, const __Solution& __initial_solution, const __Solution& __best_solution) const
		{
			if (__solution < __best_solution) {
				return __best_solution.cost() == __search_info.best_result().get();
			}
			else {
				return __solution.cost() == __search_info.best_result().get();
			}
		}
	};
} /* namespace okl */