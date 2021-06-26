#pragma once

#include "../Timer/module.hpp"

namespace okl {
	/*!
	 *  @brief      結果クラス
	 *  @details      結果クラス
	 *				- __Cost		解の値　数値型
	 *				- __CostComparisonFunction		解の値　比較関数型
	 *				- __Time			算出時間　数値型
	 */
	template<
		typename __Cost,
		class __CostComparisonFunction,
		typename __Time = double>
		class Result
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		Result() : cost_(this->undefine_cost()), time_(std::numeric_limits<time_type>::max()) {};

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~Result() = default;

		using cost_type = __Cost;
		using time_type = __Time;

		using cost_comparison_function = __CostComparisonFunction;

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 */
		inline void clear()
		{
			this->cost_ = this->undefine_cost();
			this->time_ = std::numeric_limits<time_type>::max();
		}

		/*!
		 * @brief			結果を設定
		 * @param[in]		__cost 解の値
		 * @param[in]		__time 算出時間
		 * @detail			結果を設定する
		 * 					- 数値型cost_typeはテンプレート__Costによって定義される
		 * 					- 数値型time_typeはテンプレート__Timeによって定義される
		 */
		inline void set(const cost_type& __cost, const time_type& __time)
		{
			this->cost_ = __cost;
			this->time_ = __time;
		}

		/*!
		 * @brief			解の値を取得
		 * @detail			解の値を取得する
		 * @return			cost_type	解の値を取得
		 * 					- 整数型cost_typeはテンプレート__Costによって定義される
		 */
		constexpr cost_type get() const
		{
			return this->cost_;
		}

		/*!
		 * @brief			算出時間を取得
		 * @detail			算出時間を取得する
		 * @return			time_type	算出時間を取得
		 * 					- 数値型time_typeはテンプレート__Timeによって定義される
		 */
		constexpr time_type time() const
		{
			return this->time_;
		}

		/*!
		 * @brief			未定義コスト値
		 * @return			cost_type	未定義コスト値としてstd::numeric_limits<cost_type>::max()を返す
		 * @detail			未定義コスト値を返す
		 */
		constexpr cost_type undefine_cost() const
		{
			return cost_comparison_function()(std::numeric_limits<cost_type>::min(), std::numeric_limits<cost_type>::max());
		}

		/*!
		* @brief			比較対象より解の値が悪いか比較を行う
		* @param[in]		__other 比較対象
		* @return			bool	比較対象より解の値が悪いならTrue　悪くないならFalse
		* @detail
		*/
		constexpr bool operator<(const Result& __other) const
		{
			if (this->get() == this->undefine_cost()) {
				return true;
			}
			else if (__other.get() == this->undefine_cost()) {
				return false;
			}
			else if (this->get() == __other.get()) {
				return this->time() > __other.time();
			}
			else {
				return cost_comparison_function()(this->get(), __other.get());
			}
		}

		/*!
		* @brief			比較対象より解の値が良いか比較を行う
		* @param[in]		__other 比較対象
		* @return			bool	比較対象より解の値が良いならTrue　良くないならFalse
		* @detail
		*/
		constexpr bool operator>(const Result& __other) const
		{
			return __other < *this;
		}

		/*!
		* @brief			比較対象より解の値が悪いか同じか比較を行う
		* @param[in]		__other 比較対象
		* @return			bool	比較対象より解の値が悪いか同じかならTrue　良いならFalse
		* @detail
		*/
		constexpr bool operator<=(const Result& __other) const
		{
			return !(*this > __other);
		}

		/*!
		* @brief			比較対象より解の値が良いか同じか比較を行う
		* @param[in]		__other 比較対象
		* @return			bool	比較対象より解の値が良いか同じかならTrue　悪いならFalse
		* @detail
		*/
		constexpr bool operator>=(const Result& __other) const
		{
			return !(*this < __other);
		}

	private:
		//!解の値
		cost_type cost_;

		//!算出時間
		time_type time_;
	};
} /* namespace okl */

#include "operator.hpp"