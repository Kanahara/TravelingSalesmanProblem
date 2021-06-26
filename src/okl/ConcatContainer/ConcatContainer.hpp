#pragma once

#include "ConcatContainerBase.hpp"

namespace okl {
	/*!
	 *  @brief      結合コンテナクラス
	 *  @details      結合コンテナクラス
	 *				- __Iterator				イテレータ型
	 *				- __Value					要素型
	 */
	template<typename __Iterator, typename __Value>
	class ConcatContainer : public ConcatContainerBase<__Iterator, __Value>
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		ConcatContainer() = delete;

		using container_iterator = typename ConcatContainerBase<__Iterator, __Value>::container_iterator;
		using value_type = typename ConcatContainerBase<__Iterator, __Value>::value_type;
		using iterator = typename ConcatContainerBase<__Iterator, __Value>::iterator;
		using const_iterator = typename ConcatContainerBase<__Iterator, __Value>::const_iterator;
		using key_type = typename ConcatContainerBase<__Iterator, __Value>::key_type;

		ConcatContainer(container_iterator __first_begin, container_iterator __first_end, container_iterator __second_begin, container_iterator __second_end) noexcept
			:ConcatContainerBase<__Iterator, __Value>(__first_begin, __first_end, __second_begin, __second_end), begin_(__first_begin, __first_end, __second_begin) {};
		ConcatContainer(const ConcatContainer&) = default;
		ConcatContainer(ConcatContainer&&) = default;
		ConcatContainer& operator=(const ConcatContainer&) = default;
		ConcatContainer& operator=(ConcatContainer&&) = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~ConcatContainer() = default;

		/*!
		 * @brief			読み取り専用先頭イテレータを返す
		 * @return			iterator	読み取り専用先頭イテレータを返す
		 * @detail			先頭の要素の読み取り専用イテレータを返す
		 */
		inline iterator begin() {
			return this->begin_;
		}

		/*!
		 * @brief			指定のインデックスに入っている要素を取得
		 * @detail			指定のインデックスに入っている要素を取得する
		 * @param[in]		__index 格納番号
		 * @return			value_type&	指定要素の参照を返す
		 * 					- 要素型value_typeはテンプレート__Valueによって定義される
		 * 					- 要素型key_typeはstd::size_tによって定義される
		 */
		inline const value_type& operator[](const key_type __index) const& {
			if (this->first_size_ <= __index) {
				return *(this->begin_.next_begin_iterator() + (__index - this->first_size_));
			}
			else {
				return *(this->begin_.begin_iterator() + __index);
			}
		}

		/*!
		 * @brief			指定のインデックスに入っている要素を取得
		 * @detail			指定のインデックスに入っている要素を取得する
		 * @param[in]		__index 格納番号
		 * @return			value_type&	指定要素の参照を返す
		 * 					- 要素型value_typeはテンプレート__Valueによって定義される
		 * 					- 要素型key_typeはstd::size_tによって定義される
		 */
		inline value_type operator[](const key_type __index)&& {
			if (this->first_size_ <= __index) {
				return *(this->begin_.next_begin_iterator() + (__index - this->first_size_));
			}
			else {
				return *(this->begin_.begin_iterator() + __index);
			}
		}

	private:
		//!先頭イテレータ
		iterator begin_;
	};
} /* namespace okl */