#pragma once

namespace okl {
	template<typename __Iterator>
	class Span {
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		Span(const __Iterator __begin, const __Iterator __end)
			: begin_(__begin), end_(__end) {}

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~Span() = default;

		using iterator = __Iterator;
		using const_iterator = const iterator;
		using size_type = typename iterator::difference_type;
		using value_type = typename iterator::value_type;
		using key_type = size_type;

		/*!
		 * @brief			先頭イテレータを返す
		 * @return			iterator	先頭イテレータを返す
		 * @detail			先頭の要素のイテレータを返す
		 */
		iterator begin()
		{
			return this->begin_;
		}

		/*!
		 * @brief			末尾の次のイテレータを返す
		 * @return			iterator	末尾の次のイテレータを返す
		 * @detail			末尾の要素の次のイテレータを返す
		 */
		iterator end()
		{
			return this->end_;
		}

		/*!
		 * @brief			末尾のイテレータを返す
		 * @return			iterator	末尾のイテレータを返す
		 * @detail			末尾の要素のイテレータを返す
		 */
		iterator back()
		{
			return this->end() - 1;
		}

		/*!
		 * @brief			読み取り専用先頭イテレータを返す
		 * @return			iterator	読み取り専用先頭イテレータを返す
		 * @detail			先頭の要素の読み取り専用イテレータを返す
		 */
		constexpr const_iterator begin() const
		{
			return this->begin_;
		}

		/*!
		 * @brief			読み取り専用末尾の次のイテレータを返す
		 * @return			iterator	読み取り専用末尾の次イテレータを返す
		 * @detail			末尾の要素の次のイテレータを返す
		 */
		constexpr const_iterator end() const
		{
			return this->end_;
		}

		/*!
		 * @brief			末尾のイテレータを返す
		 * @return			iterator	末尾のイテレータを返す
		 * @detail			末尾の要素のイテレータを返す
		 */
		constexpr const_iterator back() const
		{
			return this->end() - 1;
		}

		/*!
		 * @brief			読み取り専用先頭イテレータを返す
		 * @return			iterator	読み取り専用先頭イテレータを返す
		 * @detail			先頭の要素の読み取り専用イテレータを返す
		 */
		constexpr const_iterator cbegin() const
		{
			return this->begin();
		}

		/*!
		 * @brief			読み取り専用末尾の次のイテレータを返す
		 * @return			iterator	読み取り専用末尾の次イテレータを返す
		 * @detail			末尾の要素の次のイテレータを返す
		 */
		constexpr const_iterator cend() const
		{
			return this->end();
		}

		/*!
		 * @brief			先頭イテレータを設定
		 * @param[in]		__begin 先頭イテレータ
		 * @detail			先頭イテレータを設定する
		 * 					- 整数型iteratorはテンプレート__Iteratorによって定義される
		 */
		inline void setBegin(iterator __begin)
		{
			this->begin_ = __begin;
		}

		/*!
		 * @brief			末尾の次イテレータを設定
		 * @param[in]		__end 末尾の次イテレータ
		 * @detail			末尾の次イテレータを設定する
		 * 					- 整数型iteratorはテンプレート__Iteratorによって定義される
		 */
		inline void setEnd(iterator __end)
		{
			this->end_ = __end;
		}

		/*!
		 * @brief			空集合か判定
		 * @return			bool	要素数0ならTrue　空じゃないならFalse
		 * @detail			空集合か判定を行う
		 */
		constexpr bool isEmpty() const
		{
			return this->size() == 0;
		}

		/*!
		 * @brief			要素数を返す
		 * @return			size_type	要素数を返す
		 * @detail			集合内の要素数を返す
		 */
		constexpr size_type size() const
		{
			return std::distance(this->begin_, this->end_);
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
			assert(__index < this->size());
			return *(this->begin_ + __index);
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
			return std::move(*(this->begin_ + __index));
		}

	private:
		//!先頭イテレータ
		iterator begin_;

		//!末尾の次のイテレータ
		iterator end_;
	};
} /* namespace okl */