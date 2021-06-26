#pragma once
#include <vector>

namespace okl {
	/*!
	 *  @brief      スタッククラス
	 *  @details    スタッククラス
	 *				- __Value				要素型
	 *				- __Key					インデックス数値型
	 *				- __Store				要素格納コンテナ
	 */
	template<
		typename __Value,
		typename __Key = __Value,
		class __Store = std::vector<__Value >>
		class Stack {
		public:
			/*!
			 * @brief				コンストラクタ
			 */
			Stack() : size_(0) {};

			/*!
			 * @brief				デストラクタ
			 */
			virtual ~Stack() = default;

			using value_type = __Value;
			using key_type = __Key;
			using size_type = value_type;
			using iterator = typename __Store::iterator;
			using const_iterator = typename __Store::const_iterator;

			/*!
			 * @brief			先頭イテレータを返す
			 * @return			iterator	先頭イテレータを返す
			 * @detail			スタックの一番上の要素のイテレータを返す
			 */
			inline iterator begin()
			{
				return this->store_.begin();
			}

			/*!
			 * @brief			末尾の次のイテレータを返す
			 * @return			iterator	末尾の次のイテレータを返す
			 * @detail			スタックの一番下の要素の次のイテレータを返す
			 */
			inline iterator end()
			{
				return this->store_.begin() + this->size();
			}

			/*!
			 * @brief			読み取り専用先頭イテレータを返す
			 * @return			iterator	読み取り専用先頭イテレータを返す
			 * @detail			スタックの一番上の要素の読み取り専用イテレータを返す
			 */
			constexpr const_iterator begin() const
			{
				return this->store_.begin();
			}

			/*!
			 * @brief			読み取り専用末尾の次のイテレータを返す
			 * @return			iterator	読み取り専用末尾イテレータを返す
			 * @detail			スタックの一番下の要素の次のイテレータを返す
			 */
			constexpr const_iterator end() const
			{
				return this->store_.begin() + this->size();
			}

			/*!
			 * @brief			読み取り専用先頭イテレータを返す
			 * @return			iterator	読み取り専用先頭イテレータを返す
			 * @detail			スタックの一番上の要素の読み取り専用イテレータを返す
			 */
			constexpr const_iterator cbegin() const
			{
				return this->store_.cbegin();
			}

			/*!
			 * @brief			読み取り専用末尾の次のイテレータを返す
			 * @return			iterator	読み取り専用末尾イテレータを返す
			 * @detail			スタックの一番下の要素の次のイテレータを返す
			 */
			constexpr const_iterator cend() const
			{
				return this->store_.cbegin() + this->size();
			}

			/*!
			 * @brief			全要素初期化
			 * @detail			全要素を初期化処理を行う
			 *					メモリ解放は行わない
			 */
			inline void clear()
			{
				this->size_ = 0;
			}

			/*!
			 * @brief			スタックが空か判定
			 * @return			bool	要素数0ならTrue　空じゃないならFalse
			 * @detail			スタックが空か判定を行う
			 */
			constexpr bool isEmpty() const
			{
				return this->size() == 0;
			}

			/*!
			 * @brief			要素数を返す
			 * @return			size_type	要素数を返す
			 * @detail			スタック内の要素数を返す
			 */
			constexpr size_type size() const
			{
				return this->size_;
			}

			/*!
			 * @brief			格納可能要素数（容量）を返す
			 * @return			auto	格納可能要素数（容量）返す
			 * @detail			スタックの容量を返す
			 */
			constexpr size_type capacity() const
			{
				return this->store_.size();
			}

			/*!
			 * @brief			格納可能要素数（容量）を設定
			 * @param[in]		__capacity 容量
			 * @detail			スタックの容量を設定する
			 */
			inline void reserve(const size_type __capacity)
			{
				this->store_.resize(__capacity);
			}

			/*!
			 * @brief			スタックの先頭に要素を追加
			 * @param[in]		__value 要素
			 * @detail			スタックの先頭に要素を追加する
			 * 					- 要素型value_typeはテンプレート__Valueによって定義される
			 */
			inline void push(const value_type __value)
			{
				this->store_[this->size()] = __value;
				++this->size_;
			}

			/*!
			 * @brief			スタックの先頭の要素を削除
			 * @detail			スタックの先頭の要素を削除する
			 * @return			value_type	削除される要素を返す
			 * 					- 要素型value_typeはテンプレート__Valueによって定義される
			 */
			inline value_type pop()
			{
				assert(0 < this->size_);
				--this->size_;
				return this->store_[this->size()];
			}

			/*!
			 * @brief			スタックの先頭の要素を取得
			 * @return			value_type	削除される要素を返す
			 * @detail			スタックの先頭の要素を取得する
			 * 					- 要素型value_typeはテンプレート__Valueによって定義される
			 */
			constexpr value_type& top() const
			{
				return this->store_[this->size() - 1];
			}

			/*!
			 * @brief			要素をソートする
			 * @detail			要素を昇順にソートする
			 */
			inline void sort()
			{
				std::sort(this->begin(), this->end());
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
			}

			/*!
			 * @brief			指定のインデックスに入っている要素を取得
			 * @detail			指定のインデックスに入っている要素を取得する
			 * @param[in]		__index 格納番号
			 * @return			value_type&	指定要素の参照を返す
			 * 					- 要素型value_typeはテンプレート__Valueによって定義される
			 */
			const value_type& operator[](const key_type __index) const&
			{
				return this->store_[__index];
			}

			/*!
			 * @brief			指定のインデックスに入っている要素を取得
			 * @detail			指定のインデックスに入っている要素を取得する
			 * @param[in]		__index 格納番号
			 * @return			value_type	指定要素を返す
			 * 					- 要素型value_typeはテンプレート__Valueによって定義される
			 */
			inline value_type operator[](const key_type __index)&&
			{
				return std::move(this->store_[__index]);
			}

		private:
			//!サイズ変数
			size_type size_;

			//!データストア配列
			__Store store_;
	};
} /* namespace okl */

//#include "iterator.hpp"