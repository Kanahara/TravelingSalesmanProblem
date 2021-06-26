#pragma once

#include <numeric>
#include <vector>

namespace okl {
	/*!
	 *  @brief      有向辺クラス
	 *  @details      有向辺クラス
	 *				- __Id					頂点ID数値型
	 *				- __Container			ID配列
	 */
	template<
		class __Id = std::size_t,
		class __Container = std::vector<__Id>>
		class Edges
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		Edges() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~Edges() = default;

		using id_type = __Id;

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 *					メモリ解放は行わない
		 */
		inline void clear()
		{
			std::fill(this->container_.begin(), this->container_.end(), this->undefine_value());
		}

		/*!
		 * @brief			サイズ設定
		 * @param[in]		__size 頂点数
		 * @detail			全要素をメモリ確保を行う
		 */
		inline void resize(const std::size_t __size)
		{
			this->container_.resize(__size, this->undefine_value());
		}
		
		/*!
		 * @brief			開始頂点と隣接している終点頂点を取得
		 * @param[in]		__start 辺開始頂点のID
		 * @return			id_type	開始頂点と隣接している終点頂点IDを取得
		 * @detail			開始頂点と隣接している終点頂点を取得
		 */
		constexpr id_type get(const id_type __start) const
		{
			return this->container_[__start];
		}

		/*!
		 * @brief			有向辺を設定
		 * @param[in]		__start 辺開始頂点のID
		 * @param[in]		__end	辺終点頂点のID
		 * @detail			start->end有向辺を追加する
		 * 					- 整数型id_typeはテンプレート__Idによって定義される
		 */
		void set(const id_type __start, const id_type __end)
		{
			this->container_[__start] = __end;
		}

		/*!
		* @brief			代入処理を行う
		* @param[in]		__other 代入元対象
		* @return			Edges&	自分自身の参照を返す
		* @detail			自身の要素格納場所情報を削除し代入元の要素の情報を代入する
		*/
		inline Edges& operator=(const Edges& __other)
		{
			std::copy(__other.container_.begin(), __other.container_.end(), this->container_.begin());
			return *this;
		}

	protected:
		/*!
		 * @brief			未定義ID
		 * @detail			未定義IDを返す
		 * @return			id_type	未定義IDとしてid_typeの最大値を返す
		 */
		constexpr id_type undefine_value() const
		{
			return std::numeric_limits < id_type >::max();
		}

	private:
		//!ID配列コンテナ
		__Container container_;
	};
} /* namespace okl */