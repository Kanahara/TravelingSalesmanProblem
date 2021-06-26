#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <codecvt>
#include <random>
#include <string>

namespace okl {
	/*!
	 *  @brief      例題クラス
	 *  @details    例題クラス
	 *				- __Cost				コスト数値型
	 */
	template<class __Cost>
	class Instance {
	public:
		/*!
		 * @brief				コンストラクタ
		 * @param[in]		__name 例題名
		 * @param[in]		__best_known_cost 既知の最良解
　		 * @param[in]		__initial_seed 初期シード
		 */
		Instance(const std::string __name, const __Cost __best_known_cost, const std::seed_seq::result_type __initial_seed, const std::string __type = "") :
			name_(__name), best_known_cost_(__best_known_cost), initial_seed_(__initial_seed), type_(__type) {}

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~Instance() = default;

		using cost_type = __Cost;
		using seed_type = std::seed_seq::result_type;

		/*!
		 * @brief			例題読み込み
		 * @return			std::vector<Instance<__Cost>>	例題配列を返す
		 * @detail			実装により例題配列の型は変更される
		 * 					 - _CostComparisonFunction				コスト比較関数
		 */
		template<class _CostComparisonFunction>
		static const std::vector<Instance<__Cost>> load(const std::string& __path);

		/*!
		 * @brief			例題名を返す
		 * @return			std::string&	例題名の参照を返す
		 * @detail
		 */
		inline const std::string& name() const
		{
			return this->name_;
		}

		/*!
		 * @brief			既知の最良解を返す
		 * @return			cost_type		既知の最良解を返す
		 * @detail
		 *					 - 数値型cost_typeはテンプレート__Costによって定義される
		 */
		constexpr cost_type best_known_cost() const
		{
			return this->best_known_cost_;
		}

		/*!
		 * @brief			初期シードを返す
		 * @return			seed_type		初期シードを返す
		 * @detail
		 *					 - 数値型seed_typeは標準ライブラリによって定義される
		 */
		constexpr seed_type initial_seed() const
		{
			return initial_seed_;
		}

		/*!
		 * @brief			種類を返す
		 * @return			std::string&	種類名の参照を返す
		 * @detail
		 */
		inline const std::string& type() const
		{
			return this->type_;
		}

	private:
		//!例題名
		std::string name_;

		//!既知の最良解の値
		__Cost best_known_cost_;

		//!初期シード
		seed_type initial_seed_;

		//!Type名
		std::string type_;
	};
} /* namespace okl */

#include "detile.hpp"
#include "operator.hpp"