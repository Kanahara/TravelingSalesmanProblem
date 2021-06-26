#pragma once
#include <vector>
#include <algorithm>
#include "../DisjointSet.hpp"

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
		class DisjointArraySet : public DisjointSet<__Id > {
		public:
			/*!
			 * @brief				コンストラクタ
			 */
			DisjointArraySet() = default;

			/*!
			 * @brief				デストラクタ
			 */
			virtual ~DisjointArraySet() = default;

			using id_type = typename DisjointSet<__Id >::id_type;
			using element_type = typename DisjointSet<__Id >::element_type;
			using size_type = typename DisjointSet<__Id >::size_type;

			/*!
			 * @brief			全要素初期化
			 * @detail			全要素を初期化処理を行う
			 *					メモリ解放は行わない
			 */
			inline virtual void clear() override
			{
				std::fill(this->id_container_.begin(), this->id_container_.end(), this->undefine_id());
			}

			/*!
			 * @brief			格納可能要素数（容量）を設定
			 * @param[in]		__capacity 容量
			 * @detail			集合の容量を設定する
			 */
			inline virtual void reserve(const size_type __capacity) override
			{
				this->id_container_.resize(__capacity, this->undefine_id());
			}

			/*!
			 * @brief			集合から要素を移動
			 * @param[in]		__element 要素
			 * @return			id_type	属している所属集合IDを返す
			 * @detail			要素がどの集合に属しているか所属IDを返す
			 * 					- 要素型id_typeはテンプレート__Idによって定義される
			 * 					- 要素型element_typeはテンプレート__Idによって定義される
			 */
			inline virtual id_type find(const element_type __element) const override
			{
				return this->id_container_[__element];
			}

			/*!
			 * @brief			格納可能要素数（容量）を設定
			 * @param[in]		__capacity 容量
			 * @detail			集合の容量を設定する
			 */
			inline virtual id_type set_size(const id_type __set_id)
			{
				return static_cast<id_type>(std::count_if(this->id_container_.begin(), this->id_container_.end(),
					[this, __set_id](const auto  _element) { return this->find(_element) == __set_id; }
				));
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
				std::replace_if(this->id_container_.begin(), this->id_container_.end(),
					[this, __set_id_b](const auto  _element) { return this->find(_element) == __set_id_b; }, __set_id_a);
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
				this->id_container_[__element] = __set_id;
			}

			/*!
			 * @brief			要素を現在より所属IDが+1の集合に所属
			 * @param[in]		__element 要素
			 * @detail			要素を現在より所属IDが+1の集合に所属
			 * 					- 要素型element_typeはテンプレート__Idによって定義される
			 */
			inline virtual void moveToInc(const element_type __element) override
			{
				++this->id_container_[__element];
			}

			/*!
			 * @brief			要素を現在より所属IDが-1の集合に所属
			 * @param[in]		__element 要素
			 * @detail			要素を現在より所属IDが-1の集合に所属
			 * 					- 要素型element_typeはテンプレート__Idによって定義される
			 */
			inline virtual void moveToDec(const element_type __element) override
			{
				--this->id_container_[__element];
			}

			/*!
			 * @brief			全ての要素を指定された集合に所属
			 * @param[in]		__set_id 所属する集合のID
			 * @detail			全ての要素を指定された集合に所属
			 * 					- 要素型id_typeはテンプレート__Idによって定義される
			 */
			inline virtual void moveToAll(const id_type __set_id) override
			{
				std::fill(this->id_container_.begin(), this->id_container_.end(), __set_id);
			}

			/*!
			 * @brief			指定の集合に所属する要素を全て未所属にする
			 * @param[in]		__set_id 所属する集合のID
			 * @detail			指定の集合に所属する要素を全て未所属にする
			 * 					- 要素型id_typeはテンプレート__Idによって定義される
			 */
			inline virtual void clearSet(const id_type __set_id)
			{
				std::replace_if(this->id_container_.begin(), this->id_container_.end(),
					[this, __set_id](const auto _element) { return this->find(_element) == __set_id; }, this->undefine_id());
			}

			/*!
			 * @brief			指定の要素を全て未所属にする
			 * @param[in]		__element 要素
			 * @detail			指定の集合に所属する要素を全て未所属にする
			 * 					- 要素型element_typeはテンプレート__Idによって定義される
			 */
			inline virtual void out(const element_type __element)
			{
				this->id_container_[__element] = this->undefine_id();
			}

			/*!
			 * @brief			未定義ID
			 * @detail			未定義IDを返す
			 * @return			id_type	未定義IDとしてid_typeの最大値を返す
			 */
			constexpr id_type undefine_id() const
			{
				return std::numeric_limits < id_type > ::max();
			}

		protected:
			//!各要素が所属しているIDコンテナ
			__IdContainer id_container_;
	};
} /* namespace okl */