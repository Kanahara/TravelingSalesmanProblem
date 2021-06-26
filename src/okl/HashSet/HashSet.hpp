#pragma once
#include <vector>
#include <limits>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include "../Hash/module.hpp"

namespace okl {
	/*!
	 *  @brief      ハッシュ集合クラス
	 *  @details    ハッシュ集合クラス
	 *				使用するハッシュ関数としては　完全ハッシュかつ0以上capacity()未満の数値を出力する関数である必要がある
	 *				符号なし整数型だと　ハッシュ関数はf(x)=xで良い
	 *				- __Value				要素型
	 *				- __Key					インデックス数値型
	 *				- __Store				要素格納コンテナ
	 *				- __Location			格納場所コンテナ
	 */
	template<
		class __Value,
		typename __Key = std::size_t,
		class __Hash = Hash<__Value>,
		class __Store = std::vector<__Value >,
		class __Location = std::vector<__Key > >
		class HashSet {
		public:
			/*!
			 * @brief				コンストラクタ
			 */
			HashSet() : size_(0) {}

			/*!
			 * @brief				デストラクタ
			 */
			virtual ~HashSet() = default;

			using value_type = __Value;
			using key_type = __Key;
			using size_type = key_type;
			using iterator = typename __Store::iterator;
			using const_iterator = typename __Store::const_iterator;
			using hasher = __Hash;

			/*!
			 * @brief			先頭イテレータを返す
			 * @return			iterator	先頭イテレータを返す
			 * @detail			先頭の要素のイテレータを返す
			 */
			inline iterator begin()
			{
				return this->store_.begin();
			}

			/*!
			 * @brief			末尾の次のイテレータを返す
			 * @return			iterator	末尾の次のイテレータを返す
			 * @detail			末尾の要素の次のイテレータを返す
			 */
			inline iterator end()
			{
				return this->store_.begin() + this->size();
			}

			/*!
			 * @brief			読み取り専用先頭イテレータを返す
			 * @return			iterator	読み取り専用先頭イテレータを返す
			 * @detail			先頭の要素の読み取り専用イテレータを返す
			 */
			constexpr const_iterator begin() const
			{
				return this->store_.begin();
			}

			/*!
			 * @brief			読み取り専用末尾の次のイテレータを返す
			 * @return			iterator	読み取り専用末尾の次イテレータを返す
			 * @detail			末尾の要素の次のイテレータを返す
			 */
			constexpr const_iterator end() const
			{
				return this->store_.begin() + this->size();
			}

			/*!
			 * @brief			読み取り専用先頭イテレータを返す
			 * @return			iterator	読み取り専用先頭イテレータを返す
			 * @detail			先頭の要素の読み取り専用イテレータを返す
			 */
			constexpr const_iterator cbegin() const
			{
				return this->store_.cbegin();
			}

			/*!
			 * @brief			読み取り専用末尾の次のイテレータを返す
			 * @return			iterator	読み取り専用末尾の次イテレータを返す
			 * @detail			末尾の要素の次のイテレータを返す
			 */
			constexpr const_iterator cend() const
			{
				return this->store_.cbegin() + this->size();
			}

			/*!
			 * @brief			先頭の参照を返す
			 * @return			value_type&	先頭の参照を返す
			 * @detail			先頭の要素の参照を返す
			 *					- 要素型value_typeはテンプレート__Valueによって定義される
			 */
			inline value_type& front()
			{
				return *(this->store_.begin());
			}

			/*!
			 * @brief			先頭の参照を返す
			 * @return			value_type&	先頭の参照を返す
			 * @detail			先頭の要素の参照を返す
			 *					- 要素型value_typeはテンプレート__Valueによって定義される
			 */
			const value_type& front() const
			{
				return *(this->store_.begin());
			}

			/*!
			 * @brief			末尾の参照を返す
			 * @return			value_type&	末尾の参照を返す
			 * @detail			末尾の要素の参照を返す
			 *					- 要素型value_typeはテンプレート__Valueによって定義される
			 */
			inline value_type& back()
			{
				return *(this->store_.begin() + this->size() - 1);
			}

			/*!
			 * @brief			末尾の参照を返す
			 * @return			value_type&	末尾の参照を返す
			 * @detail			末尾の要素の参照を返す
			 *					- 要素型value_typeはテンプレート__Valueによって定義される
			 */
			const value_type& back() const
			{
				return *(this->store_.begin() + this->size() - 1);
			}

			/*!
			 * @brief			空集合か判定
			 * @return			bool	要素数0ならTrue　空じゃないならFalse
			 * @detail			空集合か判定を行う
			 */
			constexpr bool isEmpty() const
			{
				return this->size_ == 0;
			}

			/*!
			 * @brief			要素数を返す
			 * @return			size_type	要素数を返す
			 * @detail			集合内の要素数を返す
			 */
			constexpr size_type size() const
			{
				return this->size_;
			}

			/*!
			 * @brief			格納可能要素数（容量）を返す
			 * @return			size_type	格納可能要素数（容量）返す
			 * @detail			集合の容量を返す
			 */
			constexpr size_type capacity() const
			{
				return this->store_.size();
			}

			/*!
			 * @brief			集合に要素を追加
			 * @param[in]		__value 要素
			 * @detail			集合に要素を追加する
			 *					指定要素を末尾に追加する
			 * 					- 要素型value_typeはテンプレート__Valueによって定義される
			 */
			inline void insert(const value_type __value)
			{
				assert(!this->contains(__value));
				this->store_[this->size()] = __value;
				this->location_[hasher()(__value)] = this->size();
				++this->size_;
			}
			inline void insert(const value_type __node_id, const long int __degree) { this->insert(__node_id); };

			/*!
			 * @brief			集合に要素がないなら要素を追加
			 * @param[in]		__value 要素
			 * @detail			集合に要素がないなら要素を追加する
			 *					指定要素を末尾に追加する
			 * 					- 要素型value_typeはテンプレート__Valueによって定義される
			 */
			inline void insert_if_not_contains(const value_type __value)
			{
				if (!this->contains(__value)) {
					this->insert(__value);
				}
			}

			/*!
			 * @brief			集合から要素を削除
			 * @param[in]		__value 要素
			 * @return			const_iterator	削除した場所のイテレータ（新しい要素が入ってる場所）
			 * @detail			集合から要素を削除する
			 *					指定要素を削除し指定要素が入っていた場所に末尾の要素を移動する
			 * 					- 要素型value_typeはテンプレート__Valueによって定義される
			 */
			inline const_iterator erase(const value_type __value)
			{
				return this->erase_key(hasher()(__value));
			}

			/*!
			 * @brief			集合かキーが示す要素を削除
			 * @param[in]		__value 要素
			 * @return			const_iterator	削除した場所のイテレータ（新しい要素が入ってる場所）
			 * @detail			集合からキーが要素を削除する
			 *					キーが指定する要素を削除し指定要素が入っていた場所に末尾の要素を移動する
			 * 					- 要素型value_typeはテンプレート__Valueによって定義される
			 */
			inline const_iterator erase_key(const key_type __key)
			{
				assert(this->location_[__key] != this->undefined_index());

				const auto _location = this->location_[__key];
				const auto _last_index = this->size() - 1;
				if (_location != _last_index) {
					this->location_[hasher()(this->store_[_last_index])] = _location;
					this->store_[_location] = this->store_[_last_index];
				}
				this->location_[__key] = this->undefined_index();
				--this->size_;

				return this->begin() + _location;
			}

			/*!
			 * @brief			集合に要素があるなら要素を削除
			 * @param[in]		__value 要素
			 * @return			const_iterator	削除した場所のイテレータ（新しい要素が入ってる場所）
			 * @detail			集合に要素があるなら要素を削除する
			 *					指定要素を削除し指定要素が入っていた場所に末尾の要素を移動する
			 * 					- 要素型value_typeはテンプレート__Valueによって定義される
			 */
			inline const_iterator erase_if_contains(const value_type __value)
			{
				if (this->contains(__value)) {
					return this->erase(__value);
				}
				else {
					return this->end();
				}
			}

			/*!
			 * @brief			集合から要素を移動
			 * @param[in]		__value 要素
			 * @param[in]		__other 移動先
			 * @detail			集合から要素を削除し、__otherに要素を追加する
			 * 					- 要素型value_typeはテンプレート__Valueによって定義される
			 */
			inline void moveTo(const value_type __value, HashSet& __other)
			{
				this->erase(__value);
				__other.insert(__value);
			}

			/*!
			 * @brief			全要素初期化
			 * @detail			全要素を初期化処理を行う
			 *					メモリ解放は行わない
			 */
			inline void clear()
			{
				std::for_each(this->begin(), this->end(), [this](const auto __value) { this->location_[hasher()(__value)] = this->undefined_index(); });
				this->size_ = 0;
			}

			/*!
			 * @brief			集合に含まれているか判定
			 * @return			bool	含まれているならTrue　含まれていないらFalse
			 * @detail			集合に含まれているか判定を行う
			 */
			constexpr bool contains(const value_type __value) const
			{
				assert(hasher()(__value) < this->location_.size());
				return this->location_[hasher()(__value)] != this->undefined_index();
			}

			/*!
			 * @brief			格納可能要素数（容量）を設定
			 * @param[in]		__capacity 容量
			 * @detail			集合の容量を設定する
			 */
			inline void reserve(const size_type __capacity)
			{
				this->store_.resize(__capacity);
				this->location_.resize(__capacity, this->undefined_index());
			}

			/*!
			 * @brief			集合の最後の要素を削除
			 * @detail			集合の最後の要素を削除する
			 * @return			value_type	削除される要素を返す
			 * 					- 要素型value_typeはテンプレート__Valueによって定義される
			 */
			inline const value_type& pop()
			{
				--this->size_;
				this->location_[hasher()(this->store_[this->size_])] = this->undefined_index();
				return this->store_[this->size_];
			}

			/*!
			 * @brief			要素格納場所を返す
			 * @detail			要素格納場所を返す
			 * @return			key_type	要素格納場所を返す
			 * 					- 要素型value_typeはテンプレート__Valueによって定義される
			 */
			constexpr key_type index(const value_type& __value) const
			{
				return this->location_[hasher()(__value)];
			}

			/*!
			 * @brief			キーで指定される要素を返す
			 * @param[in]		__key キー
			 * @detail			キーで指定される要素を返す
			 * @return			const_iterator	キーで指定される要素イテレータを返す
			 * 					- 要素型key_typeはテンプレート__Keyによって定義される
			 */
			constexpr const_iterator find_key(const key_type __key) const
			{
				if (this->location_[__key] == this->undefined_index()) {
					return this->end();
				}
				else {
					return this->begin() + this->location_[__key];
				}
			}

			/*!
			 * @brief			要素をソートする
			 * @detail			要素を昇順にソートする
			 */
			inline void sort()
			{
				std::sort(this->begin(), this->end());
				this->relocation();
			}

			/*!
			 * @brief			要素をソートする
			 * @param[in]		__compare 比較関数
			 * @detail			要素を比較関数によりソートする
			 */
			template<typename _Compare>
			inline void sort(_Compare __compare)
			{
				std::sort(this->begin(), this->end(), __compare);
				this->relocation();
			}

			/*!
			 * @brief			要素をシャッフルする
			 * @param[in]		__generator ランダム生成機
			 * @detail			要素をランダム生成機を使用しシャッフルする
			 */
			template<typename _UniformRandomNumberGenerator>
			inline void shuffle(_UniformRandomNumberGenerator& __generator)
			{
				std::shuffle(this->begin(), this->end(), __generator);
				this->relocation();
			}

			/*!
			* @brief			代入処理を行う
			* @param[in]		__other 代入元対象
			* @return			HashSet&	自分自身の参照を返す
			* @detail			自身の要素格納場所情報を削除し代入元の要素の情報を代入する
			*/
			inline HashSet& operator=(const HashSet& __other)
			{
				std::for_each(this->begin(), this->end(), [this](const auto __value) { this->location_[__value] = this->undefined_index(); });
				std::for_each(__other.begin(), __other.end(),
					[this, &__other](const auto __value) {
						this->location_[hasher()(__value)] = __other.location_[hasher()(__value)];
						this->store_[this->location_[hasher()(__value)]] = __other.store_[__other.location_[hasher()(__value)]];
					});
				this->size_ = __other.size();
				return *this;
			}

			/*!
			* @brief			比較対象より小さいか比較を行う
			* @param[in]		__other 比較対象
			* @return			bool	比較対象より小さいならTrue　以上ならFalse
			* @detail
			*/
			constexpr bool operator<(const HashSet& __other) const
			{
				return this->size() < __other.size();
			}

			/*!
			* @brief			比較対象より大きいか比較を行う
			* @param[in]		__other 比較対象
			* @return			bool	比較対象より大きいならTrue　以下ならFalse
			* @detail
			*/
			constexpr bool operator>(const HashSet& __other) const
			{
				return __other < *this;
			}

			/*!
			* @brief			比較対象以上か比較を行う
			* @param[in]		__other 比較対象
			* @return			bool	比較対象以上ならTrue　より小さいならFalse
			* @detail
			*/
			constexpr bool operator<=(const HashSet& __other) const
			{
				return !(*this > __other);
			}

			/*!
			* @brief			比較対象以下か比較を行う
			* @param[in]		__other 比較対象
			* @return			bool	比較対象以下ならTrue　より大きいならFalse
			* @detail
			*/
			constexpr bool operator>=(const HashSet& __other) const
			{
				return !(*this < __other);
			}

			/*!
			 * @brief			指定のインデックスに入っている要素を取得
			 * @detail			指定のインデックスに入っている要素を取得する
			 * @param[in]		__index 格納番号
			 * @return			value_type&	指定要素の参照を返す
			 * 					- 要素型value_typeはテンプレート__Valueによって定義される
			 */
			inline const value_type& operator[](const key_type __index) const&
			{
				assert(__index < this->size());
				return this->store_[__index];
			}

			/*!
			 * @brief			指定のインデックスに入っている要素を取得
			 * @detail			指定のインデックスに入っている要素を取得する
			 * @param[in]		__index 格納番号
			 * @return			value_type&	指定要素の参照を返す
			 * 					- 要素型value_typeはテンプレート__Valueによって定義される
			 */
			inline value_type operator[](const key_type __index)&&
			{
				assert(__index < this->size());
				return std::move(this->store_[__index]);
			}

		protected:
			//!サイズ変数
			size_type size_;

			//!データストア配列
			__Store store_;

			//!データ格納場所保持配列
			__Location location_;

			/*!
			 * @brief			要素格納場所再設定する
			 * @detail			sort()　または、　shuffle()後　contains()	の正当性を保つために要素格納場所再設定する
			 */
			inline void relocation()
			{
				for (key_type _index = 0; _index < this->size(); ++_index) {
					this->location_[hasher()(this->store_[_index])] = _index;
				}
			}

			/*!
			 * @brief			未定義インデックス
			 * @return			key_type	未定義インデックスとしてkey_typeの最大値を返す
			 * @detail			未定義インデックスを返す
			 */
			constexpr key_type undefined_index() const
			{
				return std::numeric_limits < key_type > ::max();
			}

			/*!
			 * @brief			未定義値
			 * @return			value_type	未定義値としてvalue_typeの最大値を返す
			 * @detail			未定義値を返す
			 */
			constexpr value_type undefined_value() const
			{
				return std::numeric_limits < value_type > ::max();
			}
	};
} /* namespace okl */

#include "operator.hpp"