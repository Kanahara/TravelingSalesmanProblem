#pragma once
#include <cstdint>

namespace okl {
	/*!
	 *  @brief      素集合データ構造基底クラス
	 *  @details    素集合データ構造基底クラスインターフェースとしての定義
	 *				- __Id				識別番号数値型
	 */
	template<typename __Id>
	class DisjointSet {
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		DisjointSet() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~DisjointSet() = default;

		using id_type = __Id;
		using element_type = __Id;
		using size_type = id_type;

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 */
		virtual void clear() = 0;

		/*!
		 * @brief			格納可能要素数（容量）を設定
		 * @param[in]		__capacity 容量
		 * @detail			集合の容量を設定する
		 */
		virtual void reserve(const size_type __capacity) = 0;

		/*!
		 * @brief			属している所属集合ID
		 * @param[in]		__element 要素
		 * @return			id_type	属している所属集合IDを返す
		 * @detail			要素がどの集合に属しているか所属IDを返す
		 * 					- 要素型id_typeはテンプレート__Idによって定義される
		 * 					- 要素型element_typeはテンプレート__Idによって定義される
		 */
		virtual id_type find(const element_type __element) const = 0;

		/*!
		 * @brief			所属ID集合aと所属ID集合bをマージ
		 * @param[in]		__set_id_a 集合aのID
		 * @param[in]		__set_id_b 集合bのID
		 * @detail			所属ID集合aに所属ID集合bを全て移動しのマージする
		 * 					- 要素型id_typeはテンプレート__Idによって定義される
		 */
		virtual void union_set(const id_type __set_id_a, const id_type __set_id_b) = 0;

		/*!
		 * @brief			指定された集合に要素を所属
		 * @param[in]		__element 要素
		 * @param[in]		__set_id 所属する集合のID
		 * @detail			指定された集合に要素を所属する
		 * 					- 要素型element_typeはテンプレート__Idによって定義される
		 * 					- 要素型id_typeはテンプレート__Idによって定義される
		 */
		virtual void moveTo(const element_type __element, const id_type __set_id) = 0;

		/*!
		 * @brief			要素を現在より所属IDが+1の集合に所属
		 * @param[in]		__element 要素
		 * @detail			要素を現在より所属IDが+1の集合に所属
		 * 					- 要素型element_typeはテンプレート__Idによって定義される
		 */
		virtual void moveToInc(const element_type __element) = 0;

		/*!
		 * @brief			要素を現在より所属IDが-1の集合に所属
		 * @param[in]		__element 要素
		 * @detail			要素を現在より所属IDが-1の集合に所属
		 * 					- 要素型element_typeはテンプレート__Idによって定義される
		 */
		virtual void moveToDec(const element_type __element) = 0;

		/*!
		 * @brief			全ての要素を指定された集合に所属
		 * @param[in]		__set_id 所属する集合のID
		 * @detail			全ての要素を指定された集合に所属
		 * 					- 要素型id_typeはテンプレート__Idによって定義される
		 */
		virtual void moveToAll(const id_type __set_id) = 0;
	};
} /* namespace okl */