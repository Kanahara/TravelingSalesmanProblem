#pragma once

#include "../StrategyControlOperator.hpp"
#include <string>

namespace okl {
	/*!
	 *  @brief      e-貪欲戦略コントロール関数クラス
	 *  @details    e-貪欲戦略コントロール関数クラス
	 *				- __Epsolon				ランダム選択確率
	 *				- __MinModeNum			選択モード最小値	範囲[__MinModeNum, __MaxModeNum]のモードを切り替える
	 *				- __MaxModeNum			選択モード最大値	範囲[__MinModeNum, __MaxModeNum]のモードを切り替える　
	 *				- __Stack				選択モードを一時格納するスタッククラス
	 */
	template<
		class __Epsolon,
		std::uint_least8_t __MinModeNum,
		std::uint_least8_t __MaxModeNum,
		class __Stack = Stack<std::uint_least8_t>>
		class EpsilonGreedyStrategyControlOperator : public StrategyControlOperator<__Stack>
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		EpsilonGreedyStrategyControlOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~EpsilonGreedyStrategyControlOperator() = default;

		using random_device_type = typename StrategyControlOperator<__Stack>::random_device_type;
		using seed_type = typename StrategyControlOperator<__Stack>::seed_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "e-貪欲" + StrategyControlOperator<__Stack>::info(); }

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		inline void operator()(const typename __Solution::graph_type& __graph, __SearchInfo& __search_info, __Solution& __solution)
		{
			using mode_type = typename __SearchInfo::mode_type;
			this->stack_.clear();
			for (mode_type _mode = __MinModeNum; _mode <= __MaxModeNum; ++_mode) {
				if (__search_info.mode_count(_mode) == 0) {
					this->stack_.push(_mode);
				}
			}
			if (this->stack_.size() == 0) {
				if (this->random_device_.bernoulli(this->epsolon_())) {
					const auto _select_mode = this->random_device_.uniform_int(__MinModeNum, __MaxModeNum);
					__search_info.setMode(_select_mode);
				}
				else {
					this->stack_.clear();
					auto _max_expected_cost = std::numeric_limits<double>::min();
					for (mode_type _mode = __MinModeNum; _mode <= __MaxModeNum; ++_mode) {
						double _expected_cost = std::numeric_limits<double>::min();
						if (__search_info.mode_count(_mode) != 0) {
							_expected_cost = __search_info.mode_cost_avg(_mode);
						}
						if (_max_expected_cost < _expected_cost) {
							_max_expected_cost = _expected_cost;
							this->stack_.clear();
							this->stack_.push(_mode);
						}
						else if (_max_expected_cost == _expected_cost) {
							this->stack_.push(_mode);
						}
					}
					const auto _select_mode = this->random_device_.random_element(this->stack_);
					__search_info.setMode(_select_mode);
				}
			}
			else {
				const auto _select_mode = this->random_device_.random_element(this->stack_);
				__search_info.setMode(_select_mode);
			}
		}
		template<class __SearchInfo, class __Solution>
		inline void operator()(const typename __Solution::graph_type& __graph, __SearchInfo& __search_info, __Solution& __solution, const __Solution& __initial_solution, __Solution& __best_solution) { (*this)(__graph, __search_info, __solution); }

	private:
		//確率
		__Epsolon epsolon_;
	};
} /* namespace okl */