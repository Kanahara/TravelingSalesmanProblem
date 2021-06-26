#pragma once
#include "../Condition.hpp"
#include <string>

namespace okl {
	/*!
	 *  @brief     現世代に同じ解があるか判定する
	 *  @details   現世代に同じ解があるか判定するクラス
	 *				現世代に同じ解がある場合はTrue
	 *				- __Set		コスト集合クラス
	 */
	template<class __Set>
	class SameCostIndividualContainsPresentGeneration : public Condition
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		SameCostIndividualContainsPresentGeneration() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~SameCostIndividualContainsPresentGeneration() = default;

		using seed_type = typename Condition::seed_type;
		using random_device_type = typename Condition::random_device_type;
		using size_type = typename Condition::size_type;

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 */
		inline void clear() override
		{
			Condition::clear();
			this->cost_set_.clear();
		}

		/*!
		 * @brief			サイズ設定
		 * @param[in]		__size 頂点数
		 * @detail			全要素をメモリ確保を行う
		 */
		inline void resize(const size_type __size) override
		{
			Condition::resize(__size);
			this->cost_set_.reserve(__size);
		}

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "現世代に同じ解があるか"; }

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @return			bool	現在のTrue　Falseを返す
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution, class _Population>
		inline bool operator()(const typename __Solution::graph_type& __graph, const __SearchInfo& __search_info, const _Population& __population, const __Solution& __initial_solution, const __Solution& __best_solution)
		{
			this->cost_set_.clear();
			for (const auto _individual_id : __population.present_generation()) {
				const auto _cost = __population.individual(_individual_id).cost();
				const bool _contains = !(this->cost_set_.insert(_cost).second);
				//重複があるなら
				if (_contains) {
					return false;
				}
			}
			return true;
		}

	private:
		//!コスト集合
		__Set cost_set_;
	};
} /* namespace okl */