#pragma once
#include "../SelectionOperator.hpp"

namespace okl {
	/*!
	 *  @brief      エリート選択淘汰処理関数クラス
	 *  @details    遺伝的アルゴリズムなどで使用されるエリート選択淘汰処理を行う関数クラス
	 */
	class NoDuplicationEliteSelectionOperator : public SelectionOperator
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		NoDuplicationEliteSelectionOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~NoDuplicationEliteSelectionOperator() = default;

		using random_device_type = typename SelectionOperator::random_device_type;
		using seed_type = typename SelectionOperator::seed_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "コスト重複なしエリート保存" + SelectionOperator::info(); }

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution, class _Population>
		inline void operator()(const typename __Solution::graph_type& __graph, __SearchInfo& __search_info, _Population& __population, const __Solution& __initial_solution, __Solution& __best_solution)
		{
			std::for_each(__population.next_generation().begin(), __population.next_generation().end(),
				[&__population](const auto _individual_id_a) {
					std::for_each(__population.present_generation().begin(), __population.present_generation().end(),
						[&__population, _individual_id_a](const auto _individual_id_b) {
							if (__population.individual(_individual_id_a).cost() == __population.individual(_individual_id_b).cost()) {
								__population.kill(_individual_id_b);
							}
						}
					);
					__population.advanceGeneration(_individual_id_a);
				}
			);

			__population.sortPresentGeneration([&__population](const auto x, const auto y) {return __population.individual(x).cost() < __population.individual(y).cost(); });

			std::for_each(std::next(__population.present_generation().begin(), __search_info.pop_size()), __population.present_generation().end(),
				[&__population](const auto _individual_id) {
					__population.kill(_individual_id);
				}
			);
			/*
			auto _population_cost_avg = 0;

			for (const auto _id : __population) {
				auto& _individal = __solution_group[_id];
				_population_cost_avg += _individal.cost();
			}
			_population_cost_avg /= __search_info.pop_size();

			__search_info.log() << __solution_group[__population[0]].cost() << "," << _population_cost_avg << "," << __solution_group[__population[__search_info.pop_size() - 1]].cost() <<std::endl;
			*/
		}
	};
} /* namespace okl */