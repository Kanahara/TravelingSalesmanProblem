#pragma once

#include "../Operator.hpp"
#include <string>

namespace okl {
	/*!
	 *  @brief      解にグラフの頂点をすべて追加する関数クラス
	 *  @details    解にグラフの頂点をすべて追加する関数クラス
	 */
	class AddAllNodeOperator : public Operator
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		AddAllNodeOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~AddAllNodeOperator() = default;

		using random_device_type = typename Operator::random_device_type;
		using seed_type = typename Operator::seed_type;
		using size_type = typename Operator::size_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return ""; }

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		inline void operator()(typename __Solution::graph_type& __graph, __SearchInfo& __search_info, __Solution& __solution)
		{
			__solution.clear();
			for (const auto& _vertex : __graph.vertices()) {
				__solution.addNode(_vertex.id());
				__solution.addColor(_vertex.id());
			}
		}
		template<class __SearchInfo, class __Solution>
		inline void operator()(const typename __Solution::graph_type& __graph, const __SearchInfo& __search_info, __Solution& __solution, const __Solution& __initial_solution, const __Solution& __best_solution) {
			(*this)(__graph, __search_info, __solution);
		}
	};
} /* namespace okl */