#pragma once
#include "../SelectionOperator.hpp"

namespace okl {
	/*!
	 *  @brief      エリート選択淘汰処理関数クラス
	 *  @details    遺伝的アルゴリズムなどで使用されるエリート選択淘汰処理を行う関数クラス
	 */
	class ExceptForBestSolutionChangeSelectionOperator : public SelectionOperator
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		ExceptForBestSolutionChangeSelectionOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~ExceptForBestSolutionChangeSelectionOperator() = default;

		using random_device_type = typename SelectionOperator::random_device_type;
		using seed_type = typename SelectionOperator::seed_type;
		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "最良解以外を変更する" + SelectionOperator::info(); }

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution, class _Population>
		inline void operator()(const typename __Solution::graph_type& __graph, __SearchInfo& __search_info, _Population& __population, const __Solution& __initial_solution, __Solution& __best_solution)
		{
			const auto _best_individual_id = *std::max_element(__population.present_generation().begin(), __population.present_generation().end(),
				[&__population](const auto _individual_a_id, const auto _individual_b_id) {
					return __population.individual(_individual_a_id) < __population.individual(_individual_b_id);
				}
			);

			while (1 < __population.present_generation().size()) {
				const auto _individual_id = __population.present_generation().back();
				if (_best_individual_id == _individual_id) {
					__population.kill(__population.present_generation()[0]);
				}
				else {
					__population.kill(_individual_id);
				}
			}

			while (__search_info.pop_size() < __population.size()) {
				const auto _individual_id = __population.next_generation().back();
				__population.kill(_individual_id);
			}

			__population.advanceGeneration();

			////auto _population_cost_avg = 0;
			////for (const auto _id : __population) {
			////	auto& _individal = __solution_group[_id];
			////	_population_cost_avg += _individal.cost();
			////}
			////_population_cost_avg /= __search_info.pop_size();
			////__search_info.log() << __solution_group[__population[0]].cost() << "," << _population_cost_avg << "," << __solution_group[__population[__search_info.pop_size() - 1]].cost() << std::endl;
		}
	};
} /* namespace okl */