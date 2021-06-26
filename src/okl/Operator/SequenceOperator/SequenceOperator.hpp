#pragma once

#include "../Operator.hpp"
#include "../NoneOperator/module.hpp"

namespace okl {
	/*!
	 *  @brief      順次実行関数クラス
	 *  @details    関数クラスA→関数クラスBの順に順次実行関数クラス
	 *				- __OperatorA			関数クラスA
	 *				- __OperatorB			関数クラスB
	 *				- __OperatorC			関数クラスC
	 *				- __OperatorD			関数クラスD
	 *				- __OperatorE			関数クラスE
	 */
	template<
		class __OperatorA,
		class __OperatorB,
		class __OperatorC = NoneOperator,
		class __OperatorD = NoneOperator,
		class __OperatorE = NoneOperator>
		class SequenceOperator : public Operator
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		SequenceOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~SequenceOperator() = default;

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
			Operator::clear();
			this->operator_a_.clear();
			this->operator_b_.clear();
			this->operator_c_.clear();
			this->operator_d_.clear();
			this->operator_e_.clear();
		}

		/*!
		 * @brief			サイズ設定
		 * @param[in]		__size 頂点数
		 * @detail			全要素をメモリ確保を行う
		 */
		inline void resize(const size_type __size) override
		{
			assert(__size <= std::numeric_limits<typename __OperatorA::size_type>::max());
			this->operator_a_.resize(static_cast<typename __OperatorA::size_type>(__size));
			assert(__size <= std::numeric_limits<typename __OperatorB::size_type>::max());
			this->operator_b_.resize(static_cast<typename __OperatorB::size_type>(__size));
			assert(__size <= std::numeric_limits<typename __OperatorC::size_type>::max());
			this->operator_c_.resize(static_cast<typename __OperatorC::size_type>(__size));
			assert(__size <= std::numeric_limits<typename __OperatorD::size_type>::max());
			this->operator_d_.resize(static_cast<typename __OperatorD::size_type>(__size));
			assert(__size <= std::numeric_limits<typename __OperatorE::size_type>::max());
			this->operator_e_.resize(static_cast<typename __OperatorE::size_type>(__size));
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
			this->operator_c_.seed(this->random_device_.uniform_int(std::numeric_limits < std::seed_seq::result_type >::min(), std::numeric_limits < std::seed_seq::result_type >::max()));
			this->operator_d_.seed(this->random_device_.uniform_int(std::numeric_limits < std::seed_seq::result_type >::min(), std::numeric_limits < std::seed_seq::result_type >::max()));
			this->operator_e_.seed(this->random_device_.uniform_int(std::numeric_limits < std::seed_seq::result_type >::min(), std::numeric_limits < std::seed_seq::result_type >::max()));
		}

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() {
			return
				(__OperatorA::info() == "" ? "" : __OperatorA::info() + "+") +
				(__OperatorB::info() == "" ? "" : __OperatorB::info() + "+") +
				(__OperatorC::info() == "" ? "" : __OperatorC::info() + "+") +
				(__OperatorD::info() == "" ? "" : __OperatorD::info() + "+") +
				(__OperatorE::info() == "" ? "" : __OperatorE::info() + "+");
		}

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		inline void operator()(typename __Solution::graph_type& __graph, __SearchInfo& __search_info, __Solution& __solution)
		{
			this->operator_a_(__graph, __search_info, __solution);
			this->operator_b_(__graph, __search_info, __solution);
			this->operator_c_(__graph, __search_info, __solution);
			this->operator_d_(__graph, __search_info, __solution);
			this->operator_e_(__graph, __search_info, __solution);
		}

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		inline void operator()(typename __Solution::graph_type& __graph, __SearchInfo& __search_info, __Solution& __solution, __Solution& __initial_solution, __Solution& __best_solution)
		{
			this->operator_a_(__graph, __search_info, __solution, __initial_solution, __best_solution);
			this->operator_b_(__graph, __search_info, __solution, __initial_solution, __best_solution);
			this->operator_c_(__graph, __search_info, __solution, __initial_solution, __best_solution);
			this->operator_d_(__graph, __search_info, __solution, __initial_solution, __best_solution);
			this->operator_e_(__graph, __search_info, __solution, __initial_solution, __best_solution);
		}

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution, class _Population>
		inline typename std::enable_if<!std::is_same<__Solution, _Population>::value>::type operator()(typename __Solution::graph_type& __graph, __SearchInfo& __search_info, _Population& __population, __Solution& __initial_solution, __Solution& __best_solution)
		{
			this->operator_a_(__graph, __search_info, __population, __initial_solution, __best_solution);
			this->operator_b_(__graph, __search_info, __population, __initial_solution, __best_solution);
			this->operator_c_(__graph, __search_info, __population, __initial_solution, __best_solution);
			this->operator_d_(__graph, __search_info, __population, __initial_solution, __best_solution);
			this->operator_e_(__graph, __search_info, __population, __initial_solution, __best_solution);
		}

	private:
		//!1番目演算子
		__OperatorA operator_a_;

		//!2番目演算子
		__OperatorB operator_b_;

		//!3番目演算子
		__OperatorC operator_c_;

		//!4番目演算子
		__OperatorD operator_d_;

		//!5番目演算子
		__OperatorE operator_e_;
	};
} /* namespace okl */