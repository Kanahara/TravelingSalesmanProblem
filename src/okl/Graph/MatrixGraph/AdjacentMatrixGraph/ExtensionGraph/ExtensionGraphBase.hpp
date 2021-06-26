#pragma once

#include <vector>
#include <iostream>
#include <cstdio>
#include "../AdjacentMatrixGraphBase.hpp"

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
		class ExtensionGraphBase : public AdjacentMatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix, __AdjacentContainer, __AdjacentContainers> {
		public:
			/*!
			 * @brief				コンストラクタ
			 */
			ExtensionGraphBase() = default;

			/*!
			 * @brief				デストラクタ
			 */
			virtual ~ExtensionGraphBase() = default;

			using id_type = typename AdjacentMatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix, __AdjacentContainer, __AdjacentContainers>::id_type;
			using vertex_type = typename AdjacentMatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix, __AdjacentContainer, __AdjacentContainers>::vertex_type;
			using edge_type = typename AdjacentMatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix, __AdjacentContainer, __AdjacentContainers>::edge_type;
			using degree_type = typename AdjacentMatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix, __AdjacentContainer, __AdjacentContainers>::degree_type;
			using size_type = typename AdjacentMatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix, __AdjacentContainer, __AdjacentContainers>::size_type;
			using edge_size_type = typename AdjacentMatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix, __AdjacentContainer, __AdjacentContainers>::edge_size_type;
			using vertices_container = typename AdjacentMatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix, __AdjacentContainer, __AdjacentContainers>::vertices_container;

			using has_adjacent = std::true_type;
			using has_non_adjacent = std::true_type;

			/*!
			 * @brief			全要素初期化
			 * @detail			全要素を初期化処理を行う
			 *					メモリ解放は行わない
			 */
			inline void clear() override
			{
				AdjacentMatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix, __AdjacentContainer, __AdjacentContainers>::clear();
				std::for_each(this->non_adjacents_.begin(), this->non_adjacents_.end(),
					[this](auto& _non_adjacent) {
						_non_adjacent.clear();
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
				AdjacentMatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix, __AdjacentContainer, __AdjacentContainers>::resize(__size);
				this->non_adjacents_.resize(__size);
			}

			/*!
			 * @brief			非隣接頂点ID配列を返す
			 * @param[in]		__id 指定の頂点のID
			 * @return			__AdjacentContainer&		非隣接頂点ID配列を返す
			 * @detail			指定頂点に隣接してない頂点IDの集合の参照を返す
			 *					ただし、指定頂点自身は含まれない
			 */
			inline const __AdjacentContainer& non_adjacent(const id_type __id) const
			{
				return this->non_adjacents_[__id];
			}

		protected:
			/*!
			 * @brief			隣接頂点配列・非隣接頂点集合ソート
			 * @detail			隣接頂点配列・非隣接頂点集合の
			 *					ソートを行う
			 *					隣接頂点配列・非隣接頂点配列は近い順
			 *					c++の標準ソート（イントロソート）を行う
			 */
			inline void sort() override
			{
				AdjacentMatrixGraphBase<__Vertex, __Edge, __Degree, __Vertices, __AdjacentMatrix, __AdjacentContainer, __AdjacentContainers>::sort();

				for (id_type _id = 0; _id < this->non_adjacents_.size(); ++_id) {
					std::sort(this->non_adjacents_[_id].begin(), this->non_adjacents_[_id].end(), [this, _id](const id_type& __left_vertex, const id_type& __right_vertex) {return this->edge(_id, __left_vertex) < this->edge(_id, __right_vertex); });
				}
			}

		protected:
			//!隣接配列
			__AdjacentContainers non_adjacents_;

	};
} /* namespace okl */