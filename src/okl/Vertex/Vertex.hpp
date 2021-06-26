#pragma once

#include <limits>

namespace okl {
	/*!
	 *  @brief      頂点クラス
	 *  @details      頂点クラス
	 *				- __Id					頂点ID数値型
	 */
	template<typename __Id>
	class Vertex
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		Vertex() : id_(this->undefine_id()) {};

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~Vertex() = default;

		using id_type = __Id;

		/*!
		 * @brief			頂点IDを取得
		 * @detail			頂点IDを取得する
		 * @return			id_type	頂点IDを取得
		 * 					- 整数型id_typeはテンプレート__Idによって定義される
		 */
		constexpr id_type id() const
		{
			return this->id_;
		}

		/*!
		 * @brief			頂点IDを設定
		 * @param[in]		__id 頂点ID
		 * @detail			頂点IDを設定する
		 * 					- 整数型id_typeはテンプレート__Idによって定義される
		 */
		inline void setId(const id_type __id)
		{
			this->id_ = __id;
		}

		/*!
		 * @brief			未定義ID
		 * @detail			未定義IDを返す
		 * @return			id_type	未定義IDとしてid_typeの最大値を返す
		 */
		constexpr id_type undefine_id() const
		{
			return std::numeric_limits<id_type>::max();
		}

		/*!
		* @brief			比較対象より小さいか比較を行う
		* @param[in]		__other 比較対象
		* @return			bool	比較対象より小さいならTrue　以上ならFalse
		* @detail
		*/
		constexpr bool operator<(const Vertex& __other) const
		{
			return this->id() < __other.id();
		}

		/*!
		* @brief			比較対象より大きいか比較を行う
		* @param[in]		__other 比較対象
		* @return			bool	比較対象より大きいならTrue　以下ならFalse
		* @detail
		*/
		constexpr bool operator>(const Vertex& __other) const
		{
			return __other < *this;
		}

		/*!
		* @brief			比較対象以上か比較を行う
		* @param[in]		__other 比較対象
		* @return			bool	比較対象以上ならTrue　より小さいならFalse
		* @detail
		*/
		constexpr bool operator<=(const Vertex& __other) const
		{
			return !(*this > __other);
		}

		/*!
		* @brief			比較対象以下か比較を行う
		* @param[in]		__other 比較対象
		* @return			bool	比較対象以下ならTrue　より大きいならFalse
		* @detail
		*/
		constexpr bool operator>=(const Vertex& __other) const
		{
			return !(*this < __other);
		}

		/*!
		* @brief			比較対象同じか比較を行う
		* @param[in]		__other 比較対象
		* @return			bool	比較対象と同じならTrue　と違うならFalse
		* @detail
		*/
		constexpr bool operator==(const Vertex& __other) const
		{
			return this->id() == __other.id();
		}

		/*!
		* @brief			比較対象違うか比較を行う
		* @param[in]		__other 比較対象
		* @return			bool	比較対象と違うならTrue　と同じならFalse
		* @detail
		*/
		constexpr bool operator!=(const Vertex& __other) const
		{
			return !(*this == __other);
		}

	private:
		//識別番号ID
		id_type id_;
	};
} /* namespace okl */