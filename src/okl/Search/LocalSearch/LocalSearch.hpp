#pragma once

#include "../Search.hpp"

namespace okl {
	/*!
	 *  @brief      局所探索関数クラス
	 *  @details    局所探索関数クラス
	 *				定義された近傍移動による局所探索を行う
	 *				- __Solution				解クラス
	 *				- __MoveOperator			近傍移動クラス
	 *				- __EndCondition			終了条件クラス
	 */
	template<
		class __Solution,
		class __MoveOperator,
		class __EndCondition>
		class LocalSearch : public  Search<__Solution, __EndCondition>
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		LocalSearch() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~LocalSearch() = default;

		using graph_type = typename  Search<__Solution, __EndCondition>::graph_type;
		using solution_type = typename  Search<__Solution, __EndCondition>::solution_type;

		using size_type = typename Search<__Solution, __EndCondition>::size_type;

		using seed_type = typename   Search<__Solution, __EndCondition>::seed_type;
		using random_device_type = typename  Search<__Solution, __EndCondition>::random_device_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "局所探索<" + __EndCondition::info() + ">(" + __MoveOperator::info() + ")"; }

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 *					メモリ解放は行わない
		 */
		inline void clear() override
		{
			Search<__Solution, __EndCondition>::clear();
			this->move_operator_.clear();
		}

		/*!
		 * @brief			サイズ設定
		 * @param[in]		__size 頂点数
		 * @detail			全要素をメモリ確保を行う
		 */
		inline void resize(const size_type __size) override
		{
			Search<__Solution, __EndCondition>::resize(__size);
			this->move_operator_.resize(__size);
		}

		/*!
		 * @brief			シードを設定
		 * @param[in]		__seed シード
		 * @detail			メンバ変数のシードを設定する
		 * 					- seed_typeは標準ライブラリによって定義される
		 */
		inline void seed(const seed_type __seed) override
		{
			Search<__Solution, __EndCondition>::seed(__seed);
			this->move_operator_.seed(this->random_device_.uniform_int(std::numeric_limits < std::seed_seq::result_type >::min(), std::numeric_limits < std::seed_seq::result_type >::max()));
		}

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo>
		inline void operator()(graph_type& __graph, __SearchInfo& __search_info, solution_type& __solution)
		{
			this->clear();

			//this->saveInitialSolution(__solution);
			//this->upgradeBestSolution(__solution);

			while (!this->isEnd(__graph, __search_info, __solution)) {
				this->MovePhase(__graph, __search_info, __solution);
			}

			if (!this->upgradeBestSolution(__search_info, __solution)) {
				this->loadBestSolution(__solution);
			}
		}

		template<class __SearchInfo>
		inline void operator()(graph_type& __graph, __SearchInfo& __search_info, solution_type& __solution, solution_type& __initial_solution, solution_type& __best_solution)
		{
			(*this)(__graph, __search_info, __solution);
		}

	protected:
		/*!
		 * @brief			近傍移動
		 * @param[in]		graph_type		 グラフクラス
		 * @param[in]		search_info_type 探索情報クラス
		 * @param[out]		solution_type	 解クラス
		 * @detail			近傍移動を行う
		 * 					- graph_typeはテンプレート__Solutionのgraph_typeによって定義される
		 * 					- search_info_typeはテンプレート__SearchInfoによって定義される
		 * 					- solution_typeはテンプレート__Solutionによって定義される
		 */
		template<class __SearchInfo>
		inline void MovePhase(graph_type& __graph, __SearchInfo& __search_info, solution_type& __solution)
		{
			this->move_operator_(__graph, __search_info, __solution, this->initial_solution_, this->best_solution_);
			//std::cout << "--ムーブ-----" + __MoveOperator::info() << std::endl;
			//std::cout << __solution << std::endl;
			//std::cout << "-------" << std::endl;
		}

	private:
		//!移動演算子
		__MoveOperator move_operator_;
	};
} /* namespace okl */