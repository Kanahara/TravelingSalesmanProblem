#pragma once

#include <utility>

namespace okl {
	/*!
	 *  @brief     結合コンテナイテレータベースクラス
	 *  @details      結合コンテナイテレータベースクラス
	 *				- __Iterator				イテレータ型
	 *				- __Value					要素型
	 */
	template<typename __Iterator, typename __Value>
	class ConcatIteratorBase
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		ConcatIteratorBase() = delete;
		using ref_iterator_type = __Iterator;

		ConcatIteratorBase(ref_iterator_type __begin, ref_iterator_type __end, ref_iterator_type __next_begin) noexcept : it_(__begin), first_end_(__end), next_begin_(__next_begin) { if (__begin == __end) { this->it_ = __next_begin; } }
		ConcatIteratorBase(const ConcatIteratorBase&) = default;
		ConcatIteratorBase(ConcatIteratorBase&&) = default;
		ConcatIteratorBase& operator=(const ConcatIteratorBase&) = default;
		ConcatIteratorBase& operator=(ConcatIteratorBase&&) = default;
		ref_iterator_type get_raw_iterator() const { return it_; }
		ref_iterator_type begin_iterator() const { return it_; }
		ref_iterator_type next_begin_iterator() const { return next_begin_; }

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~ConcatIteratorBase() = default;

		using iterator_category = typename std::iterator_traits<ref_iterator_type>::iterator_category;
		using value_type = __Value;
		using difference_type = std::ptrdiff_t;
		using pointer = value_type*;
		using reference = value_type&;

		template<std::enable_if_t<std::is_base_of<std::input_iterator_tag, iterator_category>::value, std::nullptr_t> = nullptr>
		inline value_type* operator->() noexcept { return &(*it_); }
		template<std::enable_if_t<std::is_base_of<std::input_iterator_tag, iterator_category>::value, std::nullptr_t> = nullptr>
		inline const value_type* operator->() const noexcept { return &(*it_); }
		inline ConcatIteratorBase& operator++() noexcept {
			++this->it_;
			if (this->it_ == this->first_end_) {
				this->it_ = this->next_begin_;
			}
			return *this;
		}
		inline ConcatIteratorBase operator++(int) noexcept {
			const auto tmp = *this;
			++* this;
			return tmp;
		}
		template<std::enable_if_t<std::is_base_of<std::bidirectional_iterator_tag, iterator_category>::value, std::nullptr_t> = nullptr>
		inline ConcatIteratorBase& operator--() noexcept {
			if (this->it_ == this->next_begin_) {
				this->it_ = this->first_end_;
			}
			--this->it_;
			return *this;
		}
		template<std::enable_if_t<std::is_base_of<std::bidirectional_iterator_tag, iterator_category>::value, std::nullptr_t> = nullptr>
		inline ConcatIteratorBase operator--(int) noexcept {
			const auto tmp = *this;
			--* this;
			return tmp;
		}

		template<std::enable_if_t<std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<ref_iterator_type>::iterator_category>::value, std::nullptr_t> = nullptr>
		constexpr bool operator==(const ConcatIteratorBase& __other) const { return this->it_ == __other.get_raw_iterator(); }
		template<std::enable_if_t<std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<ref_iterator_type>::iterator_category>::value, std::nullptr_t> = nullptr>
		constexpr bool operator!=(const ConcatIteratorBase& __other) const { return !(*this == __other); }
		template<std::enable_if_t<std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<ref_iterator_type>::iterator_category>::value, std::nullptr_t> = nullptr>
		constexpr bool operator<(const ConcatIteratorBase& __other) const { return this->it_ < __other.get_raw_iterator(); }
		template<std::enable_if_t<std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<ref_iterator_type>::iterator_category>::value, std::nullptr_t> = nullptr>
		constexpr bool operator>(const ConcatIteratorBase& __other) const { return __other < *this; }
		template<std::enable_if_t<std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<ref_iterator_type>::iterator_category>::value, std::nullptr_t> = nullptr>
		constexpr bool operator<=(const ConcatIteratorBase& __other) const { return !(*this > __other); }
		template<std::enable_if_t<std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<ref_iterator_type>::iterator_category>::value, std::nullptr_t> = nullptr>
		constexpr bool operator>=(const ConcatIteratorBase& __other) const { return !(*this < __other); }

		template<std::enable_if_t<std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<ref_iterator_type>::iterator_category>::value, std::nullptr_t> = nullptr>
		constexpr bool operator==(const ref_iterator_type& __other) const { return this->it_ == __other; }
		template<std::enable_if_t<std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<ref_iterator_type>::iterator_category>::value, std::nullptr_t> = nullptr>
		constexpr bool operator!=(const ref_iterator_type& __other) const { return !(*this == __other); }
		template<std::enable_if_t<std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<ref_iterator_type>::iterator_category>::value, std::nullptr_t> = nullptr>
		constexpr bool operator<(const ref_iterator_type& __other) const { return this->it_ < __other; }
		template<std::enable_if_t<std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<ref_iterator_type>::iterator_category>::value, std::nullptr_t> = nullptr>
		constexpr bool operator>(const ref_iterator_type& __other) const { return __other < *this; }
		template<std::enable_if_t<std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<ref_iterator_type>::iterator_category>::value, std::nullptr_t> = nullptr>
		constexpr bool operator<=(const ref_iterator_type& __other) const { return !(*this > __other); }
		template<std::enable_if_t<std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<ref_iterator_type>::iterator_category>::value, std::nullptr_t> = nullptr>
		constexpr bool operator>=(const ref_iterator_type& __other) const { return !(*this < __other); }

	protected:
		ref_iterator_type it_;

	private:
		ref_iterator_type first_end_;

		ref_iterator_type next_begin_;
	};
} /* namespace okl */