#pragma once

#include <vector>
#include <iostream>
#include <cstdio>
#include "../Utility/module.hpp"
#include "../Matrix/module.hpp"
#include "../HashSet/module.hpp"

namespace okl {
	/*!
	 *  @brief      グラフ基底クラス
	 *  @details    グラフ基底クラス
	 * 				静的使用メイン	動的に更新して使用は想定していない
	 *				- __Vertex				頂点クラス
	 *				- __Edge				辺重み数値型
	 *				- __Degree				次数数値型
	 *				- __Vertices			頂点配列
	 */
	template<
		typename __Vertex,
		typename __Edge,
		typename __Degree = okl::make_twice_byte_t<typename __Vertex::id_type>,
		class __Vertices = std::vector<__Vertex>>
		class Graph {
		public:
			/*!
			 * @brief				コンストラクタ
			 */
			Graph() : edge_size_(0), is_undirected_(false), density_(0.0) {};

			/*!
			 * @brief				デストラクタ
			 */
			virtual ~Graph() = default;

			using id_type = typename __Vertex::id_type;
			using vertex_type = __Vertex;
			using edge_type = __Edge;
			using degree_type = __Degree;
			using size_type = id_type;
			using edge_size_type = std::size_t;
			using vertices_container = __Vertices;

			using has_adjacent = std::false_type;
			using has_non_adjacent = std::false_type;

			/*!
			 * @brief			全要素初期化
			 * @detail			全要素を初期化処理を行う
			 *					メモリ解放は行わない
			 */
			inline virtual void clear()
			{
				this->edge_size_ = 0;
				this->is_undirected_ = false;
				this->density_ = 0.0;
				this->vertices_.clear();
			}

			/*!
			 * @brief			サイズ設定
			 * @param[in]		__size 頂点数
			 * @detail			全要素をメモリ確保を行う
			 */
			inline virtual void resize(const size_type __size)
			{
			}

			/*!
			 * @brief			グラフに所属する頂点を追加
			 * @param[in]		__vertex 頂点クラス
			 * @detail			グラフに含まれる頂点を追加する
			 * 					頂点クラスはテンプレート__Vertexによって定義される
			 */
			inline virtual void addVertex(const vertex_type& __vertex) = 0;

			/*!
			 * @brief			グラフに有向辺を設定
			 * @param[in]		__start 辺開始頂点のID
			 * @param[in]		__end	辺終点頂点のID
			 * @param[in]		__edge	辺の重み
			 * @detail			グラフにstart->end有向辺を追加する
			 * 					- 整数型id_typeはテンプレート__Vertexのid_typeによって定義される
			 * 					- 数値型edge_typeはテンプレート__Edgeによって定義される
			 */
			inline virtual void addDirectedEdge(const id_type __start, const id_type __end, const edge_type __edge)
			{
				if (!this->existsDirectedEdge(__start, __end) && __start != __end) {
					++this->edge_size_;
				}
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
			virtual bool existsDirectedEdge(const id_type __start, const id_type __end) const = 0;

			/*!
			 * @brief			無向辺が存在するか判定
			 * @param[in]		__start 頂点aのID
			 * @param[in]		__end	頂点bのID
			 * @return			bool	無向辺が存在するならTrue　存在しないならFalse
			 * @detail			グラフにstart<->end無有向辺の存在を判定する
			 * 					start->end無向辺が重み0でないなら存在するかどうかundirected_edge_decision_matrix_を参照する
			 * 					- 整数型id_typeはテンプレート__Vertexのid_typeによって定義される
			 */
			virtual bool existsUndirectedEdge(const id_type __start, const id_type __end) const = 0;

			/*!
			 * @brief		各種情報更新
			 * @detail		辺密度 updateDensity()
			 *				無向グラフ判定 updateUndirectedDecision()
			 *				をそれぞれ更新を行う
			 */
			inline virtual void updateInfo()
			{
				this->sort();
				this->updateUndirectedDecision();
				this->updateDensity();
			}

			/*!
			 * @brief			無向グラフか判定
			 * @return			bool	無向グラフならTrue　無向グラフならFalse
			 * @detail			このグラフが無向グラフかどうか判定を行う
			 * 					updateInfo() を行っていない場合は結果は未定義
			 */
			constexpr bool isUndirected() const
			{
				return this->is_undirected_;
			}

			/*!
			 * @brief			辺を返す
			 * @return			edge_type	辺の重みを返す
			 * @param[in]		__start 辺開始頂点のID
			 * @param[in]		__end	辺終点頂点のID
			 * @detail			グラフにstart->end有向辺の重みを返す
			 * 					- 整数型id_typeはテンプレート__Vertexのid_typeによって定義される
			 */
			virtual edge_type edge(const id_type __start, const id_type __end) const = 0;

			/*!
			 * @brief			次数を返す
			 * @return			degree_type	次数を返す
			 * @param[in]		__id 指定の頂点のID
			 * @detail			指定頂点のグラフ内の次数を返す
			 * 					updateInfo() を行っていない場合は結果は未定義
			 */
			virtual degree_type degree(const id_type __id) const = 0;

			/*!
			 * @brief			辺数を返す
			 * @return			size_type	辺数を返す
			 * @detail			グラフ内の辺の数を返す
			 *					無向グラフだと辺数が半分になる
			 * 					updateInfo() を行っていない場合は結果は未定義
			 */
			inline virtual edge_size_type edge_size() const
			{
				if (this->isUndirected()) {
					return this->edge_size_ / 2;
				}
				else {
					return this->edge_size_;
				}
			}

			/*!
			 * @brief			辺密度を返す
			 * @return			double	辺密度を返す
			 * @detail			グラフの辺密度
			 * 					updateInfo() を行っていない場合は結果は未定義
			 */
			constexpr double density() const
			{
				return this->density_;
			}

			/*!
			 * @brief			頂点を返す
			 * @return			vertex_type	頂点を返す
			 * @param[in]		__node_id 指定の頂点のID
			 * @detail			指定頂点の頂点インスタンスを返す
			 */
			inline virtual const vertex_type& vertex(const id_type __node_id) const = 0;

			/*!
			 * @brief			頂点配列を返す
			 * @return			__Vertices	頂点配列を返す
			 * @detail			頂点配列インスタンスを返す
			 */
			inline const __Vertices& vertices() const
			{
				return this->vertices_;
			}

		protected:
			//!辺の数
			edge_size_type edge_size_;

			//!無向グラフの判定フラグ
			bool is_undirected_;

			//!辺密度
			double density_;

			//!頂点配列
			__Vertices vertices_;

			/*!
			 * @brief			ソート関数
			 * @detail			なにもしない
			 *					c++の標準ソート（イントロソート）を行う
			 */
			inline virtual void sort()
			{
				//std::sort(this->vertices_.begin(), this->vertices_.end());
			}

			/*!
			 * @brief			辺密度の更新を行う
			 * @detail			辺密度 ＝ 辺数 / ( 頂点数 * （頂点数 - 1) )
			 */
			inline void updateDensity()
			{
				const double _vertex_size = static_cast<double>(this->vertices().size());

				this->density_ = static_cast<double>(this->edge_size());
				this->density_ /= _vertex_size;
				this->density_ /= (_vertex_size - 1.0);
				if (this->isUndirected()) {
					this->density_ *= 2.0;
				}
			}

			/*!
			 * @brief			無向グラフの判定フラグの更新を行う
			 */
			inline virtual void updateUndirectedDecision()
			{
				this->is_undirected_ = true;
				for (id_type _start = 0; _start < this->vertices().size() - 1; ++_start) {
					const auto _start_id = std::next(this->vertices().begin(), _start)->id();
					for (id_type _end = static_cast<id_type>(_start + 1); _end < this->vertices().size(); ++_end) {
						const auto _end_id = std::next(this->vertices().begin(), _end)->id();
						if (!this->existsUndirectedEdge(_start_id, _end_id) && (this->existsDirectedEdge(_start_id, _end_id) || this->existsDirectedEdge(_end_id, _start_id))) {
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

#include "operator.hpp"