#pragma once

#include "../../../Operator/module.hpp"

namespace okl::tsp {
	/*!
	 *  @brief      最近傍ルート順序設定関数基底クラス
	 *  @details    最近傍ルート順序設定関数基底クラス
	 *				- __Set				選択頂点を一時格納する集合クラス　
	 */
	template<class __Set>
	class NearestNeighborRouteOrderOperator : public RouteOrderOperator
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		NearestNeighborRouteOrderOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~NearestNeighborRouteOrderOperator() = default;

		using random_device_type = typename RouteOrderOperator::random_device_type;
		using seed_type = typename RouteOrderOperator::seed_type;
		using size_type = typename RouteOrderOperator::size_type;

		using set_type = __Set;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return  "最近傍" + RouteOrderOperator::info(); }

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 *					メモリ解放は行わない
		 */
		inline virtual void clear() override
		{
			RouteOrderOperator::clear();
			this->nodes_.clear();
		}

		/*!
		 * @brief			サイズ設定
		 * @param[in]		__size 頂点数
		 * @detail			全要素をメモリ確保を行う
		 */
		inline virtual void resize(const size_type __size) override
		{
			RouteOrderOperator::resize(__size);
			assert(__size <= std::numeric_limits<typename __Set::size_type>::max());
			this->nodes_.reserve(static_cast<typename __Set::size_type>(__size));
		}

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		inline void operator()(const typename __Solution::graph_type& __graph, const __SearchInfo& __search_info, __Solution& __solution, const __Solution& __initial_solution, const __Solution& __best_solution) {
			__solution = __initial_solution;
			this->nodes_ = __solution.nodes();

			auto _select_node_id = this->random_device_.random_element(this->nodes_);
			this->nodes_.erase(_select_node_id);
			__solution.setRouteOrder(__graph, _select_node_id);

			this->nodes_.shuffle(this->random_device_.engin());

			while (!this->nodes_.isEmpty()) {
				_select_node_id = *std::min_element(this->nodes_.begin(), this->nodes_.end(),
					[&__graph, _select_node_id](const auto _node_id_a, const auto _node_id_b) {return __graph.edge(_select_node_id, _node_id_a) < __graph.edge(_select_node_id, _node_id_b); }
				);
				this->nodes_.erase(_select_node_id);
				__solution.setRouteOrder(__graph, _select_node_id);
			}

			__solution.calculateCost(__graph);
		}

	private:
		//一時頂点ID集合
		set_type nodes_;
	};
} /* namespace okl::tsp */