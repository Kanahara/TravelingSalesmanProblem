#pragma once
#include "../Condition.hpp"
#include <cstdint>
namespace okl {
	/*!
	 *  @brief      設定時間以上か判定
	 *  @details    設定時間以上か判定する
	 *				設定時間以上ならTrue
	 */
	class TimerCondition : public Condition
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		TimerCondition() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~TimerCondition() = default;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "指定時間繰り返すまで"; }

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @return			bool	現在のTrue　Falseを返す
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		constexpr bool operator()(const typename __Solution::graph_type& __graph, const __SearchInfo& __search_info, const __Solution& __solution, const __Solution& __initial_solution, const __Solution& __best_solution) const
		{
			return __search_info.run_time() <= __search_info.cpu_time() && __search_info.run_time() != 0;
		}
	};
} /* namespace okl */