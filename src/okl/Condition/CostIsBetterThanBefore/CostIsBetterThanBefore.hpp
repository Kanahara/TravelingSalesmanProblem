#pragma once
#include "../Condition.hpp"
#include <limits>

namespace okl {
	/*!
	 *  @brief      コストが一つ前の状態より良いか判定する
	 *  @details    コストが一つ前の状態より良いか判定する基底クラス
	 *				大きい場合はTrue
	 *				- __Cost				コスト数値型
	 *				- __CostComparisonFunction		解の値　比較関数型
	 */
	template<
		typename __Cost,
		class __CostComparisonFunction>
		class CostIsBetterThanBefore : public Condition
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		CostIsBetterThanBefore() : before_cost_(this->undefine_cost()) {};

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~CostIsBetterThanBefore() = default;

		using seed_type = typename Condition::seed_type;
		using random_device_type = typename Condition::random_device_type;
		using size_type = typename Condition::size_type;

		using cost_type = __Cost;
		using cost_comparison_function = __CostComparisonFunction;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "前より解が良かったら"; }

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 */
		inline void clear() override
		{
			Condition::clear();
			this->before_cost_ = this->undefine_cost();
		}

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @return			bool	現在のTrue　Falseを返す
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		inline bool operator()(const typename __Solution::graph_type& __graph, const __SearchInfo& __search_info, const __Solution& __solution, const __Solution& __initial_solution, const __Solution& __best_solution)
		{
			if (this->before_cost_ == this->undefine_cost() || cost_comparison_function()(this->before_cost_, __solution.cost())) {
				this->before_cost_ = __solution.cost();
				return true;
			}
			else {
				this->before_cost_ = __solution.cost();
				return false;
			}
		}

	private:
		//!一つ前のコスト
		cost_type before_cost_;

		/*!
		 * @brief			未定義コスト
		 * @detail			未定義コストを返す
		 * @return			cost_type	未定義値としてcost_typeの最大値を返す
		 */
		constexpr cost_type undefine_cost() const
		{
			return std::numeric_limits < cost_type > ::max();
		}
	};
} /* namespace okl */