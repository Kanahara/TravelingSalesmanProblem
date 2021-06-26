#pragma once

#include "../Operator.hpp"

namespace okl {
	/*!
	 *  @brief      探索関数クラスの前処理後処理関数クラス
	 *  @details    探索関数クラスの前処理後処理関数クラス
	 *				- __PreOperator			前処理関数クラス
	 *				- __Search				探索処理関数クラス
	 *				- __PostOperator		後処理関数クラス
	 */
	template<
		class __PreOperator,
		class __Search,
		class __PostOperator>
		class SearchPrePostOperator : public Operator
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		SearchPrePostOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~SearchPrePostOperator() = default;

		using random_device_type = typename Operator::random_device_type;
		using seed_type = typename Operator::seed_type;
		using size_type = typename Operator::size_type;

		using graph_type = typename __Search::graph_type;
		using solution_type = typename __Search::solution_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return __PreOperator::info() + "+" + __Search::info() + "+" + __PostOperator::info(); }

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 *					メモリ解放は行わない
		 */
		inline void clear() override
		{
			Operator::clear();
			this->pre_operator_.clear();
			this->post_operator_.clear();
			this->search_.clear();
		}

		/*!
		 * @brief			サイズ設定
		 * @param[in]		__size 頂点数
		 * @detail			全要素をメモリ確保を行う
		 */
		inline void resize(const size_type __size) override
		{
			Operator::resize(__size);
			this->pre_operator_.resize(__size);
			this->post_operator_.resize(__size);
			assert(__size <= std::numeric_limits<typename __Search::size_type>::max());
			this->search_.resize(static_cast<typename __Search::size_type>(__size));
		}

		/*!
		 * @brief			シードを設定
		 * @param[in]		__seed シード
		 * @detail			シードを設定する
		 * 					- seed_typeは標準ライブラリによって定義される
		 */
		inline void seed(const seed_type __seed)  override
		{
			Operator::seed(__seed);
			this->pre_operator_.seed(this->random_device_.uniform_int(std::numeric_limits < std::seed_seq::result_type >::min(), std::numeric_limits < std::seed_seq::result_type >::max()));
			this->post_operator_.seed(this->random_device_.uniform_int(std::numeric_limits < std::seed_seq::result_type >::min(), std::numeric_limits < std::seed_seq::result_type >::max()));
			this->search_.seed(this->random_device_.uniform_int(std::numeric_limits < std::seed_seq::result_type >::min(), std::numeric_limits < std::seed_seq::result_type >::max()));
		}

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		inline void operator()(typename __Solution::graph_type& __graph, __SearchInfo& __search_info, __Solution& __solution)
		{
			this->pre_operator_(__graph, __search_info, __solution);
			this->search_(__graph, __search_info, __solution);
			this->post_operator_(__graph, __search_info, __solution);
		}
		template<class __SearchInfo, class __Solution>
		inline void operator()(typename __Solution::graph_type& __graph, __SearchInfo& __search_info, __Solution& __solution, __Solution& __initial_solution, __Solution& __best_solution) { (*this)(__graph, __search_info, __solution); }

	private:
		//!前処理演算子
		__PreOperator pre_operator_;

		//!後処理演算子
		__PostOperator post_operator_;

		//!検索演算子
		__Search search_;
	};
} /* namespace okl */