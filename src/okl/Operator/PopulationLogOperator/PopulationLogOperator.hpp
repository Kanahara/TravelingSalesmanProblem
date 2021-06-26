#pragma once
#include "../Operator.hpp"

namespace okl {
	/*!
	 *  @brief      エリート選択淘汰処理関数クラス
	 *  @details    遺伝的アルゴリズムなどで使用されるエリート選択淘汰処理を行う関数クラス
	 */
	class PopulationLogOperator : public Operator
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		PopulationLogOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~PopulationLogOperator() = default;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "populationのログ"; }

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution, class _Population>
		inline void operator()(const typename __Solution::graph_type& __graph, __SearchInfo& __search_info, _Population& __population, const __Solution& __initial_solution, const __Solution& __best_solution)
		{
			using cost_type = typename __Solution::cost_type;
			cost_type _avg_cost = 0;
			std::for_each(__population.present_generation().begin(), __population.present_generation().end(),
				[&__population, &_avg_cost](const auto _individual_id) {
					_avg_cost += __population.individual(_individual_id).cost();
				}
			);
			_avg_cost /= static_cast<cost_type>(__population.present_generation().size());

			const auto _best_individual_id = *std::max_element(__population.present_generation().begin(), __population.present_generation().end(),
				[&__population](const auto _individual_a_id, const auto _individual_b_id) {
					return __population.individual(_individual_a_id) < __population.individual(_individual_b_id);
				}
			);
			const auto _best_cost = __population.individual(_best_individual_id).cost();

			__search_info.setInfo(std::make_tuple(__search_info.best_result().get(), _best_cost, _avg_cost, __search_info.cpu_time(), __search_info.key_data(0), __search_info.key_data(1), __search_info.key_data(2), __search_info.key_data(3), __search_info.key_data(4), __search_info.key_data(5), __search_info.key_data(6)));
		}
	};
} /* namespace okl */