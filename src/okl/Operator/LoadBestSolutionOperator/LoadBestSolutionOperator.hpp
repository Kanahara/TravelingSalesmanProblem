#pragma once

#include "../Operator.hpp"

namespace okl {
	/*!
	 *  @brief      ベスト解読み込み関数クラス
	 *  @details    ベスト解を現在の解に読み込む関数クラス
	 */
	class LoadBestSolutionOperator : public Operator
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		LoadBestSolutionOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~LoadBestSolutionOperator() = default;

		using random_device_type = typename Operator::random_device_type;
		using seed_type = typename Operator::seed_type;
		using size_type = typename Operator::size_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "最良解読込"; }

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		inline void operator()(const typename __Solution::graph_type& __graph, const __SearchInfo& __search_info, __Solution& __solution, const __Solution& __initial_solution, const __Solution& __best_solution)
		{
			assert(__solution <= __best_solution);
			__solution = __best_solution;
		}
	};
} /* namespace okl */