#pragma once

#include "../Operator.hpp"
#include <string>

namespace okl {
	/*!
	 *  @brief      カウントクリア関数クラス
	 *  @details    アルゴリズムの分析等のためのカウントをする関数クラス
	 *				- __KeyIndex			カウントする情報の識別キー
	 */
	template<std::uint_least8_t __KeyIndex>
	class ClearKeyOperator : public Operator
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		ClearKeyOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~ClearKeyOperator() = default;

		using random_device_type = typename Operator::random_device_type;
		using seed_type = typename Operator::seed_type;
		using size_type = typename Operator::size_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "クリア:" + std::to_string(__KeyIndex); }

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		inline void operator()(const typename __Solution::graph_type& __graph, __SearchInfo& __search_info, const __Solution& __solution)
		{
			__search_info.clearKey(__KeyIndex);
		}
		template<class __SearchInfo, class __Solution>
		inline void operator()(const typename __Solution::graph_type& __graph, __SearchInfo& __search_info, const __Solution& __solution, const __Solution& __initial_solution, const __Solution& __best_solution)
		{
			(*this)(__graph, __search_info, __solution);
		}
		template<class __SearchInfo, class __Solution, class _Population>
		inline typename std::enable_if<!std::is_same<__Solution, _Population>::value>::type operator()(const typename __Solution::graph_type& __graph, __SearchInfo& __search_info, const _Population& __population, const __Solution& __initial_solution, const __Solution& __best_solution)
		{
			(*this)(__graph, __search_info, __population.individual(*__population.present_generation().begin()), __initial_solution, __best_solution);
		};
	};
} /* namespace okl */