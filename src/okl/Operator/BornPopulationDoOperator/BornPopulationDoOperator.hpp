#pragma once
#include "../Operator.hpp"

namespace okl {
	/*!
	 *  @brief      設定個体生成し処理関数クラス
	 *  @details    設定個体生成し指定の操作を行う関数クラス
	 *				- __Operator			操作関数クラス
	 */
	template<class __Operator>
	class BornPopulationDoOperator : public Operator
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		BornPopulationDoOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~BornPopulationDoOperator() = default;

		using random_device_type = typename Operator::random_device_type;
		using seed_type = typename Operator::seed_type;
		using size_type = typename Operator::size_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "設定個体生成し(" + __Operator::info() + ")"; }

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
			while (__population.size() < __search_info.pop_size()) {
				auto& _individual = __population.born();
				this->operator_(__graph, __search_info, _individual, __initial_solution, __best_solution);
			}
			__population.advanceGeneration();
		}

	private:
		//!行うオペレーター
		__Operator operator_;
	};
} /* namespace okl::pmp::cpmp */