#pragma once

#include "../../Vertex/module.hpp"
#include <complex>

namespace okl::tsp {
	/*!
	 *  @brief      TSP用頂点クラス
	 *  @details      TSP用頂点クラス
	 *				- __Id					頂点ID数値型
	 *				- __Position			座標数値型
	 *				- __Demand				需要数値型
	 */
	template<
		typename __Id,
		typename __Position>
		class Vertex : public okl::Vertex<__Id>
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		Vertex() : x_(0), y_(0) {};

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~Vertex() = default;

		using id_type = typename okl::Vertex<__Id>::id_type;
		using position_type = __Position;

		/*!
		 * @brief			X座標を取得
		 * @detail			X座標を取得する
		 * @return			position_type	X座標を取得
		 * 					- 数値型position_typeはテンプレート__Positionによって定義される
		 */
		constexpr position_type x() const
		{
			return this->x_;
		}

		/*!
		 * @brief			X座標を設定
		 * @param[in]		__x X座標
		 * @detail			X座標を設定する
		 * 					- 数値型position_typeはテンプレート__Positionによって定義される
		 */
		inline void setX(const position_type __x)
		{
			this->x_ = __x;
		}

		/*!
		 * @brief			Y座標を取得
		 * @detail			Y座標を取得する
		 * @return			position_type	X座標を取得
		 * 					- 数値型position_typeはテンプレート__Positionによって定義される
		 */
		constexpr position_type y() const
		{
			return this->y_;
		}

		/*!
		 * @brief			Y座標を設定
		 * @param[in]		__y Y座標
		 * @detail			Y座標を設定する
		 * 					- 数値型position_typeはテンプレート__Positionによって定義される
		 */
		inline void setY(const position_type __y)
		{
			this->y_ = __y;
		}

		/*!
		 * @brief			座標を設定
		 * @param[in]		__x X座標
		 * @param[in]		__y Y座標
		 * @detail			座標を設定する
		 * 					- 数値型position_typeはテンプレート__Positionによって定義される
		 */
		inline void setPostion(const position_type __x, const position_type __y)
		{
			this->setX(__x);
			this->setY(__y);
		}

		/*!
		 * @brief			距離を計算
		 * @detail			距離を計算する
		 * @return			double 計算を取得
		 */
		static constexpr double distance(const Vertex& _start, const Vertex& _end, const std::string __type)
		{
			if (__type == "EUC_2D") {
				return  std::round(std::sqrt(std::pow(_start.x() - _end.x(), 2) + std::pow(_start.y() - _end.y(), 2)));
			}
			else if (__type == "EUC_2D_REAL") {
				return  std::sqrt(std::pow(_start.x() - _end.x(), 2) + std::pow(_start.y() - _end.y(), 2));
			}
			else {
				assert(false);
				return 0.0;
			}
		}

	private:
		//!x座標
		__Position x_;

		//!y座標
		__Position y_;
	};
} /* namespace okl::tsp */