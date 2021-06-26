#pragma once

#include <vector>
#include <iostream>
#include <cstdio>
#include "MatrixGraphBase.hpp"
#include "../../HashSet/module.hpp"

namespace okl {
	/*!
	 *  @brief      隣接行列グラフクラス
	 *  @details    隣接行列グラフクラス
	 * 				静的使用メイン	動的に更新して使用は想定していない
	 *				- __Vertex				頂点クラス
	 *				- __Edge				辺重み数値型
	 *				- __Degree				次数数値型
	 *				- __Vertices			頂点配列
	 *				- __AdjacentMatrix		隣接行列クラス
	 */
	template<
		typename __Vertex,
		typename __Edge,
		typename __Degree = okl::make_twice_byte_t<typename __Vertex::id_type>,
		class __Vertices = okl::HashSet<__Vertex>,
		class __AdjacentMatrix = SimpleMatrix<__Edge, std::uint_fast64_t>>
		class MatrixGraphwWithErase : public MatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix> {
		public:
			/*!
			 * @brief				コンストラクタ
			 */
			MatrixGraphwWithErase() = default;

			/*!
			 * @brief				デストラクタ
			 */
			virtual ~MatrixGraphwWithErase() = default;

			using id_type = typename MatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix>::id_type;
			using vertex_type = typename MatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix>::vertex_type;
			using edge_type = typename MatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix>::edge_type;
			using degree_type = typename MatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix>::degree_type;
			using size_type = typename MatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix>::size_type;
			using edge_size_type = typename MatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix>::edge_size_type;
			using vertices_container = typename MatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix>::vertices_container;

			using has_adjacent = std::false_type;
			using has_non_adjacent = std::false_type;

			/*!
			 * @brief			サイズ設定
			 * @param[in]		__size 頂点数
			 * @detail			全要素をメモリ確保を行う
			 */
			inline void resize(const size_type __size) override
			{
				MatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix>::resize(__size);
				this->vertices_.reserve(__size);
			}

			/*!
			 * @brief			グラフに所属する頂点を追加
			 * @param[in]		__vertex 頂点クラス
			 * @detail			グラフに含まれる頂点を追加する
			 * 					頂点クラスはテンプレート__Vertexによって定義される
			 */
			inline void addVertex(const vertex_type& __vertex) override
			{
				this->vertices_.insert(__vertex);
			}

			/*!
			 * @brief			グラフに所属する頂点を削除
			 * @param[in]		__node_id 指定の頂点のID
			 * @detail			グラフに含まれる指定頂点IDの頂点を削除する
			 * 					id_typeはテンプレート__Vertex;;id_typeによって定義される
			 */
			inline typename vertices_container::const_iterator eraseVertex(const id_type __node_id)
			{
				for (const auto& _vertex : this->vertices()) {
					if (this->existsDsirectedEdge(_vertex.id(), __node_id)) {
						this->eraseDirectedEdge(_vertex.id(), __node_id);
					}
				}
				return this->vertices_.erase_key(__node_id);
			}

			/*!
			 * @brief			頂点を返す
			 * @return			vertex_type	頂点を返す
			 * @param[in]		__node_id 指定の頂点のID
			 * @detail			指定頂点の頂点インスタンスを返す
			 */
			inline const vertex_type& vertex(const id_type __node_id) const override
			{
				return *(this->vertices_.find_key(__node_id));
			}

			/*!
			 * @brief			グラフに有向辺を削除
			 * @param[in]		__start 辺開始頂点のID
			 * @param[in]		__end	辺終点頂点のID
			 * @detail			グラフにstart->end有向辺を作成する
			 * 					- 整数型id_typeはテンプレート__Vertexのid_typeによって定義される
			 */
			inline void eraseDirectedEdge(const id_type __start, const id_type __end)
			{
				--this->edge_size_;
				this->adjacent_matrix_.set(__start, __end, 0);
			}

			/*!
			 * @brief			グラフに無向辺を削除
			 * @param[in]		__start 辺開始頂点のID
			 * @param[in]		__end	辺終点頂点のID
			 * @detail			グラフにstart<->end無向辺を作成する
			 * 					- 整数型id_typeはテンプレート__Vertexのid_typeによって定義される
			 */
			inline void eraseUndirectedEdge(const id_type __start, const id_type __end)
			{
				this->eraseDirectedEdge(__start, __end);
				this->eraseDirectedEdge(__end, __start);
			}
	};
}
/* namespace okl */