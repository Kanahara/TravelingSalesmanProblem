#pragma once
#include "../SelectionOperator.hpp"

namespace okl {
	/*!
	 *  @brief      エリート選択淘汰処理関数クラス
	 *  @details    遺伝的アルゴリズムなどで使用されるエリート選択淘汰処理を行う関数クラス
	 */
	class EliteSelectionOperator : public SelectionOperator
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		EliteSelectionOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~EliteSelectionOperator() = default;

		using random_device_type = typename SelectionOperator::random_device_type;
		using seed_type = typename SelectionOperator::seed_type;
		using size_type = typename SelectionOperator::size_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "エリート保存" + SelectionOperator::info(); }

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution, class _Population>
		inline void operator()(const typename __Solution::graph_type& __graph, __SearchInfo& __search_info, _Population& __population, const __Solution& __initial_solution, __Solution& __best_solution)
		{
			__population.advanceGeneration();
			__population.sortPresentGeneration([&__population](const auto x, const auto y) {return __population.individual(x).cost() < __population.individual(y).cost(); });

			while (__search_info.pop_size() < __population.size()) {
				const auto _individual_id = __population.next_generation().back();
				__population.kill(_individual_id);
			}
		}
	};
} /* namespace okl */