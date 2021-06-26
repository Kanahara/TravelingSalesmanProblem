#pragma once

#include "ConcatIteratorBase.hpp"

namespace okl {
	/*!
	 *  @brief     結合コンテナコンストイテレータクラス
	 *  @details      結合コンテナコンストイテレータクラス
	 *				- __Iterator				イテレータ型
	 *				- __Value					要素型
	 */
	template<typename __Iterator, typename __Value>
	class ConcatConstIterator : public ConcatIteratorBase<__Iterator, __Value>
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		ConcatConstIterator() = delete;
		using ref_iterator_type = __Iterator;

		ConcatConstIterator(ref_iterator_type __begin, ref_iterator_type __end, ref_iterator_type __next_begin) noexcept : ConcatIteratorBase<__Iterator, __Value>(__begin, __end, __next_begin) {}
		ConcatConstIterator(const ConcatConstIterator&) = default;
		ConcatConstIterator(ConcatConstIterator&&) = default;
		ConcatConstIterator& operator=(const ConcatConstIterator&) = default;
		ConcatConstIterator& operator=(ConcatConstIterator&&) = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~ConcatConstIterator() = default;

		using iterator_category = typename std::iterator_traits<ref_iterator_type>::iterator_category;
		using value_type = __Value;
		using difference_type = std::ptrdiff_t;
		using pointer = value_type*;
		using reference = value_type&;

		inline const value_type& operator*() noexcept { return *(this->it_); }
		constexpr value_type operator*() const noexcept { return *(this->it_); }
	};
} /* namespace okl */