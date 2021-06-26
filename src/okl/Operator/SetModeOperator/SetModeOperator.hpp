#pragma once

#include "../Operator.hpp"
#include <string>

namespace okl {
	/*!
	 *  @brief      モード設定関数クラス
	 *  @details    モードを切り替える関数クラス
	 *				- __ModeType			切り替えるモード番号
	 */
	template<std::uint_least8_t __ModeType>
	class SetModeOperator : public Operator
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		SetModeOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~SetModeOperator() = default;

		using random_device_type = typename Operator::random_device_type;
		using seed_type = typename Operator::seed_type;
		using size_type = typename Operator::size_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "モード設定" + std::to_string(__ModeType); }

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		inline void operator()(const typename __Solution::graph_type& __graph, __SearchInfo& __search_info, const __Solution& __solution)
		{
			__search_info.setMode(__ModeType);
		}
		template<class __SearchInfo, class __Solution>
		inline void operator()(const typename __Solution::graph_type& __graph, __SearchInfo& __search_info, const __Solution& __solution, const __Solution& __initial_solution, const __Solution& __best_solution) { (*this)(__graph, __search_info, __solution); }
	};
} /* namespace okl */