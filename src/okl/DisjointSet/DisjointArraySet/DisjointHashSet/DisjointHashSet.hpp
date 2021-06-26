#pragma once

#include "../DisjointArraySet.hpp"
#include "../../../HashSet/module.hpp"

namespace okl {
	/*!
	 *  @brief      素集合データ構造
	 *  @details    配列と集合で実装した素集合データ構造クラス
	 *				- __Id				識別番号数値型
	 *				- __IdContainer		ID格納配列
	 *				- __Set				集合クラス
	 *				- __Sets			集合格納配列
	 */
	template<
		class __Id,
		class __IdContainer = std::vector<__Id >,
		class __Set = HashSet<__Id>,
		class __Sets = std::vector<__Set> >
		class DisjointHashSet : public DisjointArraySet<__Id, __IdContainer >
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		DisjointHashSet() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~DisjointHashSet() = default;

		using id_type = typename DisjointArraySet<__Id, __IdContainer >::id_type;
		using element_type = typename DisjointArraySet<__Id, __IdContainer >::element_type;
		using size_type = typename DisjointArraySet<__Id, __IdContainer >::size_type;
		using set_type = __Set;

		/*!
		 * @brief			指定した集合に所属している要素集合を取得
		 * @param[in]		__set_id 集合ID
		 * @return			set_type&	指定した集合に所属している要素集合の参照を返す
		 * @detail			指定した集合に所属している要素集合を取得する
		 */
		inline const set_type& set(const id_type __set_id) const
		{
			return this->sets_[__set_id];
		}

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 *					メモリ解放は行わない
		 */
		inline void clear() override final
		{
			DisjointArraySet<__Id, __IdContainer >::clear();
			std::for_each(this->sets_.begin(), this->sets_.end(), [](auto& _set) { _set.clear(); });
		}

		/*!
		 * @brief			格納可能要素数（容量）を設定
		 * @param[in]		__capacity 容量
		 * @detail			集合の容量を設定する
		 */
		inline void reserve(const size_type __capacity) override final
		{
			DisjointArraySet<__Id, __IdContainer >::reserve(__capacity);
			this->sets_.resize(__capacity);
			std::for_each(this->sets_.begin(), this->sets_.end(), [__capacity](auto& _set) { _set.reserve(__capacity); });
		}

		/*!
		 * @brief			所属ID修正の要素数を返す
		 * @param[in]		__set_id 集合のID
		 * @detail			所属ID修正の要素数を返す
		 */
		inline virtual id_type set_size(const id_type __set_id) override
		{
			return this->sets_[__set_id].size();
		}

		/*!
		 * @brief			所属ID集合aと所属ID集合bをマージ
		 * @param[in]		__set_id_a 集合aのID
		 * @param[in]		__set_id_b 集合bのID
		 * @detail			所属ID集合aに所属ID集合bを全て移動しのマージする
		 * 					- 要素型id_typeはテンプレート__Idによって定義される
		 */
		inline void union_set(const id_type __set_id_a, const id_type __set_id_b) override final
		{
			DisjointArraySet<__Id, __IdContainer >::union_set(__set_id_a, __set_id_b);
		}

		/*!
		 * @brief			指定された集合に要素を所属
		 * @param[in]		__element 要素
		 * @param[in]		__set_id 所属する集合のID
		 * @detail			指定された集合に要素を所属する
		 * 					- 要素型element_typeはテンプレート__Idによって定義される
		 * 					- 要素型id_typeはテンプレート__Idによって定義される
		 */
		inline void moveTo(const element_type __element, const id_type __set_id) override final
		{
			if (this->find(__element) != this->undefine_id()) {
				this->sets_[this->find(__element)].erase(__element);
			}
			this->sets_[__set_id].insert(__element);
			DisjointArraySet<__Id, __IdContainer >::moveTo(__element, __set_id);
		}

		/*!
		 * @brief			要素を現在より所属IDが+1の集合に所属
		 * @param[in]		__element 要素
		 * @detail			要素を現在より所属IDが+1の集合に所属
		 * 					- 要素型element_typeはテンプレート__Idによって定義される
		 */
		inline void moveToInc(const element_type __element) override final
		{
			this->sets_[this->find(__element)].erase(__element);
			DisjointArraySet<__Id, __IdContainer >::moveToInc(__element);
			this->sets_[this->find(__element)].insert(__element);
		}

		/*!
		 * @brief			要素を現在より所属IDが-1の集合に所属
		 * @param[in]		__element 要素
		 * @detail			要素を現在より所属IDが-1の集合に所属
		 * 					- 要素型element_typeはテンプレート__Idによって定義される
		 */
		inline void moveToDec(const element_type __element) override final
		{
			this->sets_[this->find(__element)].erase(__element);
			DisjointArraySet<__Id, __IdContainer >::moveToDec(__element);
			this->sets_[this->find(__element)].insert(__element);
		}

		/*!
		 * @brief			全ての要素を指定された集合に所属
		 * @param[in]		__set_id 所属する集合のID
		 * @detail			全ての要素を指定された集合に所属
		 * 					- 要素型id_typeはテンプレート__Idによって定義される
		 */
		inline void moveToAll(const id_type __set_id) override final
		{
			DisjointArraySet<__Id, __IdContainer >::moveToAll(__set_id);
			std::for_each(this->sets_.begin(), this->sets_.end(), [](auto& _set) { _set.clear(); });

			for (id_type _id = 0; _id < this->sets_.size(); ++_id) {
				this->sets_[__set_id].insert(_id);
			}
		}

		/*!
		 * @brief			指定の集合に所属する要素を全て未所属にする
		 * @param[in]		__set_id 所属する集合のID
		 * @detail			指定の集合に所属する要素を全て未所属にする
		 * 					- 要素型id_typeはテンプレート__Idによって定義される
		 */
		inline void clearSet(const id_type __set_id) override final
		{
			std::for_each(this->set(__set_id).begin(), this->set(__set_id).end(), [this](const auto _element) { DisjointArraySet<__Id, __IdContainer >::out(_element); });
			this->sets_[__set_id].clear();
		}

		/*!
		 * @brief			指定の要素を未所属にする
		 * @param[in]		__element 要素
		 * @detail			指定の集合に所属する要素を全て未所属にする
		 * 					- 要素型element_typeはテンプレート__Idによって定義される
		 */
		inline void out(const element_type __element) override final
		{
			this->sets_[this->find(__element)].erase(__element);
			DisjointArraySet<__Id, __IdContainer >::out(__element);
		}

	private:
		//!集合配列コンテナ
		__Sets sets_;
	};
} /* namespace okl */