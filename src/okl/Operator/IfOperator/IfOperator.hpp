#pragma once

#include "../Operator.hpp"
#include "../NoneOperator/module.hpp"

namespace okl {
	/*!
	 *  @brief      条件により関数を切り替え実行関数クラス
	 *  @details    関数クラスAを関数クラスBを条件クラスによって切り替え実行する関数クラス
	 *				条件クラスがTrueなら関数クラスAをFalseなら関数クラスBを実行する
	 *				- __OperatorA			関数クラスA
	 *				- __OperatorB			関数クラスB
	 */
	template<
		class __Condition,
		class __OperatorA,
		class __OperatorB = okl::NoneOperator>
		class IfOperator : public Operator
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		IfOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~IfOperator() = default;

		using random_device_type = typename Operator::random_device_type;
		using seed_type = typename Operator::seed_type;
		using size_type = typename Operator::size_type;

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 *					メモリ解放は行わない
		 */
		inline void clear() override
		{
			this->operator_a_.clear();
			this->operator_b_.clear();
			this->condition_.clear();
		}

		/*!
		 * @brief			サイズ設定
		 * @param[in]		__size 頂点数
		 * @detail			全要素をメモリ確保を行う
		 */
		void resize(const size_type __size) override
		{
			this->operator_a_.resize(__size);
			this->operator_b_.resize(__size);
		}

		/*!
		 * @brief			シードを設定
		 * @param[in]		__seed シード
		 * @detail			シードを設定する
		 * 					- seed_typeは標準ライブラリによって定義される
		 */
		inline void seed(const seed_type __seed) override
		{
			Operator::seed(__seed);
			this->operator_a_.seed(this->random_device_.uniform_int(std::numeric_limits < std::seed_seq::result_type >::min(), std::numeric_limits < std::seed_seq::result_type >::max()));
			this->operator_b_.seed(this->random_device_.uniform_int(std::numeric_limits < std::seed_seq::result_type >::min(), std::numeric_limits < std::seed_seq::result_type >::max()));
		}

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "IF<" + __Condition::info() + ">(" + __OperatorA::info() + ")(" + __OperatorB::info() + ")"; }

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		inline void operator()(typename __Solution::graph_type& __graph, __SearchInfo& __search_info, __Solution& __solution)
		{
			if (this->condition_(__graph, __search_info, __solution)) {
				this->operator_a_(__graph, __search_info, __solution);
			}
			else {
				this->operator_b_(__graph, __search_info, __solution);
			}
		}

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		inline void operator()(typename __Solution::graph_type& __graph, __SearchInfo& __search_info, __Solution& __solution, __Solution& __initial_solution, __Solution& __best_solution)
		{
			if (this->condition_(__graph, __search_info, __solution, __initial_solution, __best_solution)) {
				this->operator_a_(__graph, __search_info, __solution, __initial_solution, __best_solution);
			}
			else {
				this->operator_b_(__graph, __search_info, __solution, __initial_solution, __best_solution);
			}
		}

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution, class _Container>
		inline void operator()(typename __Solution::graph_type& __graph, __SearchInfo& __search_info, __Solution& __solution, __Solution& __initial_solution, __Solution& __best_solution, const _Container& __target_vertex_set)
		{
			if (this->condition_(__graph, __search_info, __solution, __initial_solution, __best_solution)) {
				this->operator_a_(__graph, __search_info, __solution, __initial_solution, __best_solution, __target_vertex_set);
			}
			else {
				this->operator_b_(__graph, __search_info, __solution, __initial_solution, __best_solution, __target_vertex_set);
			}
		}

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution, class _Population>
		inline typename std::enable_if<!std::is_same<__Solution, _Population>::value>::type operator()(typename __Solution::graph_type& __graph, __SearchInfo& __search_info, _Population& __population, __Solution& __initial_solution, __Solution& __best_solution)
		{
			if (this->condition_(__graph, __search_info, __population, __initial_solution, __best_solution)) {
				this->operator_a_(__graph, __search_info, __population, __initial_solution, __best_solution);
			}
			else {
				this->operator_b_(__graph, __search_info, __population, __initial_solution, __best_solution);
			}
		}

	private:
		//!1番目演算子
		__OperatorA operator_a_;

		//!2番目演算子
		__OperatorB operator_b_;

		//切り替え条件
		__Condition condition_;
	};
} /* namespace okl */