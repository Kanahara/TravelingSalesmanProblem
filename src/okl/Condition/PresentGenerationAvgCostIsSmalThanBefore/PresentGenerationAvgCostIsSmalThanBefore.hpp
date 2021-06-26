#pragma once
#include "../Condition.hpp"
#include <limits>
#include <numeric>
#include <algorithm>

namespace okl {
	/*!
	 *  @brief      ベストコストが一つ前の状態より小さいか判定する
	 *  @details    ベストコストが一つ前の状態より小さいか判定するクラス
	 *				小さい場合はTrue
	 *				- __Cost				コスト数値型
	 */
	template<typename __Cost>
	class PresentGenerationAvgCostIsSmalThanBefore : public Condition
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		PresentGenerationAvgCostIsSmalThanBefore() : before_cost_(this->undefine_cost()) {};

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~PresentGenerationAvgCostIsSmalThanBefore() = default;

		using seed_type = typename Condition::seed_type;
		using random_device_type = typename Condition::random_device_type;
		using size_type = typename Condition::size_type;

		using cost_type = __Cost;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "解集団内の解平均値が更新されたら"; }

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 */
		inline void clear() override
		{
			this->before_cost_ = this->undefine_cost();
		}

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @return			bool	現在のTrue　Falseを返す
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution, class _Population>
		inline bool operator()(const typename __Solution::graph_type& __graph, const __SearchInfo& __search_info, _Population& __population, const __Solution& __initial_solution, const __Solution& __best_solution)
		{
			using cost_type = typename __Solution::cost_type;
			cost_type _avg_cost = 0;
			std::for_each(__population.present_generation().begin(), __population.present_generation().end(),
				[&__population, &_avg_cost](const auto _individual_id) {
					_avg_cost += __population.individual(_individual_id).cost();
				}
			);
			_avg_cost /= static_cast<cost_type>(__population.present_generation().size());

			if (this->before_cost_ > _avg_cost || this->before_cost_ == this->undefine_cost()) {
				this->before_cost_ = _avg_cost;
				return true;
			}
			else {
				this->before_cost_ = _avg_cost;
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