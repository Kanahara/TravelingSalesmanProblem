#pragma once

#include "../Operator.hpp"

namespace okl {
	/*!
	 *  @brief      異なる解に処理関数クラス
	 *  @details    異なる解に処理を行う関数クラス
	 *				- __OtherSolution		通常とは違う解クラス
	 *				- __Search				処理関数クラス
	 */
	template<
		class __OtherSolution,
		class __Operator>
		class OtherSolutionOperator : public Operator
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		OtherSolutionOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~OtherSolutionOperator() = default;

		using random_device_type = typename Operator::random_device_type;
		using seed_type = typename Operator::seed_type;
		using size_type = typename Operator::size_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return __Operator::info(); }

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 *					メモリ解放は行わない
		 */
		inline virtual void clear() override
		{
			Operator::clear();
			this->other_solution_.clear();
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
			assert(__size <= std::numeric_limits<typename __OtherSolution::size_type>::max());
			this->other_solution_.resize(static_cast<typename __OtherSolution::size_type>(__size));
			this->operator_.resize(__size);
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
			this->operator_.seed(this->random_device_.uniform_int(std::numeric_limits < std::seed_seq::result_type >::min(), std::numeric_limits < std::seed_seq::result_type >::max()));
		}

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		inline void operator()(typename __Solution::graph_type& __graph, __SearchInfo& __search_info, __Solution& __solution)
		{
			this->operator_(__graph, __search_info, this->other_solution_);
		}

	private:
		//!後処理演算子
		__OtherSolution other_solution_;

		//!検索演算子
		__Operator operator_;
	};
} /* namespace okl */