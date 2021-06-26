#pragma once

#include "../Search.hpp"
#include "../../HashSet/module.hpp"

namespace okl {
	/*!
	 *  @brief      局所探索関数クラス
	 *  @details    局所探索関数クラス
	 *				定義された近傍移動による局所探索を行う
	 *				- __Population				募集団クラス
	 *				- __Start					初期解生成クラス
	 *				- __Crossover				考査クラス
	 *				- __Mutation				突然変異クラス
	 *				- __Selection				個体選択クラス
	 *				- __EndCondition			終了条件クラス
	 */
	template<
		class __Population,
		class __Start,
		class __Crossover,
		class __Mutation,
		class __Selection,
		class __EndCondition>
		class GeneticAlgorithm : public Search<typename __Population::individual_type, __EndCondition>
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		GeneticAlgorithm() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~GeneticAlgorithm() = default;

		using graph_type = typename Search<typename __Population::individual_type, __EndCondition>::graph_type;
		using solution_type = typename Search<typename __Population::individual_type, __EndCondition>::solution_type;

		using size_type = typename Search<solution_type, __EndCondition>::size_type;

		using seed_type = typename  Search<solution_type, __EndCondition>::seed_type;
		using random_device_type = typename Search<solution_type, __EndCondition>::random_device_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "遺伝的アルゴリズム<" + __EndCondition::info() + ">(" + __Start::info() + ")(" + __Crossover::info() + ")(" + __Mutation::info() + ")(" + __Selection::info() + ")"; }

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 *					メモリ解放は行わない
		 */
		inline void clear() override
		{
			Search<solution_type, __EndCondition>::clear();
			this->start_.clear();
			this->crossover_.clear();
			this->mutation_.clear();
			this->selection_.clear();
			this->population_.clear();
		}

		/*!
		 * @brief			サイズ設定
		 * @param[in]		__size 頂点数
		 * @detail			全要素をメモリ確保を行う
		 */
		inline void resize(const size_type __size) override
		{
			Search<solution_type, __EndCondition>::resize(__size);
			this->start_.resize(__size);
			this->crossover_.resize(__size);
			this->mutation_.resize(__size);
			this->selection_.resize(__size);
			this->population_.resize(__size);
		}

		/*!
		 * @brief			シードを設定
		 * @param[in]		__seed シード
		 * @detail			メンバ変数のシードを設定する
		 * 					- seed_typeは標準ライブラリによって定義される
		 */
		inline void seed(const seed_type __seed) override
		{
			Search<solution_type, __EndCondition>::seed(__seed);
			this->start_.seed(this->random_device_.uniform_int(std::numeric_limits < std::seed_seq::result_type >::min(), std::numeric_limits < std::seed_seq::result_type >::max()));
			this->crossover_.seed(this->random_device_.uniform_int(std::numeric_limits < std::seed_seq::result_type >::min(), std::numeric_limits < std::seed_seq::result_type >::max()));
			this->mutation_.seed(this->random_device_.uniform_int(std::numeric_limits < std::seed_seq::result_type >::min(), std::numeric_limits < std::seed_seq::result_type >::max()));
			this->selection_.seed(this->random_device_.uniform_int(std::numeric_limits < std::seed_seq::result_type >::min(), std::numeric_limits < std::seed_seq::result_type >::max()));
		}

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo>
		void operator()(graph_type& __graph, __SearchInfo& __search_info, solution_type& __solution)
		{
			this->clear();
			this->saveInitialSolution(__solution);
			this->Start(__graph, __search_info);

			while (!this->isEnd(__graph, __search_info, __solution))
			{
				this->Crossover(__graph, __search_info);

				this->Mutation(__graph, __search_info);

				this->Selection(__graph, __search_info);
			}
		}

		template<class __SearchInfo>
		void operator()(graph_type& __graph, __SearchInfo& __search_info, solution_type& __solution, solution_type& __initial_solution, solution_type& __best_solution) {
			(*this)(__graph, __search_info, __solution);
		}

	protected:
		/*!
		 * @brief			初期解生成
		 * @param[in]		graph_type		 グラフクラス
		 * @param[out]		__SearchInfo 探索情報クラス
		 * @detail			初期解生成を行う
		 * 					- graph_typeはテンプレート__LocalSearchのsolution_typeのgraph_typeによって定義される
		 * 					- __SearchInfoはテンプレート__SearchInfoによって定義される
		 */
		template<class __SearchInfo>
		inline void Start(graph_type& __graph, __SearchInfo& __search_info)
		{
			this->start_(__graph, __search_info, this->population_, this->initial_solution_, this->best_solution_);
			for (const auto _individual_id : this->population_.present_generation()) {
				this->upgradeBestSolution(__search_info, this->population_.individual(_individual_id));
			}
		}

		/*!
		 * @brief			交叉
		 * @param[in]		graph_type		 グラフクラス
		 * @param[out]		__SearchInfo 探索情報クラス
		 * @detail			交叉を行う
		 * 					- graph_typeはテンプレート__LocalSearchのsolution_typeのgraph_typeによって定義される
		 * 					- __SearchInfoはテンプレート__SearchInfoによって定義される
		 */
		template<class __SearchInfo>
		inline void Crossover(graph_type& __graph, __SearchInfo& __search_info)
		{
			this->crossover_(__graph, __search_info, this->population_, this->initial_solution_, this->best_solution_);
			for (const auto _individual_id : this->population_.next_generation()) {
				this->upgradeBestSolution(__search_info, this->population_.individual(_individual_id));
			}
		}

		/*!
		 * @brief			突然変異
		 * @param[in]		graph_type		 グラフクラス
		 * @param[out]		__SearchInfo 探索情報クラス
		 * @detail			突然変異を行う
		 * 					- graph_typeはテンプレート__LocalSearchのsolution_typeのgraph_typeによって定義される
		 * 					- __SearchInfoはテンプレート__SearchInfoによって定義される
		 */
		template<class __SearchInfo>
		inline void Mutation(graph_type& __graph, __SearchInfo& __search_info)
		{
			this->mutation_(__graph, __search_info, this->population_, this->initial_solution_, this->best_solution_);
			for (const auto _individual_id : this->population_.present_generation()) {
				this->upgradeBestSolution(__search_info, this->population_.individual(_individual_id));
			}
		}

		/*!
		 * @brief			選択
		 * @param[in]		graph_type		 グラフクラス
		 * @param[out]		__SearchInfo 探索情報クラス
		 * @detail			選択を行う
		 * 					- graph_typeはテンプレート__LocalSearchのsolution_typeのgraph_typeによって定義される
		 * 					- __SearchInfoはテンプレート__SearchInfoによって定義される
		 */
		template<class __SearchInfo>
		inline void Selection(graph_type& __graph, __SearchInfo& __search_info)
		{
			this->selection_(__graph, __search_info, this->population_, this->initial_solution_, this->best_solution_);
		}

	private:
		//!母集団
		__Population population_;

		//!交叉
		__Crossover crossover_;

		//!突然変異
		__Mutation mutation_;

		//!選択
		__Selection selection_;

		//!初期解生成
		__Start start_;
	};
} /* namespace okl */