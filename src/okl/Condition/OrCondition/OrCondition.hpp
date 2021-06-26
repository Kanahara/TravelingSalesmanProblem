#pragma once

#include "../Condition.hpp"

namespace okl {
	/*!
	 *  @brief      二つの条件クラスのor演算子結果を返す
	 *  @details    二つの条件クラスのor演算子結果を返す条件クラス
	 *				- __ConditionA				条件クラス一つ目
	 *				- __ConditionB				条件クラス二つ目
	 */
	template<class __ConditionA, class __ConditionB>
	class OrCondition : public Condition
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		OrCondition() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~OrCondition() = default;

		using seed_type = typename Condition::seed_type;
		using random_device_type = typename Condition::random_device_type;
		using size_type = typename Condition::size_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return __ConditionA::info() + " or " + __ConditionB::info(); }

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 */
		inline void clear() override
		{
			Condition::clear();
			this->condition_a_.clear();
			this->condition_b_.clear();
		}

		/*!
		 * @brief			サイズ設定
		 * @param[in]		__size 頂点数
		 * @detail			全要素をメモリ確保を行う
		 */
		inline void resize(const size_type __size) override
		{
			Condition::resize(__size);
			this->condition_a_.resize(__size);
			this->condition_b_.resize(__size);
		}

		/*!
		 * @brief			シードを設定
		 * @param[in]		__seed シード
		 * @detail			シードを設定する
		 * 					- seed_typeは標準ライブラリによって定義される
		 */
		inline void seed(const seed_type __seed) override
		{
			Condition::seed(__seed);
			this->condition_a_.seed(this->random_device_.uniform_int(std::numeric_limits < std::seed_seq::result_type >::min(), std::numeric_limits < std::seed_seq::result_type >::max()));
			this->condition_b_.seed(this->random_device_.uniform_int(std::numeric_limits < std::seed_seq::result_type >::min(), std::numeric_limits < std::seed_seq::result_type >::max()));
		}

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @return			bool	現在のTrue　Falseを返す
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		inline bool operator()(const typename __Solution::graph_type& __graph, __SearchInfo& __search_info, __Solution& __solution)
		{
			return this->condition_a_(__graph, __search_info, __solution) || this->condition_b_(__graph, __search_info, __solution);
		}
		template<class __SearchInfo, class __Solution>
		inline bool operator()(const typename __Solution::graph_type& __graph, __SearchInfo& __search_info, __Solution& __solution, const __Solution& __initial_solution, const __Solution& __best_solution)
		{
			return this->condition_a_(__graph, __search_info, __solution, __initial_solution, __best_solution) || this->condition_b_(__graph, __search_info, __solution, __initial_solution, __best_solution);
		}
		template<class __SearchInfo, class __Solution, class _Population>
		inline typename std::enable_if<!std::is_same<__Solution, _Population>::value, bool>::type operator()(const typename __Solution::graph_type& __graph, __SearchInfo& __search_info, _Population& __population, const __Solution& __initial_solution, __Solution& __best_solution)
		{
			return (*this)(__graph, __search_info, __population.individual(*__population.present_generation().begin()), __initial_solution, __best_solution);
		}

	private:
		//!条件クラスA
		__ConditionA condition_a_;

		//!条件クラスB
		__ConditionB condition_b_;
	};
} /* namespace okl */