#pragma once

#include "../Operator.hpp"

namespace okl {
	/*!
	 *  @brief      ベスト解を更新する関数クラス
	 *  @details    ベスト解を現在の解に読み込む関数クラス
	 */
	class UpgradeBestSolutionOperator : public Operator
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		UpgradeBestSolutionOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~UpgradeBestSolutionOperator() = default;

		using random_device_type = typename Operator::random_device_type;
		using seed_type = typename Operator::seed_type;
		using size_type = typename Operator::size_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "最良解更新"; }

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		inline void operator()(const typename __Solution::graph_type& __graph, __SearchInfo& __search_info, const __Solution& __solution)
		{
			//if (__solution.meetConstraints()) {
			__search_info.setResult(__solution.cost());
			//}
		}
		template<class __SearchInfo, class __Solution>
		inline void operator()(const typename __Solution::graph_type& __graph, __SearchInfo& __search_info, const __Solution& __solution, const __Solution& __initial_solution, __Solution& __best_solution)
		{
			if (__best_solution < __solution) {
				__best_solution = __solution;
				if (__solution.meetConstraints()) {
					__search_info.setResult(__solution.cost());
				}
			}
			else {
				__search_info.setResult(__best_solution.cost());
			}
		}
	};
} /* namespace okl */