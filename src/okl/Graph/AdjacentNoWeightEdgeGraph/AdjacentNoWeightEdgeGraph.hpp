#pragma once

#include <vector>
#include <iostream>
#include <cstdio>
#include <unordered_set>
#include "../Graph.hpp"
#include "../../HashSet/module.hpp"

namespace okl {
	/*!
	 *  @brief      辺重みなし隣接集合グラフクラス
	 *  @details    辺重みなし隣接集合グラフクラス
	 * 				静的使用メイン	動的に更新して使用は想定していない
	 *				- __Vertex				頂点クラス
	 *				- __Edge				辺重み数値型
	 *				- __Degree				次数数値型
	 *				- __Vertices			頂点配列
	 *				- __AdjacentContainer	隣接集合クラス
	 *				- __AdjacentContainers	隣接集合配列
	 */
	template<
		typename __Vertex,
		typename __Edge,
		typename __Degree = okl::make_twice_byte_t<typename __Vertex::id_type>,
		class __Vertices = okl::HashSet<__Vertex>,
		class __AdjacentContainer = std::unordered_set<typename __Vertex::id_type>,
		class __AdjacentContainers = std::vector<__AdjacentContainer>>
		class AdjacentNoWeightEdgeGraph : public Graph<__Vertex, __Edge, __Degree, __Vertices> {
		public:
			/*!
			 * @brief				コンストラクタ
			 */
			AdjacentNoWeightEdgeGraph() = default;

			/*!
			 * @brief				デストラクタ
			 */
			virtual ~AdjacentNoWeightEdgeGraph() = default;

			using id_type = typename Graph<__Vertex, __Edge, __Degree, __Vertices>::id_type;
			using vertex_type = typename Graph<__Vertex, __Edge, __Degree, __Vertices>::vertex_type;
			using edge_type = typename Graph<__Vertex, __Edge, __Degree, __Vertices>::edge_type;
			using degree_type = typename Graph<__Vertex, __Edge, __Degree, __Vertices>::degree_type;
			using size_type = typename Graph<__Vertex, __Edge, __Degree, __Vertices>::size_type;
			using edge_size_type = typename Graph<__Vertex, __Edge, __Degree, __Vertices>::edge_size_type;

			using has_adjacent = std::true_type;
			using has_non_adjacent = std::false_type;

			/*!
			 * @brief			全要素初期化
			 * @detail			全要素を初期化処理を行う
			 *					メモリ解放は行わない
			 */
			inline void clear() override
			{
				Graph<__Vertex, __Edge, __Degree, __Vertices>::clear();
				std::for_each(this->adjacents_.begin(), this->adjacents_.end(),
					[this](auto& _adjacent) {
						_adjacent.clear();
					}
				);
			}

			/*!
			 * @brief			サイズ設定
			 * @param[in]		__size 頂点数
			 * @detail			全要素をメモリ確保を行う
			 */
			inline void resize(const size_type __size) override
			{
				Graph<__Vertex, __Edge, __Degree, __Vertices>::resize(__size);
				this->vertices_.reserve(__size);
				this->adjacents_.resize(__size);
			}

			/*!
			 * @brief			グラフに所属する頂点を追加
			 * @param[in]		__vertex 頂点クラス
			 * @detail			グラフに含まれる頂点を追加する
			 * 					頂点クラスはテンプレート__Vertexによって定義される
			 */
			inline virtual void addVertex(const vertex_type& __vertex)
			{
				this->vertices_.insert(__vertex);
			}

			/*!
			 * @brief			グラフに所属する頂点を削除
			 * @param[in]		__node_id 削除頂点ID
			 * @detail			グラフに含まれる頂点を削除する
			 * @return			__Vertices::iterator	削除した頂点の次のイテレータ
			 * 					- 整数型id_typeはテンプレート__Vertexのid_typeによって定義される
			 */
			inline typename __Vertices::key_type eraseVertex(const id_type __node_id)
			{
				for (const auto _node_id : this->adjacent(__node_id)) {
					this->eraseDirectedEdge(_node_id, __node_id);
				}
				this->edge_size_ -= this->adjacent(__node_id).size();
				this->updateDensity();
				return this->vertices_.erase_key(__node_id);
			}

			/*!
			 * @brief			グラフに有向辺を設定
			 * @param[in]		__start 辺開始頂点のID
			 * @param[in]		__end	辺終点頂点のID
			 * @param[in]		__edge	辺の重み
			 * @detail			グラフにstart->end有向辺を追加する
			 * 					- 整数型id_typeはテンプレート__Vertexのid_typeによって定義される
			 * 					- 数値型edge_typeはテンプレート__Edgeによって定義される
			 */
			inline void addDirectedEdge(const id_type __start, const id_type __end, const edge_type __edge) override
			{
				Graph<__Vertex, __Edge, __Degree, __Vertices>::addDirectedEdge(__start, __end, __edge);
				if (__edge != 0) {
					this->adjacents_[__start].emplace(__end);
				}
			}

			/*!
			 * @brief			グラフの有向辺を削除
			 * @param[in]		__start 辺開始頂点のID
			 * @param[in]		__end	辺終点頂点のID
			 * @detail			グラフにstart->end有向辺を追加する
			 * 					- 整数型id_typeはテンプレート__Vertexのid_typeによって定義される
			 */
			inline void eraseDirectedEdge(const id_type __start, const id_type __end)
			{
				--this->edge_size_;
				this->adjacents_[__start].erase(__end);
			}

			/*!
			 * @brief			グラフの有向辺を削除
			 * @param[in]		__start 辺開始頂点のID
			 * @param[in]		__end	辺終点頂点のIDイテレータ
			 * @return			__AdjacentContainer::iterator	削除した頂点の次のイテレータ
			 * @detail			グラフにstart->end有向辺を追加する
			 * 					- 整数型id_typeはテンプレート__Vertexのid_typeによって定義される
			 */
			inline typename __AdjacentContainer::const_iterator eraseDirectedEdge(const id_type __start, typename __AdjacentContainer::const_iterator __end)
			{
				--this->edge_size_;
				return this->adjacents_[__start].erase(__end);
			}

			/*!
			 * @brief			グラフに無向辺を設定
			 * @param[in]		__start 頂点aのID
			 * @param[in]		__end	頂点bのID
			 * @param[in]		__edge	辺の重み
			 * @detail			グラフにstart<->end無向辺を追加する
			 * 					整数型id_typeはテンプレート__Vertexのid_typeによって定義される
			 * 					数値型edge_typeはテンプレート__Edgeによって定義される
			 */
			inline void addUndirectedEdge(const id_type __start, const id_type __end, const edge_type __edge)
			{
				this->addDirectedEdge(__start, __end, __edge);
				this->addDirectedEdge(__end, __start, __edge);
			}

			/*!
			 * @brief			有向辺が存在するか判定
			 * @param[in]		__start 辺開始頂点のID
			 * @param[in]		__end	辺終点頂点のID
			 * @return			bool	有向辺が存在するならTrue　存在しないならFalse
			 * @detail			グラフにstart->end有向辺の存在を判定する
			 * 					start->end有向辺が重み0でないなら存在するという判定を行う
			 * 					- 整数型id_typeはテンプレート__Vertexのid_typeによって定義される
			 */
			bool existsDirectedEdge(const id_type __start, const id_type __end) const override
			{
				return this->adjacent(__start).find(__end) != this->adjacent(__start).end();
			}

			/*!
			 * @brief			無向辺が存在するか判定
			 * @param[in]		__start 頂点aのID
			 * @param[in]		__end	頂点bのID
			 * @return			bool	無向辺が存在するならTrue　存在しないならFalse
			 * @detail			グラフにstart<->end無有向辺の存在を判定する
			 * 					start->end無向辺が重み0でないならstart->end start<-endの重さが同じかどうか
			 * 					- 整数型id_typeはテンプレート__Vertexのid_typeによって定義される
			 */
			bool existsUndirectedEdge(const id_type __start, const id_type __end) const override
			{
				return this->existsDirectedEdge(__start, __end) && this->existsDirectedEdge(__end, __start);
			}

			/*!
			 * @brief			辺を返す
			 * @return			edge_type	辺の重み(0 or 1)を返す
			 * @param[in]		__start 辺開始頂点のID
			 * @param[in]		__end	辺終点頂点のID
			 * @detail			グラフにstart->end有向辺の重みを返す
			 * 					- 整数型id_typeはテンプレート__Vertexのid_typeによって定義される
			 */
			edge_type edge(const id_type __start, const id_type __end) const override
			{
				return this->adjacent(__start).find(__end) != this->adjacent(__start).end();
			}

			/*!
			 * @brief			次数を返す
			 * @return			degree_type	次数を返す
			 * @param[in]		__id 指定の頂点のID
			 * @detail			指定頂点のグラフ内の次数を返す
			 * 					updateInfo() を行っていない場合は結果は未定義
			 */
			degree_type degree(const id_type __id) const override
			{
				return static_cast<degree_type>(this->adjacents_[__id].size());
			}

			/*!
			 * @brief			頂点を返す
			 * @return			vertex_type	頂点を返す
			 * @param[in]		__node_id 指定の頂点のID
			 * @detail			指定頂点の頂点インスタンスを返す
			 */
			inline const vertex_type& vertex(const id_type __node_id) const override
			{
				return *(this->vertices().find_key(__node_id));
			}

			/*!
			 * @brief			隣接頂点ID配列を返す
			 * @param[in]		__id 指定の頂点のID
			 * @return			__AdjacentContainer&		隣接頂点ID配列を返す
			 * @detail			指定頂点に隣接している頂点IDの集合の参照を返す
			 *					ただし、指定頂点自身は含まれない
			 */
			inline const __AdjacentContainer& adjacent(const id_type __id) const
			{
				return this->adjacents_[__id];
			}

		private:
			//!隣接配列
			__AdjacentContainers adjacents_;

			/*!
			 * @brief			無向グラフの判定フラグの更新を行う
			 */
			inline void updateUndirectedDecision() override
			{
				this->is_undirected_ = true;
				for (const auto& _vertex : this->vertices()) {
					const auto _start_id = _vertex.id();
					for (const auto _end_id : this->adjacent(_start_id)) {
						if (!this->existsDirectedEdge(_end_id, _start_id)) {
							this->is_undirected_ = false;
							break;
						}
					}
					if (this->is_undirected_ == false) {
						break;
					}
				}
			}
	};
}
/* namespace okl */