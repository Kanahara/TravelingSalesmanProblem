#pragma once

#include "../StrategyControlOperator.hpp"
#include <string>
#include <cmath>

namespace okl {
	/*!
	 *  @brief      UCB戦略コントロール関数クラス
	 *  @details    UCB戦略コントロール関数クラス
	 *				- __Alpha				探索係数α
	 *				- __MinModeNum			選択モード最小値	範囲[__MinModeNum, __MaxModeNum]のモードを切り替える
	 *				- __MaxModeNum			選択モード最大値	範囲[__MinModeNum, __MaxModeNum]のモードを切り替える　
	 *				- __Stack				選択モードを一時格納するスタッククラス
	 */
	template<
		class __Alpha,
		std::uint_least8_t __MinModeNum,
		std::uint_least8_t __MaxModeNum,
		class __Stack = Stack<std::uint_least8_t>>
		class UpperConfidenceBoundStrategyControlOperator : public StrategyControlOperator<__Stack>
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		UpperConfidenceBoundStrategyControlOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~UpperConfidenceBoundStrategyControlOperator() = default;

		using random_device_type = typename StrategyControlOperator<__Stack>::random_device_type;
		using seed_type = typename StrategyControlOperator<__Stack>::seed_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "UCB" + StrategyControlOperator<__Stack>::info(); }

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
				this->stack_.clear();
				auto _max_expected_score = std::numeric_limits<double>::min();
				double _all_count = 0;
				for (mode_type _mode = __MinModeNum; _mode <= __MaxModeNum; ++_mode) {
					_all_count += static_cast<double>(__search_info.mode_count(_mode));
				}
				const auto _log_all_count_times_alpah_div_two = std::log(_all_count) * this->alpha_div_two_;
				for (mode_type _mode = __MinModeNum; _mode <= __MaxModeNum; ++_mode) {
					const auto _score = __search_info.mode_expected_score_avg(_mode) + std::sqrt(_log_all_count_times_alpah_div_two / static_cast<double>(__search_info.mode_count(_mode)));
					if (_max_expected_score < _score) {
						_max_expected_score = _score;
						this->stack_.clear();
						this->stack_.push(_mode);
					}
					else if (_max_expected_score == _score) {
						this->stack_.push(_mode);
					}
				}
			}
			const auto _select_mode = this->random_device_.random_element(this->stack_);
			__search_info.setMode(_select_mode);
		}
		template<class __SearchInfo, class __Solution>
		inline void operator()(const typename __Solution::graph_type& __graph, __SearchInfo& __search_info, __Solution& __solution, const __Solution& __initial_solution, __Solution& __best_solution) { (*this)(__graph, __search_info, __solution); }

	private:
		//係数
		double alpha_div_two_;
	};
} /* namespace okl */