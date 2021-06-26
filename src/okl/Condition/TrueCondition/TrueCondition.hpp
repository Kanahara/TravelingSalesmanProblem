#pragma once

#include "../Condition.hpp"
#include <cstdint>

namespace okl {
	/*!
	 *  @brief      Trueのみを返す
	 *  @details    Trueのみを返す
	 */
	class TrueCondition : public Condition
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		TrueCondition() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~TrueCondition() = default;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "True"; }

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @return			bool	現在のTrue　Falseを返す
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		constexpr bool operator()(const typename __Solution::graph_type& __graph, const __SearchInfo& __search_info, const __Solution& __solution) const
		{
			return true;
		}
		template<class __SearchInfo, class __Solution>
		constexpr bool operator()(const typename __Solution::graph_type& __graph, const __SearchInfo& __search_info, const __Solution& __solution, const __Solution& __initial_solution, const __Solution& __best_solution) const
		{
			return true;
		}
		template<class __SearchInfo, class __Solution, class _Population>
		constexpr typename std::enable_if<!std::is_same<__Solution, _Population>::value>::type operator()(const typename __Solution::graph_type& __graph, const __SearchInfo& __search_info, const _Population& __population, const __Solution& __initial_solution, const __Solution& __best_solution) const
		{
			return true;
		}
	};
} /* namespace okl */