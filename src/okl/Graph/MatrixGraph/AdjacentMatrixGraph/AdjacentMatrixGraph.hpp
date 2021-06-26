#pragma once

#include <vector>
#include <iostream>
#include <cstdio>
#include "AdjacentMatrixGraphBase.hpp"

namespace okl {
	/*!
	 *  @brief      隣接行列 & 隣接配列グラフクラス
	 *  @details    隣接行列 & 隣接配列グラフクラス
	 * 				静的使用メイン	動的に更新して使用は想定していない
	 *				- __Vertex				頂点クラス
	 *				- __Edge				辺重み数値型
	 *				- __Degree				次数数値型
	 *				- __Vertices			頂点配列
	 *				- __AdjacentMatrix		隣接行列クラス
	 *				- __AdjacentContainer	隣接配列クラス
	 *				- __AdjacentContainers	隣接配列クラス配列
	 */
	template<
		typename __Vertex,
		typename __Edge,
		typename __Degree = okl::make_twice_byte_t<typename __Vertex::id_type>,
		class __Vertices = std::vector<__Vertex>,
		class __AdjacentMatrix = SimpleMatrix<__Edge, std::uint_fast64_t>,
		class __AdjacentContainer = std::vector<typename __Vertex::id_type>,
		class __AdjacentContainers = std::vector<__AdjacentContainer>>
		class AdjacentMatrixGraph : public AdjacentMatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix, __AdjacentContainer, __AdjacentContainers> {
		public:
			/*!
			 * @brief				コンストラクタ
			 */
			AdjacentMatrixGraph() = default;

			/*!
			 * @brief				デストラクタ
			 */
			virtual ~AdjacentMatrixGraph() = default;

			using id_type = typename AdjacentMatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix, __AdjacentContainer, __AdjacentContainers>::id_type;
			using vertex_type = typename AdjacentMatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix, __AdjacentContainer, __AdjacentContainers>::vertex_type;
			using edge_type = typename AdjacentMatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix, __AdjacentContainer, __AdjacentContainers>::edge_type;
			using degree_type = typename AdjacentMatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix, __AdjacentContainer, __AdjacentContainers>::degree_type;
			using size_type = typename AdjacentMatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix, __AdjacentContainer, __AdjacentContainers>::size_type;
			using edge_size_type = typename AdjacentMatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix, __AdjacentContainer, __AdjacentContainers>::edge_size_type;
			using vertices_container = typename AdjacentMatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix, __AdjacentContainer, __AdjacentContainers>::vertices_container;

			using has_adjacent = std::true_type;
			using has_non_adjacent = std::false_type;

			/*!
			 * @brief			サイズ設定
			 * @param[in]		__size 頂点数
			 * @detail			全要素をメモリ確保を行う
			 */
			inline virtual void resize(const size_type __size) override
			{
				AdjacentMatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix, __AdjacentContainer, __AdjacentContainers>::resize(__size);
				this->vertices_.resize(__size);
			}

			/*!
			 * @brief			グラフに所属する頂点を追加
			 * @param[in]		__vertex 頂点クラス
			 * @detail			グラフに含まれる頂点を追加する
			 * 					頂点クラスはテンプレート__Vertexによって定義される
			 */
			inline virtual void addVertex(const vertex_type& __vertex) override
			{
				this->vertices_[__vertex.id()] = __vertex;
			}

			/*!
			 * @brief			頂点を返す
			 * @return			vertex_type	頂点を返す
			 * @param[in]		__node_id 指定の頂点のID
			 * @detail			指定頂点の頂点インスタンスを返す
			 */
			inline virtual const vertex_type& vertex(const id_type __node_id) const override
			{
				return this->vertices_[__node_id];
			}

		private:
			/*!
			 * @brief			隣接頂点ID配列作成する
			 * @detail			隣接頂点ID配列作成を行う
			 */
			inline void makeAdjacents() override
			{
				for (size_type _start = 0; _start < this->vertices().size() - 1; ++_start) {
					const auto _start_id = std::next(this->vertices().begin(), _start)->id();
					for (size_type _end = _start + 1; _end < this->vertices().size(); ++_end) {
						const auto _end_id = std::next(this->vertices().begin(), _end)->id();
						if (this->existsDirectedEdge(_start_id, _end_id)) {
							this->adjacents_[_start_id].emplace_back(_end_id);
							this->adjacents_[_end_id].emplace_back(_start_id);
						}
					}
				}
				for (const auto& _vertex : this->vertices()) {
					this->adjacents_[_vertex.id()].shrink_to_fit();
				}
			}
	};
}
/* namespace okl */