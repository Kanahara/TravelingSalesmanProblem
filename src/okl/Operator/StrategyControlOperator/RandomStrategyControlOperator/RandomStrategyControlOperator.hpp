#pragma once

#include "../StrategyControlOperator.hpp"
#include <string>
#include <cmath>

namespace okl {
	/*!
	 *  @brief      ランダム戦略コントロール関数クラス
	 *  @details    ランダム戦略コントロール関数クラス
	 *				- __MinModeNum			選択モード最小値	範囲[__MinModeNum, __MaxModeNum]のモードを切り替える
	 *				- __MaxModeNum			選択モード最大値	範囲[__MinModeNum, __MaxModeNum]のモードを切り替える　
	 *				- __Stack				選択モードを一時格納するスタッククラス
	 */
	template<
		std::uint_least8_t __MinModeNum,
		std::uint_least8_t __MaxModeNum,
		class __Stack = Stack<std::uint_least8_t>>
		class RandomStrategyControlOperator : public StrategyControlOperator<__Stack>
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		RandomStrategyControlOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~RandomStrategyControlOperator() = default;

		using random_device_type = typename StrategyControlOperator<__Stack>::random_device_type;
		using seed_type = typename StrategyControlOperator<__Stack>::seed_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "ランダム" + StrategyControlOperator<__Stack>::info(); }

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		inline void operator()(const typename __Solution::graph_type& __graph, __SearchInfo& __search_info, __Solution& __solution)
		{
			const auto _select_mode = this->random_device_.uniform_int(__MinModeNum, __MaxModeNum);
			__search_info.setMode(_select_mode);
		}
		template<class __SearchInfo, class __Solution>
		inline void operator()(const typename __Solution::graph_type& __graph, __SearchInfo& __search_info, __Solution& __solution, const __Solution& __initial_solution, __Solution& __best_solution) { (*this)(__graph, __search_info, __solution); }
	};
} /* namespace okl */