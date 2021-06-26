#pragma once

#include "ConcatIteratorBase.hpp"

namespace okl {
	/*!
	 *  @brief     結合コンテナイテレータクラス
	 *  @details      結合コンテナイテレータクラス
	 *				- __Iterator				イテレータ型
	 *				- __Value					要素型
	 */
	template<typename __Iterator, typename __Value>
	class ConcatIterator : public ConcatIteratorBase<__Iterator, __Value>
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		ConcatIterator() = delete;
		using ref_iterator_type = __Iterator;

		ConcatIterator(ref_iterator_type __begin, ref_iterator_type __end, ref_iterator_type __next_begin) noexcept :ConcatIteratorBase<__Iterator, __Value>(__begin, __end, __next_begin) {}
		ConcatIterator(const ConcatIterator&) = default;
		ConcatIterator(ConcatIterator&&) = default;
		ConcatIterator& operator=(const ConcatIterator&) = default;
		ConcatIterator& operator=(ConcatIterator&&) = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~ConcatIterator() = default;

		using iterator_category = typename std::iterator_traits<ref_iterator_type>::iterator_category;
		using value_type = __Value;
		using difference_type = std::ptrdiff_t;
		using pointer = value_type*;
		using reference = value_type&;

		inline value_type& operator*() noexcept { return *(this->it_); }
		constexpr value_type operator*() const noexcept { return *(this->it_); }
	};
} /* namespace okl */