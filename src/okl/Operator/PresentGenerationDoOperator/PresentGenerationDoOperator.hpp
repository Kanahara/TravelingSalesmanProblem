#pragma once
#include "../Operator.hpp"

namespace okl {
	/*!
	 *  @brief      現世代処理関数クラス
	 *  @details    現世代全てに指定の操作を行う関数クラス
	 *				- __Operator			操作関数クラス
	 */
	template<class __Operator>
	class PresentGenerationDoOperator : public Operator
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		PresentGenerationDoOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~PresentGenerationDoOperator() = default;

		using random_device_type = typename Operator::random_device_type;
		using seed_type = typename Operator::seed_type;
		using size_type = typename Operator::size_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "現世代に(" + __Operator::info() + ")"; }

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 *					メモリ解放は行わない
		 */
		inline void clear() override
		{
			Operator::clear();
			this->operator_.clear();
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
		}

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution, class _Population>
		inline void operator()(typename __Solution::graph_type& __graph, __SearchInfo& __search_info, _Population& __population, __Solution& __initial_solution, __Solution& __best_solution)
		{
			std::for_each(__population.present_generation().begin(), __population.present_generation().end(),
				[this, &__graph, &__search_info, &__population, &__initial_solution, &__best_solution](const auto _individual_id) {
					this->operator_(__graph, __search_info, __population.individual(_individual_id), __initial_solution, __best_solution);
				}
			);
		}

	private:
		//!行うオペレーター
		__Operator operator_;
	};
} /* namespace okl::pmp::cpmp */