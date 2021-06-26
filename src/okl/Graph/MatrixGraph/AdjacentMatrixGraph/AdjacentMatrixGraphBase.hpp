#pragma once

#include <vector>
#include <iostream>
#include <cstdio>
#include "../MatrixGraphBase.hpp"

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
		class AdjacentMatrixGraphBase : public MatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix> {
		public:
			/*!
			 * @brief				コンストラクタ
			 */
			AdjacentMatrixGraphBase() = default;

			/*!
			 * @brief				デストラクタ
			 */
			virtual ~AdjacentMatrixGraphBase() = default;

			using id_type = typename MatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix>::id_type;
			using vertex_type = typename MatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix>::vertex_type;
			using edge_type = typename MatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix>::edge_type;
			using degree_type = typename MatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix>::degree_type;
			using size_type = typename MatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix>::size_type;
			using edge_size_type = typename MatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix>::edge_size_type;
			using vertices_container = typename MatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix>::vertices_container;

			using has_adjacent = std::true_type;
			using has_non_adjacent = std::false_type;

			/*!
			 * @brief			全要素初期化
			 * @detail			全要素を初期化処理を行う
			 *					メモリ解放は行わない
			 */
			inline virtual void clear() override
			{
				MatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix>::clear();
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
			inline virtual void resize(const size_type __size) override
			{
				MatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix>::resize(__size);
				this->adjacents_.resize(__size);
			}

			/*!
			 * @brief		各種情報更新
			 * @detail		辺密度 updateDensity()
			 *				無向グラフ判定 updateUndirectedDecision()
			 *				隣接頂点ID配列の作成 makeAdjacents()
			 *				をそれぞれ更新を行う
			 */
			inline virtual void updateInfo() override
			{
				MatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix>::updateInfo();
				this->makeAdjacents();
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
				return static_cast<degree_type>(this->adjacent(__id).size());
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

		protected:
			/*!
			 * @brief			隣接頂点配列ソート
			 * @detail			隣接頂点配列の
			 *					ソートを行う
			 *					隣接頂点配列は近い順
			 *					c++の標準ソート（イントロソート）を行う
			 */
			inline virtual void sort() override
			{
				MatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix>::sort();
				for (id_type _id = 0; _id < this->adjacents_.size(); ++_id) {
					std::sort(this->adjacents_[_id].begin(), this->adjacents_[_id].end(), [this, _id](const id_type& __left_vertex, const id_type& __right_vertex) {return this->edge(_id, __left_vertex) < this->edge(_id, __right_vertex); });
				}
			}

		protected:
			//!隣接配列
			__AdjacentContainers adjacents_;

			/*!
			 * @brief			隣接頂点ID配列作成する
			 * @detail			隣接頂点ID配列作成を行う
			 */
			inline virtual void makeAdjacents() = 0;

	};
}
/* namespace okl */