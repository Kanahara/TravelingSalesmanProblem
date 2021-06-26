#pragma once

#include "../HashSet/module.hpp"

namespace okl {
	/*!
	 *  @brief      種類集合クラス
	 *  @details    種類集合クラス
	 *				- __Value			要素型
	 *				- __Key				インデックス数値型
	 *				- __Set				集合型
	 *				- __SizeContainer	サイズ配列型
	 */
	template<
		class __Value,
		class __Set = okl::HashSet<__Value>,
		class __SizeContainer = std::vector<typename __Set::size_type> >
		class TypeSet {
		public:
			/*!
			 * @brief				コンストラクタ
			 */
			TypeSet() = default;

			/*!
			 * @brief				デストラクタ
			 */
			virtual ~TypeSet() = default;

			using value_type = typename __Set::value_type;
			using size_type = typename __Set::size_type;
			using iterator = typename __Set::iterator;
			using const_iterator = typename __Set::const_iterator;
			using key_type = typename __Set::key_type;

			using set_type = __Set;
			using size_container_type = __SizeContainer;

			/*!
			 * @brief			先頭イテレータを返す
			 * @return			iterator	先頭イテレータを返す
			 * @detail			先頭の要素のイテレータを返す
			 */
			inline iterator begin()
			{
				return this->set_.begin();
			}

			/*!
			 * @brief			末尾の次のイテレータを返す
			 * @return			iterator	末尾の次のイテレータを返す
			 * @detail			末尾の要素の次のイテレータを返す
			 */
			inline iterator end()
			{
				return this->set_.end();
			}

			/*!
			 * @brief			読み取り専用先頭イテレータを返す
			 * @return			iterator	読み取り専用先頭イテレータを返す
			 * @detail			先頭の要素の読み取り専用イテレータを返す
			 */
			constexpr const_iterator begin() const
			{
				return this->set_.begin();
			}

			/*!
			 * @brief			読み取り専用末尾の次のイテレータを返す
			 * @return			iterator	読み取り専用末尾の次イテレータを返す
			 * @detail			末尾の要素の次のイテレータを返す
			 */
			constexpr const_iterator end() const
			{
				return this->set_.end();
			}

			/*!
			 * @brief			読み取り専用先頭イテレータを返す
			 * @return			iterator	読み取り専用先頭イテレータを返す
			 * @detail			先頭の要素の読み取り専用イテレータを返す
			 */
			constexpr const_iterator cbegin() const
			{
				return this->set_.cbegin();
			}

			/*!
			 * @brief			読み取り専用末尾の次のイテレータを返す
			 * @return			iterator	読み取り専用末尾の次イテレータを返す
			 * @detail			末尾の要素の次のイテレータを返す
			 */
			constexpr const_iterator cend() const
			{
				return this->set_.cend();
			}

			/*!
			 * @brief			空集合か判定
			 * @return			bool	要素数0ならTrue　空じゃないならFalse
			 * @detail			空集合か判定を行う
			 */
			constexpr bool isEmpty() const
			{
				return this->set_.isEmpty();
			}

			/*!
			 * @brief			要素数を返す
			 * @return			size_type	要素数を返す
			 * @detail			集合内の要素数を返す
			 */
			constexpr size_type size() const
			{
				return this->set_.size();
			}

			/*!
			 * @brief			格納可能要素数（容量）を返す
			 * @return			auto	格納可能要素数（容量）返す
			 * @detail			集合の容量を返す
			 */
			constexpr size_type capacity() const
			{
				return this->set_.capacity();
			}

			/*!
			 * @brief			指定要素の要素数を返す
			 * @param[in]		__value 要素
			 * @return			size_type	要素数を返す
			 * @detail			集合内の要素数を返す
			 */
			constexpr size_type type_size(const value_type __value) const
			{
				return this->size_container_[__value];
			}

			/*!
			 * @brief			集合に要素を追加
			 * @param[in]		__value 要素
			 * @detail			集合に要素を追加する
			 *					要素の数を増やし、指定要素が集合にない場合追加
			 * 					- 要素型value_typeはテンプレート__Valueによって定義される
			 */
			inline void insert(const value_type __value)
			{
				++this->size_container_[__value];
				if (!this->set_.contains(__value)) {
					this->set_.insert(__value);
				}
			}

			/*!
			 * @brief			集合から要素を削除
			 * @param[in]		__value 要素
			 * @detail			集合から要素を削除する
			 *					要素の数を減らし、指定要素が0場合集合から削除
			 * 					- 要素型value_typeはテンプレート__Valueによって定義される
			 */
			inline void erase(const value_type __value)
			{
				--this->size_container_[__value];
				if (this->type_size(__value) == 0) {
					this->set_.erase(__value);
				}
			}

			/*!
			 * @brief			集合から要素を移動
			 * @param[in]		__value 要素
			 * @param[in]		__other 移動先
			 * @detail			集合から要素を削除し、__otherに要素を追加する
			 * 					- 要素型value_typeはテンプレート__Valueによって定義される
			 */
			inline void moveTo(const value_type __value, TypeSet& __other)
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
				std::for_each(this->begin(), this->end(), [this](const auto _color) { this->size_container_[_color] = 0; });
				this->set_.clear();
			}

			/*!
			 * @brief			集合に含まれているか判定
			 * @return			bool	含まれているならTrue　含まれていないらFalse
			 * @detail			集合に含まれているか判定を行う
			 */
			constexpr bool contains(const value_type __value) const
			{
				return this->set_.contains(__value);
			}

			/*!
			 * @brief			格納可能要素数（容量）を設定
			 * @param[in]		__capacity 容量
			 * @detail			集合の容量を設定する
			 */
			inline void reserve(const size_type __capacity)
			{
				this->set_.reserve(__capacity);
				this->size_container_.resize(__capacity);
			}

			/*!
			 * @brief			要素をソートする
			 * @detail			要素を昇順にソートする
			 */
			inline void sort()
			{
				this->set_.sort();
			}

			/*!
			 * @brief			要素をソートする
			 * @param[in]		__compare 比較関数
			 * @detail			要素を比較関数によりソートする
			 */
			template<typename _Compare>
			inline void sort(_Compare __compare)
			{
				this->set_.sort(__compare);
			}

			/*!
			 * @brief			要素をシャッフルする
			 * @param[in]		__generator ランダム生成機
			 * @detail			要素をランダム生成機を使用しシャッフルする
			 */
			template<typename _UniformRandomNumberGenerator>
			inline void shuffle(_UniformRandomNumberGenerator& __generator)
			{
				this->set_.shuffle(__generator);
			}

			/*!
			* @brief			代入処理を行う
			* @param[in]		__other 代入元対象
			* @return			TypeSet&	自分自身の参照を返す
			* @detail			自身の要素格納場所情報を削除し代入元の要素の情報を代入する
			*/
			inline TypeSet& operator=(const TypeSet& __other)
			{
				std::for_each(this->begin(), this->end(), [this](const auto __value) { this->size_container_[__value] = 0; });
				std::for_each(__other.begin(), __other.end(),
					[this, &__other](const auto __value) {
						this->size_container_[__value] = __other.size_container_[__value];
					});
				this->set_ = __other.set_;

				return *this;
			}

			/*!
			* @brief			比較対象より小さいか比較を行う
			* @param[in]		__other 比較対象
			* @return			bool	比較対象より小さいならTrue　以上ならFalse
			* @detail
			*/
			constexpr bool operator<(const TypeSet& __other) const
			{
				return this->size() < __other.size();
			}

			/*!
			* @brief			比較対象より大きいか比較を行う
			* @param[in]		__other 比較対象
			* @return			bool	比較対象より大きいならTrue　以下ならFalse
			* @detail
			*/
			constexpr bool operator>(const TypeSet& __other) const
			{
				return __other < *this;
			}

			/*!
			* @brief			比較対象以上か比較を行う
			* @param[in]		__other 比較対象
			* @return			bool	比較対象以上ならTrue　より小さいならFalse
			* @detail
			*/
			constexpr bool operator<=(const TypeSet& __other) const
			{
				return !(*this > __other);
			}

			/*!
			* @brief			比較対象以下か比較を行う
			* @param[in]		__other 比較対象
			* @return			bool	比較対象以下ならTrue　より大きいならFalse
			* @detail
			*/
			constexpr bool operator>=(const TypeSet& __other) const
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
				return this->set_[__index];
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
				return std::move(this->set_[__index]);
			}

		protected:
			//!種類集合
			set_type set_;

			//!種類内各種類の数
			size_container_type size_container_;
	};
} /* namespace okl */

#include "operator.hpp"