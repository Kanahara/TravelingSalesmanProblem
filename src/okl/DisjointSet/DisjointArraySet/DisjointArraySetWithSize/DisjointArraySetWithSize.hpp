#pragma once
#include <vector>
#include <algorithm>
#include "../DisjointArraySet.hpp"

namespace okl {
	/*!
	 *  @brief      素集合データ構造
	 *  @details    配列で実装した素集合データ構造クラス
	 *				- __Id				識別番号数値型
	 *				- __IdContainer		ID格納配列
	 */
	template<
		typename __Id,
		class __IdContainer = std::vector<__Id >>
		class DisjointArraySetWithSize : public DisjointArraySet<__Id, __IdContainer> {
		public:
			/*!
			 * @brief				コンストラクタ
			 */
			DisjointArraySetWithSize() = default;

			/*!
			 * @brief				デストラクタ
			 */
			virtual ~DisjointArraySetWithSize() = default;

			using id_type = typename DisjointArraySet<__Id, __IdContainer>::id_type;
			using element_type = typename DisjointArraySet<__Id, __IdContainer>::element_type;
			using size_type = typename DisjointArraySet<__Id, __IdContainer>::size_type;

			/*!
			 * @brief			全要素初期化
			 * @detail			全要素を初期化処理を行う
			 *					メモリ解放は行わない
			 */
			inline virtual void clear() override
			{
				DisjointArraySet<__Id, __IdContainer>::clear();
				std::fill(this->sizes_.begin(), this->sizes_.end(), 0);
			}

			/*!
			 * @brief			格納可能要素数（容量）を設定
			 * @param[in]		__capacity 容量
			 * @detail			集合の容量を設定する
			 */
			inline virtual void reserve(const size_type __capacity) override
			{
				DisjointArraySet<__Id, __IdContainer>::reserve(__capacity);
				this->sizes_.resize(__capacity, 0);
			}

			/*!
			 * @brief			格納可能要素数（容量）を設定
			 * @param[in]		__capacity 容量
			 * @detail			集合の容量を設定する
			 */
			inline virtual id_type set_size(const id_type __set_id) override
			{
				return this->sizes_[__set_id];
			}

			/*!
			 * @brief			所属ID集合aと所属ID集合bをマージ
			 * @param[in]		__set_id_a 集合aのID
			 * @param[in]		__set_id_b 集合bのID
			 * @detail			所属ID集合aに所属ID集合bを全て移動しのマージする
			 * 					- 要素型id_typeはテンプレート__Idによって定義される
			 */
			inline virtual void union_set(const id_type __set_id_a, const id_type __set_id_b) override
			{
				DisjointArraySet<__Id, __IdContainer>::union_set(__set_id_a, __set_id_b);
				this->sizes_[__set_id_a] = static_cast<id_type>(this->sizes_[__set_id_a] + this->sizes_[__set_id_b]);
				this->sizes_[__set_id_b] = 0;
			}

			/*!
			 * @brief			指定された集合に要素を所属
			 * @param[in]		__element 要素
			 * @param[in]		__set_id 所属する集合のID
			 * @detail			指定された集合に要素を所属する
			 * 					- 要素型element_typeはテンプレート__Idによって定義される
			 * 					- 要素型id_typeはテンプレート__Idによって定義される
			 */
			inline virtual void moveTo(const element_type __element, const id_type __set_id) override
			{
				if (this->find(__element) != this->undefine_id()) {
					--this->sizes_[this->find(__element)];
				}
				DisjointArraySet<__Id, __IdContainer>::moveTo(__element, __set_id);
				++this->sizes_[__set_id];
			}

			/*!
			 * @brief			要素を現在より所属IDが+1の集合に所属
			 * @param[in]		__element 要素
			 * @detail			要素を現在より所属IDが+1の集合に所属
			 * 					- 要素型element_typeはテンプレート__Idによって定義される
			 */
			inline virtual void moveToInc(const element_type __element) override
			{
				if (this->find(__element) != this->undefine_id()) {
					--this->sizes_[this->find(__element)];
				}
				DisjointArraySet<__Id, __IdContainer>::moveToInc(__element);
				++this->sizes_[this->find(__element)];
			}

			/*!
			 * @brief			要素を現在より所属IDが-1の集合に所属
			 * @param[in]		__element 要素
			 * @detail			要素を現在より所属IDが-1の集合に所属
			 * 					- 要素型element_typeはテンプレート__Idによって定義される
			 */
			inline virtual void moveToDec(const element_type __element) override
			{
				if (this->find(__element) != this->undefine_id()) {
					--this->sizes_[this->find(__element)];
				}
				DisjointArraySet<__Id, __IdContainer>::moveToDec(__element);
				++this->sizes_[this->find(__element)];
			}

			/*!
			 * @brief			全ての要素を指定された集合に所属
			 * @param[in]		__set_id 所属する集合のID
			 * @detail			全ての要素を指定された集合に所属
			 * 					- 要素型id_typeはテンプレート__Idによって定義される
			 */
			inline virtual void moveToAll(const id_type __set_id) override
			{
				DisjointArraySet<__Id, __IdContainer>::moveToAll(__set_id);
				std::fill(this->sizes_.begin(), this->sizes_.end(), 0);
				this->sizes_[__set_id] = static_cast<id_type>(this->id_container_.size());
			}

			/*!
			 * @brief			指定の集合に所属する要素を全て未所属にする
			 * @param[in]		__set_id 所属する集合のID
			 * @detail			指定の集合に所属する要素を全て未所属にする
			 * 					- 要素型id_typeはテンプレート__Idによって定義される
			 */
			inline virtual void clearSet(const id_type __set_id)
			{
				DisjointArraySet<__Id, __IdContainer>::clearSet(__set_id);
				this->sizes_[__set_id] = 0;
			}

			/*!
			 * @brief			指定の要素を全て未所属にする
			 * @param[in]		__element 要素
			 * @detail			指定の集合に所属する要素を全て未所属にする
			 * 					- 要素型element_typeはテンプレート__Idによって定義される
			 */
			inline virtual void out(const element_type __element)
			{
				--this->sizes_[this->find(__element)];
				DisjointArraySet<__Id, __IdContainer>::out(__element);				
			}

		private:
			//!各所属の要素数
			__IdContainer sizes_;
	};
} /* namespace okl */