#pragma once
#include "../Random/module.hpp"
#include "../Result/module.hpp"
#include "../Timer/module.hpp"

#include <fstream>

namespace okl {
	/*!
	 *  @brief      探索関数クラス基底
	 *  @details    探索関数クラス基底クラス
	 *				- __Solution				解クラス
	 *				- __EndCondition			終了条件クラス
	 */
	template<
		class __Solution,
		class __EndCondition>
		class Search
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		Search() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~Search() = default;

		using graph_type = typename __Solution::graph_type;
		using solution_type = __Solution;

		using size_type = typename __Solution::size_type;

		using seed_type = std::seed_seq::result_type;
		using random_device_type = Random<>;

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 *					メモリ解放は行わない
		 */
		virtual void clear()
		{
			this->initial_solution_.clear();
			this->best_solution_.clear();
			this->end_condition_.clear();
		}

		/*!
		 * @brief			サイズ設定
		 * @param[in]		__size 頂点数
		 * @detail			全要素をメモリ確保を行う
		 */
		virtual void resize(const size_type __size)
		{
			this->initial_solution_.resize(__size);
			this->best_solution_.resize(__size);
			assert(__size <= std::numeric_limits<typename __EndCondition::size_type>::max());
			this->end_condition_.resize(static_cast<typename __EndCondition::size_type>(__size));
		}

		/*!
		 * @brief			シードを設定
		 * @param[in]		__seed シード
		 * @detail			メンバ変数のシードを設定する
		 * 					- seed_typeは標準ライブラリによって定義される
		 */
		virtual void seed(const seed_type __seed)
		{
			this->random_device_.seed(__seed);
			this->end_condition_.seed(__seed);
		}

	protected:
		//!乱数生成機
		random_device_type random_device_;

		/*!
		 * @brief			現在の解を初期解として保存
		 * @param[in]		__solution 現在の解
		 * @detail			現在の解を初期解として保存する
		 * 					- solution_typeはテンプレート__Solutionによって定義される
		 */
		inline void saveInitialSolution(const solution_type& __solution)
		{
			this->initial_solution_ = __solution;
		}

		/*!
		 * @brief			初期解を現在の解に読み込む
		 * @param[out]		__solution 現在の解
		 * @detail			初期解を現在の解に読み込む
		 * 					- solution_typeはテンプレート__Solutionによって定義される
		 */
		inline void loadInitialSolution(solution_type& __solution)
		{
			__solution = this->initial_solution_;
		}

		/*!
		 * @brief			解が改善していればベスト解として保存
		 * @param[out]		search_info_type 探索情報クラス
		 * @param[in]		solution_type	 解クラス
		 * @return			bool	解を更新したならTrue してないならFalse
		 * @detail			解が改善していればベスト解として保存する
		 * 					- search_info_typeはテンプレート__SearchInfoによって定義される
		 * 					- solution_typeはテンプレート__Solutionによって定義される
		 */
		template<class __SearchInfo>
		inline bool upgradeBestSolution(__SearchInfo& __search_info, const solution_type& __solution)
		{
			if (this->best_solution_ < __solution) {
				this->best_solution_ = __solution;
				if (__solution.meetConstraints()) {
					__search_info.setResult(__solution.cost());
				}
				return true;
			}
			else {
				return false;
			}
		}

		/*!
		 * @brief			最良解を現在の解に読み込む
		 * @param[out]		__solution 現在の解
		 * @detail			最良解を現在の解に読み込む
		 * 					- solution_typeはテンプレート__Solutionによって定義される
		 */
		inline void loadBestSolution(solution_type& __solution)
		{
			if (__solution <= this->best_solution_) {
				__solution = this->best_solution_;
			}
		}

		/*!
		 * @brief			探索終了条件判定
		 * @param[in]		graph_type		 グラフクラス
		 * @param[in]		search_info_type 探索情報クラス
		 * @param[out]		solution_type	 解クラス
		 * @return				bool	終了条件を満たしたならTrue 満たしてないならFalse
		 * @detail			最良解を現在の解に読み込む
		 * 					- graph_typeはテンプレート__Solutionのgraph_typeによって定義される
		 * 					- search_info_typeはテンプレート__SearchInfoによって定義される
		 * 					- solution_typeはテンプレート__Solutionによって定義される
		 */
		template<class __SearchInfo>
		inline bool isEnd(const graph_type& __graph, __SearchInfo& __search_info, solution_type& __solution)
		{
			return this->end_condition_(__graph, __search_info, __solution, this->initial_solution_, this->best_solution_);
		}

		//!初期解
		solution_type initial_solution_;

		//!最良解
		solution_type best_solution_;

	private:
		//!終了条件
		__EndCondition end_condition_;
	};
} /* namespace okl */