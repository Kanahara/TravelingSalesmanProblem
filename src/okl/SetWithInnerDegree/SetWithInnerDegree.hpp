#pragma once

#include "../HashSet/module.hpp"

namespace okl {
	/*!
	 *  @brief      部分集合内次数付き頂点ID集合クラス
	 *  @details    部分集合内次数付き頂点ID集合クラス
	 *				高速化のコア部分
	 *				差分式次数更新を行う
	 *				- __Id					ID型
	 *				- __Degree				次数数値型
	 *				- __DegreeContainer		次数格納コンテナ
	 *				- __Set					頂点ID集合
	 */
	template<
		typename __Id,
		typename __Degree,
		class __DegreeContainer = std::vector<__Degree >,
		class __Set = HashSet<__Id>>
		class SetWithInnerDegree
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		SetWithInnerDegree() : bias_degree_(0) {};

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~SetWithInnerDegree() = default;

		using value_type = typename __Set::value_type;;
		using key_type = typename __Set::key_type;
		using size_type = typename __Set::size_type;
		using iterator = typename __Set::iterator;
		using const_iterator = typename __Set::const_iterator;

		using id_type = __Id;
		using degree_type = __Degree;
		using bias_degree_type = degree_type;
		using concat_iterator = ConcatIterator<iterator, id_type>;
		using concat_const_iterator = ConcatConstIterator<const_iterator, id_type>;

		/*!
		 * @brief			先頭イテレータを返す
		 * @return			iterator	先頭イテレータを返す
		 * @detail			先頭の要素のイテレータを返す
		 */
		inline concat_iterator begin()
		{
			concat_iterator _begin(this->updated_added_node_set_.begin(), this->updated_added_node_set_.end(), this->not_updated_added_node_set_.begin());
			return _begin;
		}

		/*!
		 * @brief			末尾の次のイテレータを返す
		 * @return			iterator	末尾の次のイテレータを返す
		 * @detail			末尾の要素の次のイテレータを返す
		 */
		inline iterator end()
		{
			return this->not_updated_added_node_set_.end();
		}

		/*!
		 * @brief			読み取り専用先頭イテレータを返す
		 * @return			iterator	読み取り専用先頭イテレータを返す
		 * @detail			先頭の要素の読み取り専用イテレータを返す
		 */
		constexpr concat_const_iterator begin() const
		{
			concat_const_iterator _begin(this->updated_added_node_set_.begin(), this->updated_added_node_set_.end(), this->not_updated_added_node_set_.begin());
			return _begin;
		}

		/*!
		 * @brief			読み取り専用末尾の次のイテレータを返す
		 * @return			iterator	読み取り専用末尾の次イテレータを返す
		 * @detail			末尾の要素の次のイテレータを返す
		 */
		constexpr const_iterator end() const
		{
			return this->not_updated_added_node_set_.end();
		}

		/*!
		 * @brief			読み取り専用先頭イテレータを返す
		 * @return			iterator	読み取り専用先頭イテレータを返す
		 * @detail			先頭の要素の読み取り専用イテレータを返す
		 */
		constexpr concat_const_iterator cbegin() const
		{
			concat_const_iterator _begin(this->updated_added_node_set_.begin(), this->updated_added_node_set_.end(), this->not_updated_added_node_set_.begin());
			return _begin;
		}

		/*!
		 * @brief			読み取り専用末尾の次のイテレータを返す
		 * @return			iterator	読み取り専用末尾の次イテレータを返す
		 * @detail			末尾の要素の次のイテレータを返す
		 */
		constexpr const_iterator cend() const
		{
			return this->not_updated_added_node_set_.end();
		}

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 *					メモリ解放は行わない
		 */
		inline virtual void clear()
		{
			this->bias_degree_ = 0;
			std::for_each(this->updated_added_node_set_.begin(), this->updated_added_node_set_.end(),
				[this](const auto _node_id) { this->degrees_[_node_id] = this->undefined_degree(); });
			this->updated_added_node_set_.clear();
			this->not_updated_added_node_set_.clear();
			this->not_updated_erased_node_set_.clear();
		}

		/*!
		 * @brief			格納可能要素数（容量）を設定
		 * @param[in]		__capacity 容量
		 * @detail			集合の容量を設定する
		 */
		inline virtual void reserve(const size_type __capacity)
		{
			this->degrees_.resize(__capacity, this->undefined_degree());
			this->updated_added_node_set_.reserve(__capacity);
			this->not_updated_added_node_set_.reserve(__capacity);
			this->not_updated_erased_node_set_.reserve(__capacity);
		}

		/*!
		 * @brief			空集合か判定
		 * @return			bool	要素数0ならTrue　空じゃないならFalse
		 * @detail			空集合か判定を行う
		 */
		constexpr bool isEmpty() const
		{
			return this->updated_added_node_set_.isEmpty() && this->not_updated_added_node_set_.isEmpty();
		}

		/*!
		 * @brief			要素数を返す
		 * @return			size_type	要素数を返す
		 * @detail			集合内の要素数を返す
		 */
		constexpr size_type size() const
		{
			return static_cast<size_type>(this->updated_added_node_set_.size() + this->not_updated_added_node_set_.size());
		}

		/*!
		 * @brief			格納可能要素数（容量）を返す
		 * @return			auto	格納可能要素数（容量）返す
		 * @detail			集合の容量を返す
		 */
		constexpr size_type capacity() const
		{
			return this->updated_added_node_set_.capacity();
		}

		/*!
		 * @brief			集合に要素を追加
		 * @param[in]		__node_id 頂点ID
		 * @detail			集合に頂点IDを追加する
		 *					指定頂点IDを末尾に追加する
		 *					次数は undefined_degree() が代入される
		 * 					- 要素型id_typeはテンプレート__Idによって定義される
		 */
		inline void insert(const id_type __node_id)
		{
			////削除した頂点が削除後更新を行う前に再度追加された場合
			//if (this->not_updated_erased_node_set_.contains(__node_id)) {
			//	//未更新削除集合から削除
			//	this->updated_added_node_set_.insert(__node_id);
			//	this->not_updated_erased_node_set_.erase(__node_id);
			//}
			//else {
				//未更新追加集合に追加
				this->not_updated_added_node_set_.insert(__node_id);
			//}
		}

		/*!
		 * @brief			集合に要素を追加
		 * @param[in]		__node_id 頂点ID
		 * @param[in]		__degree 次数
		 * @detail			集合に頂点IDを次数を設定して追加する
		 * 					- 要素型id_typeはテンプレート__Idによって定義される
		 * 					- 要素型degree_typeはテンプレート__Degreeによって定義される
		 */
		inline void insert(const id_type __node_id, const degree_type __degree)
		{
			this->updated_added_node_set_.insert(__node_id);
			this->setDegree(__node_id, __degree);
		}

		/*!
		 * @brief			集合から要素を削除
		 * @param[in]		__node_id 頂点ID
		 * @detail			集合から要素を削除する
		 *					指定要素を削除し指定要素が入っていた場所に末尾の要素を移動する
		 * 					- 要素型id_typeはテンプレート__Idによって定義される
		 */
		inline virtual void erase(const id_type __node_id)
		{
			////追加した頂点が追加後更新を行う前に再度削除された場合
			if (this->not_updated_added_node_set_.contains(__node_id)) {
				//未更新追加集合から削除
				this->not_updated_added_node_set_.erase(__node_id);
			}
			else {
				//未更新削除集合に追加
				this->updated_added_node_set_.erase(__node_id);
				this->not_updated_erased_node_set_.insert(__node_id);
			}
		}

		/*!
		 * @brief			集合に含まれているか判定
		 * @return			bool	含まれているならTrue　含まれていないらFalse
		 * @detail			集合に含まれているか判定を行う
		 */
		constexpr bool contains(const id_type __node_id) const
		{
			return this->updated_added_node_set_.contains(__node_id) || this->not_updated_added_node_set_.contains(__node_id);
		}

		/*!
		 * @brief			次数が更新済みかどうか判定する
		 * @return			bool	次数が更新済みで正しい状態ならTrue 更新済みでないならFalse
		 * @detail			次数が更新済みかどうか判定
		 */
		constexpr bool isUpdated() const
		{
			return this->not_updated_added_node_set_.isEmpty() && this->not_updated_erased_node_set_.isEmpty();
		}

		/*!
		 * @brief			次数を返す
		 * @return			degree_type	指定頂点の次数を返す
		 * @param[in]		__graph グラフ
		 * @param[in]		__node_id 指定の頂点のID
		 * @detail			指定頂点の次数を返す
		 * 					- 要素型degree_typeはテンプレート__Degreeによって定義される
		 * 					- 要素型id_typeはテンプレート__Idによって定義される
		 */
		template<class __Graph>
		inline degree_type degree(const __Graph& __graph, const id_type __node_id)
		{
			assert(this->contains(__node_id));
			this->update(__graph);
			return static_cast<degree_type>(this->degrees_[__node_id] - this->bias_degree_);
		}

		/*!
		 * @brief			大小比較用の内部で保持された未処理の次数を返す
		 * @return			degree_type	指定頂点の生次数を返す
		 * @param[in]		__graph グラフ
		 * @param[in]		__node_id 指定の頂点のID
		 * @detail			大小比較用に計算を行っていない次数を返す
		 * 					- 要素型degree_typeはテンプレート__Degreeによって定義される
		 * 					- 要素型id_typeはテンプレート__Idによって定義される
		 */
		template<class __Graph>
		inline degree_type row_degree(const __Graph& __graph, const id_type __node_id)
		{
			assert(this->contains(__node_id));
			this->update(__graph);
			return this->degrees_[__node_id];
		}

		/*!
		 * @brief			バイアス次数を取得する
		 * @detail			バイアス次数を取得する
		 */
		constexpr bias_degree_type bias_degree() const
		{
			return this->bias_degree_;
		}

		///*!
		// * @brief			要素をソートする
		// * @detail			要素を昇順にソートする
		// */
		//void sort();
		//{
		//	this->node_set_.sort();
		//}

		///*!
		// * @brief			要素をソートする
		// * @param[in]		__compare 比較関数
		// * @detail			要素を比較関数によりソートする
		// */
		//template<typename _Compare>
		//void sort(_Compare __compare);
		//{
		//	this->node_set_.sort(__compare);
		//}

		/*!
		 * @brief			要素をシャッフルする
		 * @param[in]		__generator ランダム生成機
		 * @detail			要素をランダム生成機を使用しシャッフルする
		 */
		template<typename _UniformRandomNumberGenerator>
		inline void shuffle(_UniformRandomNumberGenerator& __generator)
		{
			this->updated_added_node_set_.shuffle(__generator);
			this->not_updated_added_node_set_.shuffle(__generator);
		}

		/*!
		* @brief			代入処理を行う
		* @param[in]		__other 代入元対象
		* @return			SetWithInnerDegree&	自分自身の参照を返す
		* @detail			自身の要素格納場所情報を削除し代入元の要素の情報を代入する
		*/
		inline SetWithInnerDegree& operator=(const SetWithInnerDegree& __other)
		{
			std::for_each(__other.updated_added_node_set_.begin(), __other.updated_added_node_set_.end(),
				[this, &__other](const auto _node_id) { this->degrees_[_node_id] = __other.degrees_[_node_id]; });
			std::for_each(__other.not_updated_added_node_set_.begin(), __other.not_updated_added_node_set_.end(),
				[this, &__other](const auto _node_id) { this->degrees_[_node_id] = __other.degrees_[_node_id]; });
			this->updated_added_node_set_ = __other.updated_added_node_set_;
			this->not_updated_added_node_set_ = __other.not_updated_added_node_set_;
			this->not_updated_erased_node_set_ = __other.not_updated_erased_node_set_;
			this->bias_degree_ = __other.bias_degree_;
			return *this;
		}

		/*!
		* @brief			比較対象より小さいか比較を行う
		* @param[in]		__other 比較対象
		* @return			bool	比較対象より小さいならTrue　以上ならFalse
		* @detail
		*/
		constexpr bool operator<(const SetWithInnerDegree& __other) const
		{
			return this->size() < __other.size();
		}

		/*!
		* @brief			比較対象より大きいか比較を行う
		* @param[in]		__other 比較対象
		* @return			bool	比較対象より大きいならTrue　以下ならFalse
		* @detail
		*/
		constexpr bool operator>(const SetWithInnerDegree& __other) const
		{
			return __other < *this;
		}

		/*!
		* @brief			比較対象以上か比較を行う
		* @param[in]		__other 比較対象
		* @return			bool	比較対象以上ならTrue　より小さいならFalse
		* @detail
		*/
		constexpr bool operator<=(const SetWithInnerDegree& __other) const
		{
			return !(*this > __other);
		}

		/*!
		* @brief			比較対象以下か比較を行う
		* @param[in]		__other 比較対象
		* @return			bool	比較対象以下ならTrue　より大きいならFalse
		* @detail
		*/
		constexpr bool operator>=(const SetWithInnerDegree& __other) const
		{
			return !(*this < __other);
		}

		/*!
		 * @brief			指定のインデックスに入っている要素を取得
		 * @detail			指定のインデックスに入っている要素を取得する
		 * @param[in]		__index 格納番号
		 * @return			value_type&	指定要素の参照を返す
		 * 					- 要素型value_typeはテンプレート__Idによって定義される
		 */
		inline const value_type& operator[](const key_type __index) const&
		{
			ConcatContainer<const_iterator, id_type> _nodes(this->updated_added_node_set_.begin(), this->updated_added_node_set_.end(), this->not_updated_added_node_set_.begin(), this->not_updated_added_node_set_.end());
			return _nodes[__index];
		}

		/*!
		 * @brief			指定のインデックスに入っている要素を取得
		 * @detail			指定のインデックスに入っている要素を取得する
		 * @param[in]		__index 格納番号
		 * @return			value_type&	指定要素の参照を返す
		 * 					- 要素型value_typeはテンプレート__Idによって定義される
		 */
		inline value_type operator[](const key_type __index)&&
		{
			ConcatContainer<const_iterator, id_type> _nodes(this->updated_added_node_set_.begin(), this->updated_added_node_set_.end(), this->not_updated_added_node_set_.begin(), this->not_updated_added_node_set_.end());
			return _nodes[__index];
		}

	private:
		//!バイアス次数
		bias_degree_type bias_degree_;

		//!次数コンテナ
		__DegreeContainer degrees_;

		//!更新処理が行われた追加された頂点集合
		__Set updated_added_node_set_;

		//!更新処理が行われていない追加された頂点集合
		__Set not_updated_added_node_set_;

		//!更新処理が行われていない削除された頂点集合
		__Set not_updated_erased_node_set_;

		/*!
		 * @brief			次数を更新
		 * @param[in]		__graph グラフ
		 * @detail			次数の更新を行う
		 * 					updateAddedVertices()　updateErasedVertices()	を実行する
		 */
		template<class __Graph>
		inline void update(const __Graph& __graph)
		{
			this->updateErasedVertices(__graph);
			this->updateAddedVertices(__graph);
		}

		/*!
		 * @brief			追加された頂点次数を更新
		 * @detail			追加された頂点IDに関連する頂点次数を更新
		 *					複数頂点まとめて処理を行う
		 */
		template<class __Graph>
		inline void updateAddedVertices(const __Graph& __graph)
		{
			std::for_each(this->not_updated_added_node_set_.begin(), this->not_updated_added_node_set_.end(),
				[this, &__graph](const auto _not_updated_added_node_id) {
					const size_type _updated_added_node_set_size = static_cast<size_type>(this->updated_added_node_set_.size());
					const size_type _adjacent_set_size = static_cast<size_type>(__graph.adjacent(_not_updated_added_node_id).size());
					const size_type _non_adjacent_set_size = static_cast<size_type>(__graph.non_adjacent(_not_updated_added_node_id).size());
					size_type _min_size = 0;
					if constexpr (__Graph::has_non_adjacent::value) {
						_min_size = std::min({ _updated_added_node_set_size, _adjacent_set_size, _non_adjacent_set_size });
					}
					else {
						_min_size = std::min({ _updated_added_node_set_size, _adjacent_set_size });
					}
					if (_min_size == _updated_added_node_set_size) {
						this->setDegree(_not_updated_added_node_id, this->bias_degree());
						std::for_each(this->updated_added_node_set_.begin(), this->updated_added_node_set_.end(),
							[this, &__graph, _not_updated_added_node_id](const auto _updated_added_node_id) {
								if (__graph.existsDirectedEdge(_not_updated_added_node_id, _updated_added_node_id)) {
									this->incDegree(_not_updated_added_node_id);
									this->incDegree(_updated_added_node_id);
								}
								//const auto _exists_directed_edge = __graph.existsDirectedEdge(_not_updated_added_node_id, _updated_added_node_id);
								//this->degrees_[_not_updated_added_node_id] = _exists_directed_edge ? this->degrees_[_not_updated_added_node_id] + 1: this->degrees_[_not_updated_added_node_id];
								//this->degrees_[_updated_added_node_id] = _exists_directed_edge ? this->degrees_[_updated_added_node_id] + 1 : this->degrees_[_updated_added_node_id];
							});
					}
					else if (_min_size == _adjacent_set_size) {
						this->setDegree(_not_updated_added_node_id, this->bias_degree());
						std::for_each(__graph.adjacent(_not_updated_added_node_id).begin(), __graph.adjacent(_not_updated_added_node_id).end(),
							[this, _not_updated_added_node_id](const auto _adjacent_node_id) {
								//if (this->updated_added_node_set_.contains(_adjacent_node_id)) {
								//	this->incDegree(_not_updated_added_node_id);
								//	this->incDegree(_adjacent_node_id);
								//}
								++this->degrees_[_adjacent_node_id];
								this->degrees_[_not_updated_added_node_id] = this->updated_added_node_set_.contains(_adjacent_node_id) ? this->degrees_[_not_updated_added_node_id] + 1 : this->degrees_[_not_updated_added_node_id];
							});
					}
					else {
						if constexpr (__Graph::has_non_adjacent::value) {
							if (_min_size == _non_adjacent_set_size) {
								this->decBiasDegree();
								this->setDegree(_not_updated_added_node_id, static_cast<degree_type>(this->updated_added_node_set_.size() + this->bias_degree()));
								std::for_each(__graph.non_adjacent(_not_updated_added_node_id).begin(), __graph.non_adjacent(_not_updated_added_node_id).end(),
									[this, _not_updated_added_node_id](const auto _non_adjacent_node_id) {
										//if (this->updated_added_node_set_.contains(_non_adjacent_node_id)) {
										//	this->decDegree(_not_updated_added_node_id);
										//	this->decDegree(_non_adjacent_node_id);
										//}
										--this->degrees_[_non_adjacent_node_id];
										this->degrees_[_not_updated_added_node_id] = this->updated_added_node_set_.contains(_non_adjacent_node_id) ? this->degrees_[_not_updated_added_node_id] - 1 : this->degrees_[_not_updated_added_node_id];
									});
							}
							else {
								assert(false);
							}
						}
						else {
							assert(false);
						}
					}
					this->updated_added_node_set_.insert(_not_updated_added_node_id);
				});
			this->not_updated_added_node_set_.clear();
		}

		/*!
		 * @brief			削除された頂点次数を更新
		 * @detail			削除された頂点ID集合に関連する頂点次数を更新
		 *					複数頂点まとめて処理を行う
		 */
		template<class __Graph>
		inline void updateErasedVertices(const __Graph& __graph)
		{
			const size_type _set_size = static_cast<size_type>(this->size() * 17);
			
			std::for_each(this->not_updated_erased_node_set_.begin(), this->not_updated_erased_node_set_.end(),
				[this, &__graph, _set_size](const auto _erased_node_id) {
					const size_type _adjacent_set_size = static_cast<size_type>(__graph.adjacent(_erased_node_id).size() * 10);
					const size_type _non_adjacent_set_size = static_cast<size_type>(__graph.non_adjacent(_erased_node_id).size() * 10);
					size_type _min_size = 0;
					if constexpr (__Graph::has_non_adjacent::value) {
						_min_size = std::min({ _set_size, _adjacent_set_size, _non_adjacent_set_size });
					}
					else {
						_min_size = std::min({ _set_size, _adjacent_set_size });
					}
					if (_min_size == _set_size) {
						std::for_each(this->updated_added_node_set_.begin(), this->updated_added_node_set_.end(),
							[this, &__graph, _erased_node_id](const auto _updated_added_node_id) {
								//if (__graph.existsDirectedEdge(_erased_node_id, _updated_added_node_id)) {
								//	this->decDegree(_updated_added_node_id);
								//}
								this->degrees_[_updated_added_node_id] = __graph.existsDirectedEdge(_erased_node_id, _updated_added_node_id) ? this->degrees_[_updated_added_node_id] - 1 : this->degrees_[_updated_added_node_id];
							});
						std::for_each(this->not_updated_added_node_set_.begin(), this->not_updated_added_node_set_.end(),
							[this, &__graph, _erased_node_id](const auto _not_updated_added_node_id) {
								//if (__graph.existsDirectedEdge(_erased_node_id, _not_updated_added_node_id)) {
								//	this->decDegree(_not_updated_added_node_id);
								//}
								this->degrees_[_not_updated_added_node_id] = __graph.existsDirectedEdge(_erased_node_id, _not_updated_added_node_id) ? this->degrees_[_not_updated_added_node_id] - 1 : this->degrees_[_not_updated_added_node_id];
							});
					}
					else if (_min_size == _adjacent_set_size) {
						std::for_each(__graph.adjacent(_erased_node_id).begin(), __graph.adjacent(_erased_node_id).end(),
							[this](const auto _adjacent_node_id) {
								//if (this->contains(_adjacent_node_id)) {
								//	this->decDegree(_adjacent_node_id);
								//}
								//this->degrees_[_adjacent_node_id] = this->contains(_adjacent_node_id) ? this->degrees_[_adjacent_node_id] - 1 : this->degrees_[_adjacent_node_id];
								--this->degrees_[_adjacent_node_id];
						});
					}
					else {
						if constexpr (__Graph::has_non_adjacent::value) {
							if (_min_size == _non_adjacent_set_size) {
								this->incBiasDegree();
								std::for_each(__graph.non_adjacent(_erased_node_id).begin(), __graph.non_adjacent(_erased_node_id).end(),
									[this](const auto _non_adjacent_node_id) {
										//if (this->contains(_non_adjacent_node_id)) {
										//	this->incDegree(_non_adjacent_node_id);
										//}
										//this->degrees_[_non_adjacent_node_id] = this->contains(_non_adjacent_node_id) ? this->degrees_[_non_adjacent_node_id] + 1 : this->degrees_[_non_adjacent_node_id];
										++this->degrees_[_non_adjacent_node_id];
									});
							}
							else {
								assert(false);
							}
						}
						else {
							assert(false);
						}
					}
				});
			this->not_updated_erased_node_set_.clear();
		}

		/*!
		 * @brief			次数を設定
		 * @param[in]		__node_id 指定の頂点のID
		 * @detail			指定頂点の次数を設定する
		 * 					- 要素型id_typeはテンプレート__Idによって定義される
		 * 					- 要素型degree_typeはテンプレート__Degreeによって定義される
		 */
		inline void setDegree(const id_type __node_id, const degree_type __degree)
		{
			assert(this->contains(__node_id));
			this->degrees_[__node_id] = __degree;
		}

		/*!
		 * @brief			次数を+1する
		 * @param[in]		__node_id 指定の頂点のID
		 * @detail			指定頂点の次数を+1する
		 * 					- 要素型id_typeはテンプレート__Idによって定義される
		 */
		inline void incDegree(const id_type __node_id)
		{
			assert(this->contains(__node_id));
			++this->degrees_[__node_id];
		}

		/*!
		 * @brief			次数を-1する
		 * @param[in]		__node_id 指定の頂点のID
		 * @detail			指定頂点の次数を-1する
		 * 					- 要素型vertex_typeはテンプレート__Graphのdegree_typeによって定義される
		 */
		inline void decDegree(const id_type __node_id)
		{
			assert(this->contains(__node_id));
			--this->degrees_[__node_id];
		}

		/*!
		 * @brief			次数を+nする
		 * @param[in]		__node_id 指定の頂点のID
		 * @param[in]		__n 引く数値
		 * @detail			指定頂点の次数を+nする
		 * 					- 要素型id_typeはテンプレート__Idによって定義される
		 */
		inline void plusDegree(const id_type __node_id, const degree_type __n)
		{
			assert(this->contains(__node_id));
			this->degrees_[__node_id] += __n;
		}

		/*!
		 * @brief			次数を-nする
		 * @param[in]		__node_id 指定の頂点のID
		 * @param[in]		__n 引く数値
		 * @detail			指定頂点の次数を-nする
		 * 					- 要素型vertex_typeはテンプレート__Graphのdegree_typeによって定義される
		 */
		inline void minusDegree(const id_type __node_id, const degree_type __n)
		{
			assert(this->contains(__node_id));
			this->degrees_[__node_id] -= __n;
		}

		/*!
		 * @brief			バイアス次数を+1する
		 * @detail			バイアス次数を+1する
		 */
		inline void incBiasDegree()
		{
			++this->bias_degree_;
		}

		/*!
		 * @brief			バイアス次数を-1する
		 * @detail			バイアス次数を-1する
		 */
		inline void decBiasDegree()
		{
			--this->bias_degree_;
		}

		/*!
		 * @brief			未定義次数
		 * @return			degree_type	未定義値としてdegree_typeの最大値を返す
		 * @detail			未定義次数を返す
		 * 					- 要素型degree_typeはテンプレート__Degreeによって定義される
		 */
		constexpr degree_type undefined_degree() const
		{
			return std::numeric_limits < degree_type > ::max();
		}
	};
} /* namespace okl */

#include "operator.hpp"