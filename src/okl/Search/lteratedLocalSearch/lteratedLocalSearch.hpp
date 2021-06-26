#pragma once

#include "../Search.hpp"

namespace okl {
	/*!
	 *  @brief      反復局所探索関数クラス
	 *  @details    反復局所探索関数クラス
	 *				定義された局所探索と初期解生成と摂動により反復局所探索を行う
	 *				- __LocalSearch				局所探索クラス
	 *				- __Start					初期解生成クラス
	 *				- __Perturbation			摂動クラス
	 *				- __EndCondition			終了条件クラス
	 */
	template<
		class __LocalSearch,
		class __Start,
		class __Perturbation,
		class __EndCondition>
		class lteratedLocalSearch : public Search< typename __LocalSearch::solution_type, __EndCondition>
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		lteratedLocalSearch() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~lteratedLocalSearch() = default;

		using graph_type = typename Search<typename __LocalSearch::solution_type, __EndCondition>::graph_type;;
		using solution_type = typename Search<typename __LocalSearch::solution_type, __EndCondition>::solution_type;

		using size_type = typename Search<typename __LocalSearch::solution_type, __EndCondition>::size_type;

		using seed_type = typename  Search<solution_type, __EndCondition>::seed_type;
		using random_device_type = typename Search<solution_type, __EndCondition>::random_device_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "反復局所探索法<" + __EndCondition::info() + ">(" + __LocalSearch::info() + ")(" + __Start::info() + ")(" + __Perturbation::info() + ")"; }

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 *					メモリ解放は行わない
		 */
		inline void clear() override
		{
			Search<solution_type, __EndCondition>::clear();
			this->local_search_.clear();
			this->start_.clear();
			this->perturbation_.clear();
		}

		/*!
		 * @brief			サイズ設定
		 * @param[in]		__size 頂点数
		 * @detail			全要素をメモリ確保を行う
		 */
		inline void resize(const size_type __size) override
		{
			Search<solution_type, __EndCondition>::resize(__size);
			this->local_search_.resize(__size);
			this->start_.resize(__size);
			this->perturbation_.resize(__size);
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
			this->local_search_.seed(this->random_device_.uniform_int(std::numeric_limits < std::seed_seq::result_type >::min(), std::numeric_limits < std::seed_seq::result_type >::max()));
			this->start_.seed(this->random_device_.uniform_int(std::numeric_limits < std::seed_seq::result_type >::min(), std::numeric_limits < std::seed_seq::result_type >::max()));
			this->perturbation_.seed(this->random_device_.uniform_int(std::numeric_limits < std::seed_seq::result_type >::min(), std::numeric_limits < std::seed_seq::result_type >::max()));
		}

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo>
		inline void operator()(graph_type& __graph, __SearchInfo& __search_info, solution_type& __solution)
		{
			this->clear();

			this->saveInitialSolution(__solution);

			this->Start(__graph, __search_info, __solution);

			while (true)
			{
				this->LocalSearch(__graph, __search_info, __solution);
				if (this->isEnd(__graph, __search_info, __solution)) {
					break;
				}
				else {
					this->Perturbation(__graph, __search_info, __solution);
				}
			}
			this->loadBestSolution(__solution);
		}

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo>
		void operator()(graph_type& __graph, __SearchInfo& __search_info, solution_type& __solution, solution_type& __initial_solution, solution_type& __best_solution)
		{
			(*this)(__graph, __search_info, __solution);
		}

	protected:
		/*!
		 * @brief			局所探索
		 * @param[in]		graph_type		 グラフクラス
		 * @param[out]		__SearchInfo 探索情報クラス
		 * @param[out]		solution_type	 解クラス
		 * @detail			局所探索を行う
		 * 					- graph_typeはテンプレート__LocalSearchのsolution_typeのgraph_typeによって定義される
		 * 					- __SearchInfoはテンプレート__SearchInfoによって定義される
		 * 					- solution_typeはテンプレート__LocalSearchのsolution_typeによって定義される
		 */
		template<class __SearchInfo>
		inline void LocalSearch(graph_type& __graph, __SearchInfo& __search_info, solution_type& __solution)
		{
			this->local_search_(__graph, __search_info, __solution);
			this->upgradeBestSolution(__search_info, __solution);
			//std::cout << "--局所探索-----" + __LocalSearch::info() << std::endl;
			//std::cout << __solution << std::endl;
			//std::cout << "-------" << std::endl;
		}

		/*!
		 * @brief			初期解生成
		 * @param[in]		graph_type		 グラフクラス
		 * @param[out]		__SearchInfo 探索情報クラス
		 * @param[out]		solution_type	 解クラス
		 * @detail			初期解生成を行う
		 * 					- graph_typeはテンプレート__LocalSearchのsolution_typeのgraph_typeによって定義される
		 * 					- __SearchInfoはテンプレート__SearchInfoによって定義される
		 * 					- solution_typeはテンプレート__LocalSearchのsolution_typeによって定義される
		 */
		template<class __SearchInfo>
		inline void Start(graph_type& __graph, __SearchInfo& __search_info, solution_type& __solution)
		{
			//std::cout << "--初期解生成前-----" + __Start::info() << std::endl;
			//std::cout << __solution << std::endl;
			//std::cout << "-------" << std::endl;
			this->start_(__graph, __search_info, __solution, this->initial_solution_, this->best_solution_);
			//std::cout << "--初期解生成後-----" + __Start::info() << std::endl;
			//std::cout << __solution << std::endl;
			//std::cout << "-------" << std::endl;
		}

		/*!
		 * @brief			摂動
		 * @param[in]		graph_type		 グラフクラス
		 * @param[out]		__SearchInfo 探索情報クラス
		 * @param[out]		solution_type	 解クラス
		 * @detail			摂動を行う
		 * 					- graph_typeはテンプレート__LocalSearchのsolution_typeのgraph_typeによって定義される
		 * 					- __SearchInfoはテンプレート__SearchInfoによって定義される
		 * 					- solution_typeはテンプレート__LocalSearchのsolution_typeによって定義される
		 */
		template<class __SearchInfo>
		inline void Perturbation(graph_type& __graph, __SearchInfo& __search_info, solution_type& __solution)
		{
			this->perturbation_(__graph, __search_info, __solution, this->initial_solution_, this->best_solution_);
			//std::cout << "--摂動-----" + __Perturbation::info() << std::endl;
			//std::cout << __solution << std::endl;
			//std::cout << "-------" << std::endl;
		}

	private:
		//!局所探索
		__LocalSearch local_search_;

		//!初期解生成
		__Start start_;

		//!摂動演算子
		__Perturbation perturbation_;
	};
} /* namespace okl */