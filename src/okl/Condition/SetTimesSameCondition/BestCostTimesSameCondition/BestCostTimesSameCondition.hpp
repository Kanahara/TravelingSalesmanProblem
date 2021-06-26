#pragma once

#include "../SetTimesSameCondition.hpp"

namespace okl {
	/*!
	 *  @brief      |Best|回数条件が一緒かどうか判定
	 *  @details    |Best|回数条件が一緒かどうか判定する
	 *				|Best|回数条件が一緒ならTrue　一度でも変化したなら回数を数えなおし
	 *				- __Condition				条件クラス
	 */
	template<class __Condition>
	class BestCostTimesSameCondition : public SetTimesSameCondition<__Condition>
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		BestCostTimesSameCondition() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~BestCostTimesSameCondition() = default;

		using seed_type = typename SetTimesSameCondition<__Condition>::seed_type;
		using random_device_type = typename SetTimesSameCondition<__Condition>::random_device_type;
		using size_type = typename SetTimesSameCondition<__Condition>::size_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "回(" + __Condition::info() + ")が同じなら"; }

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @return			bool	現在のTrue　Falseを返す
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		inline bool operator()(const typename __Solution::graph_type& __graph, const __SearchInfo& __search_info, const __Solution& __solution, const __Solution& __initial_solution, const __Solution& __best_solution)
		{
			bool _state = this->condition_(__graph, __search_info, __solution, __initial_solution, __best_solution);
			if (this->before_state_ != _state || this->is_first_) {
				this->same_count_ = 1;
				this->is_first_ = false;
			}
			else {
				this->same_count_++;
			}
			this->before_state_ = _state;

			if (this->same_count_ <= __best_solution.cost()) {
				return false;
			}
			else {
				this->same_count_ = 1;
				return true;
			}
		}
	};
} /* namespace okl */