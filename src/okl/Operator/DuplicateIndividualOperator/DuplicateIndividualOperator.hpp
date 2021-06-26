#pragma once
#include "../Operator.hpp"
#include <unordered_set>

namespace okl {
	/*!
	 *  @brief      解集団内の重複個体の片方に処理関数クラス
	 *  @details    解集団内の重複個体の片方に対して指定の操作を行う関数クラス
	 */
	template<class __Operator, class __Condition, class __Set>
	class DuplicateIndividualOperator : public Operator
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		DuplicateIndividualOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~DuplicateIndividualOperator() = default;

		using random_device_type = typename Operator::random_device_type;
		using seed_type = typename Operator::seed_type;
		using size_type = typename Operator::size_type;

		using set_type = __Set;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "解集団内の重複個体の片方に対して(" + __Operator::info() + ")"; }

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 *					メモリ解放は行わない
		 */
		inline void clear() override
		{
			Operator::clear();
			this->operator_.clear();
			this->condition_.clear();
			this->cost_set_.clear();
		}

		/*!
		 * @brief			サイズ設定
		 * @param[in]		__size 頂点数
		 * @detail			全要素をメモリ確保を行う
		 */
		inline void resize(const size_type __size) override
		{
			Operator::resize(__size);
			assert(__size <= std::numeric_limits<typename __Operator::size_type>::max());
			this->operator_.resize(static_cast<typename __Operator::size_type>(__size));
			this->cost_set_.reserve(__size);
		}

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution, class _Population>
		inline void operator()(typename __Solution::graph_type& __graph, __SearchInfo& __search_info, _Population& __population, __Solution& __initial_solution, __Solution& __best_solution)
		{
			this->cost_set_.clear();
			std::for_each(__population.present_generation().begin(), __population.present_generation().end(),
				[this, &__graph, &__search_info, &__population, &__initial_solution, &__best_solution](const auto _individual_id) {
					const auto _cost = __population.individual(_individual_id).cost();
					const bool _contains = !(this->cost_set_.insert(_cost).second);
					//重複があるなら
					if (_contains) {
						this->operator_(__graph, __search_info, __population.individual(_individual_id), __initial_solution, __best_solution);
					}
				}
			);
		}

	private:
		//!行うオペレーター
		__Operator operator_;

		//!終了条件
		__Condition condition_;

		//!コスト重複判定用集合
		set_type cost_set_;
	};
} /* namespace okl::pmp::cpmp */