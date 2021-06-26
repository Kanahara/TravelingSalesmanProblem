#pragma once

#include <vector>
#include <iostream>
#include <cstdio>
#include "../Graph.hpp"

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
		class __Vertices = std::vector<__Vertex>,
		class __AdjacentMatrix = SimpleMatrix<__Edge, std::uint_fast64_t>>
		class MatrixGraphBase : public Graph<__Vertex, __Edge, __Degree, __Vertices> {
		public:
			/*!
			 * @brief				コンストラクタ
			 */
			MatrixGraphBase() = default;

			/*!
			 * @brief				デストラクタ
			 */
			virtual ~MatrixGraphBase() = default;

			using id_type = typename Graph<__Vertex, __Edge, __Degree, __Vertices>::id_type;
			using vertex_type = typename Graph<__Vertex, __Edge, __Degree, __Vertices>::vertex_type;
			using edge_type = typename Graph<__Vertex, __Edge, __Degree, __Vertices>::edge_type;
			using degree_type = typename Graph<__Vertex, __Edge, __Degree, __Vertices>::degree_type;
			using size_type = typename Graph<__Vertex, __Edge, __Degree, __Vertices>::size_type;
			using edge_size_type = typename Graph<__Vertex, __Edge, __Degree, __Vertices>::edge_size_type;
			using vertices_container = typename Graph<__Vertex, __Edge, __Degree, __Vertices>::vertices_container;

			using has_adjacent = std::false_type;
			using has_non_adjacent = std::false_type;

			/*!
			 * @brief			全要素初期化
			 * @detail			全要素を初期化処理を行う
			 *					メモリ解放は行わない
			 */
			inline virtual void clear() override
			{
				Graph<__Vertex, __Edge, __Degree, __Vertices>::clear();
				this->adjacent_matrix_.clear();
			}

			/*!
			 * @brief			サイズ設定
			 * @param[in]		__size 頂点数
			 * @detail			全要素をメモリ確保を行う
			 */
			inline virtual void resize(const size_type __size) override
			{
				Graph<__Vertex, __Edge, __Degree, __Vertices>::resize(__size);
				this->adjacent_matrix_.resize(__size);
			}

			/*!
			 * @brief			辺数を返す
			 * @return			size_type	辺数を返す
			 * @detail			グラフ内の辺の数を返す
			 *					無向グラフだと辺数が半分になる
			 * 					updateInfo() を行っていない場合は結果は未定義
			 */
			inline edge_size_type edge_size() const override
			{
				if constexpr (__AdjacentMatrix::symmetric::value) {
					return this->edge_size_;
				}
				else {
					return Graph<__Vertex, __Edge, __Degree, __Vertices>::edge_size();
				}
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
				this->adjacent_matrix_.set(__start, __end, __edge);
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
			inline bool existsDirectedEdge(const id_type __start, const id_type __end) const override
			{
				return this->edge(__start, __end) != 0;
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
			inline bool existsUndirectedEdge(const id_type __start, const id_type __end) const override
			{
				return this->edge(__start, __end) == this->edge(__end, __start) && this->edge(__start, __end) != 0;
			}

			/*!
			 * @brief			辺を返す
			 * @return			edge_type	辺の重みを返す
			 * @param[in]		__start 辺開始頂点のID
			 * @param[in]		__end	辺終点頂点のID
			 * @detail			グラフにstart->end有向辺の重みを返す
			 * 					- 整数型id_typeはテンプレート__Vertexのid_typeによって定義される
			 */
			inline edge_type edge(const id_type __start, const id_type __end) const override
			{
				return this->adjacent_matrix_(__start, __end);
			}

			/*!
			 * @brief			次数を返す
			 * @return			degree_type	次数を返す
			 * @param[in]		__id 指定の頂点のID
			 * @detail			指定頂点のグラフ内の次数を返す
			 * 					updateInfo() を行っていない場合は結果は未定義
			 */
			inline degree_type degree(const id_type __id) const override
			{
				return static_cast<degree_type>(std::count_if(this->vertices().begin(), this->vertices().end(),
					[this, __id](const auto& _vertex) { return this->existsDirectedEdge(__id, _vertex.id()); }));
			}

		protected:
			//!辺行列
			__AdjacentMatrix adjacent_matrix_;
	};
}
/* namespace okl */