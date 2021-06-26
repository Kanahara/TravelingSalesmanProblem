#pragma once
#include <vector>
#include <limits>
#include "../../HashSet/module.hpp"

namespace okl::tsp {
	/*!
	 *  @brief      巡回セールスマン問題に対する解クラス
	 *  @details    巡回セールスマン問題に対する解クラス
	 *				- __Graph				グラフクラス
	 *				- __Cost				コスト数値型
	 *				- __IdContainer			IDメディアンクラス
	 */
	template<
		class __Graph,
		class __Cost,
		class __Set = HashSet<typename __Graph::id_type>,
		class __RouteContainer = std::vector<typename __Graph::id_type>>
		class Solution
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		Solution() : cost_(std::numeric_limits<cost_type>::max()), order_number_(0) {};

		/*!
			 * @brief				デストラクタ
		 */
		virtual ~Solution() = default;

		using graph_type = __Graph;

		using vertex_type = typename __Graph::id_type;
		using node_type = vertex_type;
		using degree_type = typename __Graph::degree_type;
		using edge_type = typename __Graph::edge_type;

		using size_type = typename __Graph::size_type;
		using index_type = std::size_t;

		using id_type = typename __Graph::id_type;

		using cost_type = __Cost;
		using route_container_type = __RouteContainer;
		using set_type = __Set;
		using cost_comparison_function = std::greater<cost_type>;

		/*!
		 * @brief			解の値を返す
		 * @return			cost_type	解の値を返す
		 * @detail			解の値としてクリークのサイズを返す
		 * 					- 数値型cost_typeはテンプレート__Costによって定義される
		 */
		constexpr cost_type cost() const
		{
			return this->cost_;
		}

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 *					メモリ解放は行わない
		 */
		inline void clear()
		{
			this->cost_ = std::numeric_limits<cost_type>::max();
			this->order_number_ = 0;
			this->nodes_.clear();
			std::fill(this->route_.begin(), this->route_.end(), this->undefine_id());
		}

		/*!
		 * @brief			サイズ設定
		 * @param[in]		__size 頂点数
		 * @detail			全要素をメモリ確保を行う
		 */
		inline void resize(const size_type __size)
		{
			this->route_.resize(__size, this->undefine_id());
			this->nodes_.reserve(__size);
		}

		/*!
		 * @brief			制約条件を満たすか判定
		 * @return			bool	制約を満たすならTrue　悪くないならFalse
		 * @detail			容量超過メディアン集合が空ならTrue
		 */
		constexpr bool meetConstraints() const
		{
			return this->order_number_ == this->nodes().size();
		}

		/*!
		 * @brief			探索頂点追加する
		 * @param[in]		__graph グラフクラス
		 * @param[in]		__node_id 探索頂点として追加する頂点ID
		 * @detail			閉鎖メディアン集合に探索頂点追加する
		 */
		inline void addNode(const node_type __node_id)
		{
			this->nodes_.insert(__node_id);
		}

		/*!
		 * @brief			ルート順序を設定する
		 * @param[in]		__graph グラフクラス
		 * @param[in]		__node_id  巡回するノードID
		 * @detail			解生成時のみに行う　__node_idを最後に巡回するように設定する
		 */
		inline void setRouteOrder(const __Graph& __graph, const id_type __node_id)
		{
			assert(this->order_number_ < this->route_.size());
			this->route_[this->order_number_] = __node_id;
			++this->order_number_;
		}

		/*!
		 * @brief			ルート順序を設定する
		 * @param[in]		__graph グラフクラス
		 * @param[in]		__node_id  巡回するノードID
		 * @detail			解生成時のみに行う　__node_idを最後に巡回するように設定する
		 */
		inline void calculateCost (const __Graph& __graph)
		{
			const std::size_t _last_index = this->route().size() - 1;
			this->cost_ = __graph.edge(this->route()[0], this->route()[_last_index]);
			for (std::size_t _index = 0; _index < _last_index; ++_index) {
				this->cost_ += __graph.edge(this->route()[_index], this->route()[_index + 1]);
			}
		}

		/*!
		 * @brief			ルートスワップ
		 * @param[in]		__graph グラフクラス
		 * @param[in]		__index_a 交換するルートインデックスA
		 * @param[in]		__index_b 交換するルートインデックスB
		 * @param[in]		__gain_cost 交換した場合のゲインコスト
		 * @detail			解生成時のみに行う
		 */
		inline void routeSwap(const __Graph& __graph, index_type __index_a, index_type __index_b, const cost_type __gain_cost)
		{
			if (__index_b < __index_a) {
				std::swap(__index_a, __index_b);
			}

			this->cost_ += __gain_cost;
			const auto _inner_range_size = (__index_b - __index_a + 1) / 2;
			const auto _outer_range_size = (__index_a - __index_b - 1 +  this->route().size()) / 2;
			if (_inner_range_size < _outer_range_size) {
				for (index_type _order = 0; _order < _inner_range_size; ++_order)
				{
					std::swap(this->route_[__index_a + _order], this->route_[__index_b - _order]);
				}
			}
			else {
				for (index_type _order = 1; _order <= _outer_range_size; ++_order)
				{
					index_type __index_aa = __index_a;
					if (__index_a < _order) {
						__index_aa += this->route().size();
					}
					__index_aa -= _order;

					index_type __index_bb = __index_b + _order;
					if (this->route_.size() <= __index_bb) {
						__index_bb -= this->route().size();
					}

					std::swap(this->route_[__index_aa], this->route_[__index_bb]);
				}
			}
		}

		/*!
		 * @brief			ルートスワップ
		 * @param[in]		__graph グラフクラス
		 * @param[in]		__index_a 交換するルートインデックスA
		 * @param[in]		__index_b 交換するルートインデックスB
		 * @detail			解生成時のみに行う
		 */
		inline void routeSwap(const __Graph& __graph, const index_type __index_a, const index_type __index_b)
		{
			this->routeSwap(__graph, __index_a, __index_b, this->gaincostAfterNodeSwap(__graph, __index_a, __index_b));
		}

		/*!
		 * @brief			ノードスワップを行った場合のコスト差
		 * @param[in]		__graph グラフクラス
		 * @param[in]		__node_id_a 交換するノードID
		 * @param[in]		__node_id_b 交換するノードID
		 * @return			cost_type		コスト差を返す
		 * @detail			ノードスワップを行った場合のコスト差を返す
		 *					ノードスワップそのものは行わない
		 */
		constexpr cost_type gaincostAfterRouteSwap(const __Graph& __graph, index_type __index_a, index_type __index_b)
		{
			if (__index_b < __index_a) {
				std::swap(__index_a, __index_b);
			}

			const index_type _last_index = this->route().size() - 1;

			index_type __prev_index_a;
			if (__index_a == 0) {
				__prev_index_a = _last_index;
			}
			else {
				__prev_index_a = __index_a - 1;
			}

			index_type __next_index_b;
			if (__index_b == _last_index) {
				__next_index_b = 0;
			}
			else {
				__next_index_b = __index_b + 1;
			}

			return (__graph.edge(this->route_[__prev_index_a], this->route_[__index_b]) + __graph.edge(this->route_[__index_a], this->route_[__next_index_b]))
				-(__graph.edge(this->route_[__prev_index_a], this->route_[__index_a]) + __graph.edge(this->route_[__index_b], this->route_[__next_index_b]));
		}

		/*!
		 * @brief			ノード集合の参照を返す
		 * @return			set_type&		ノードID集合の参照を返す
		 * @detail			ノード集合 の参照を返す
		 *					set_typeはテンプレート__Setにより定義される
		 */
		inline const set_type& nodes() const
		{
			return this->nodes_;
		}

		/*!
		 * @brief			ノード集合をシャッフルする
		 * @param[in]		__generator ランダム生成機
		 * @detail			要素をランダム生成機を使用しシャッフルする
		 */
		template<typename _UniformRandomNumberGenerator>
		inline void shuffleNodes(_UniformRandomNumberGenerator& __generator)
		{
			this->nodes_.shuffle(__generator);
		}

		/*!
		 * @brief			開設メディアン集合をソートする
		 * @param[in]		__graph グラフクラス
		 * @param[in]		__node_id 指定ノードID
		 * @detail			開設メディアン集合をソートを指定の近い順する
		 */
		inline void sortOpeningMedians(const __Graph& __graph, const node_type __node_id)
		{
			this->nodes_.sort([&__graph, __node_id](const id_type& __left_vertex, const id_type& __right_vertex) {return __graph.edge(__node_id, __left_vertex) < __graph.edge(__node_id, __right_vertex); });
		}

		/*!
		 * @brief			ルート配列の参照を返す
		 * @return			route_container_type&		ルート配列の参照を返す
		 * @detail			ルート配列 の参照を返す
		 *					route_container_typeはテンプレート__Setにより定義される
		 */
		inline const route_container_type& route() const
		{
			return this->route_;
		}

		void check(const __Graph& __graph) {
			const std::size_t _last_index = this->route().size() - 1;
			cost_type ccc = __graph.edge(this->route()[0], this->route()[_last_index]);
			for (std::size_t _index = 0; _index < _last_index; ++_index) {
				ccc += __graph.edge(this->route()[_index], this->route()[_index + 1]);
			}
			auto _route = this->route_;
			std::sort(_route.begin(), _route.end());
			for (std::size_t _i = 0; _i < _route.size(); ++_i) {
				if (_route[_i] != _i) {
					std::cout << "ERROE";
					char c; std::cin >> c;
				}
			}
			if (ccc != this->cost_) {
				std::cout << "ERROE";
				char c; std::cin >> c;
			}
		}

		/*!
		* @brief			代入処理を行う
		* @param[in]		__other 代入元対象
		* @return			Solution&	自分自身の参照を返す
		* @detail
		*/
		inline Solution& operator=(const Solution& __other)
		{
			this->cost_ = __other.cost_;
			this->order_number_ = __other.order_number_;
			this->route_ = __other.route_;
			if (this->nodes_.size() != __other.nodes_.size()) {
				this->nodes_ = __other.nodes_;
			}
			return *this;
		}

		//比較演算子
		constexpr bool operator<(const Solution& __other) const
		{
			return cost_comparison_function()(this->cost(), __other.cost());
		}
		constexpr bool operator>(const Solution& __other) const
		{
			return __other < *this;
		}
		constexpr bool operator<=(const Solution& __other) const
		{
			return !(*this > __other);
		}
		constexpr bool operator>=(const Solution& __other) const
		{
			return !(*this < __other);
		}
		constexpr bool operator==(const Solution& __other) const
		{
			return true;
		}
		constexpr bool operator!=(const Solution& __other) const
		{
			return !(*this == __other);
		}

	protected:
		/*!
		 * @brief			巡回路のコストを計算する
		 * @param[in]		__graph グラフクラス
		 * @detail			巡回路のコストを計算する
		 */
		constexpr cost_type calculateRouteCost(const __Graph& __graph) const
		{
			return 0;
		}

		/*!
		 * @brief			未定義IDを返す
		 * @return			id_type		未定義IDを返す
		 * @detail			未定義ID値としてstd::numeric_limits<id_type>::max()を返す
		 */
		constexpr id_type undefine_id() const
		{
			return std::numeric_limits<id_type>::max();
		}

	private:
		//!コスト
		cost_type cost_;

		//!巡回数
		size_type order_number_;

		//!素集合データセットのIDに対する開設しているメディアン
		route_container_type route_;

		//!ノードID集合
		__Set nodes_;
	};
} /* namespace okl::tsp */

#include "operator.hpp"