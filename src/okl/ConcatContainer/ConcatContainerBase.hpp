#pragma once

#include <utility>
#include "ConcatIterator.hpp"
#include "ConcatConstIterator.hpp"

namespace okl {
	/*!
	 *  @brief      結合コンテナベースクラス
	 *  @details      結合コンテナベースクラス
	 *				- __Iterator				イテレータ型
	 *				- __Value					要素型
	 */
	template<typename __Iterator, typename __Value>
	class ConcatContainerBase
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		ConcatContainerBase() = delete;

		using container_iterator = __Iterator;
		using value_type = __Value;
		using iterator = ConcatIterator<container_iterator, value_type>;
		using const_iterator = ConcatConstIterator<container_iterator, value_type>;
		using key_type = std::size_t;

		ConcatContainerBase(container_iterator __first_begin, container_iterator __first_end, container_iterator __second_begin, container_iterator __second_end) noexcept
			: end_(__second_end), first_size_(std::distance(__first_begin, __first_end)), second_size_(std::distance(__second_begin, __second_end)) {};
		ConcatContainerBase(const ConcatContainerBase&) = default;
		ConcatContainerBase(ConcatContainerBase&&) = default;
		ConcatContainerBase& operator=(const ConcatContainerBase&) = default;
		ConcatContainerBase& operator=(ConcatContainerBase&&) = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~ConcatContainerBase() = default;

		/*!
		 * @brief			読み取り専用末尾の次のイテレータを返す
		 * @return			iterator	読み取り専用末尾の次イテレータを返す
		 * @detail			末尾の要素の次のイテレータを返す
		 */
		inline container_iterator end() const {
			return this->end_;
		}

		/*!
		 * @brief			読み取り専用末尾の次のイテレータを返す
		 * @return			iterator	読み取り専用末尾の次イテレータを返す
		 * @detail			末尾の要素の次のイテレータを返す
		 */
		inline container_iterator cend() const {
			return this->end_;
		}

		/*!
		 * @brief			要素数が0か判定
		 * @return			bool	要素数0ならTrue　0じゃないならFalse
		 * @detail			要素数が0か判定を行う
		 */
		constexpr bool isEmpty() const {
			return this->size() == 0;
		}

		/*!
		 * @brief			要素数を返す
		 * @return			size_type	要素数を返す
		 * @detail			要素数を返す
		 */
		constexpr std::size_t size() const {
			return this->first_size_ + this->second_size_;
		}

	protected:
		//!末尾の次のイテレータ
		container_iterator end_;

		//!コンテナサイズ変数
		std::size_t first_size_;

		//!コンテナサイズ変数
		std::size_t second_size_;
	};
} /* namespace okl */