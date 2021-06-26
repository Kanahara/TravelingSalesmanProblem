#pragma once

#include <chrono>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <ctime>

namespace okl {
	/*!
	 *  @brief		タイマークラス
	 *  @details    タイマークラス
	 *				計測のための汎用的な計測クラス
	 */
	class Timer
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		Timer() : start_time_(clock::time_point::min()) {}

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~Timer() = default;

		using clock = std::chrono::high_resolution_clock;
		using nanoseconds = std::chrono::duration<double, std::nano>;
		using microseconds = std::chrono::duration<double, std::micro>;
		using milliseconds = std::chrono::duration<double, std::milli>;
		using seconds = std::chrono::duration<double>;
		using minutes = std::chrono::duration<double, std::ratio< 60>>;
		using hours = std::chrono::duration<double, std::ratio< 3600>>;

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 */
		void clear()
		{
			this->start_time_ = clock::time_point::min();
		}

		/*!
		 * @brief			タイマーが開始されているか判定
		 * @return			bool　タイマーが開始されているならTrue 開始されてないならFalse
		 * @detail			タイマーが開始されているか判定する
		 */
		constexpr bool isStarted() const
		{
			return this->start_time_.time_since_epoch() != clock::duration(0);
		}

		/*!
		 * @brief			計測開始
		 * @detail			タイマーの計測を開始する
		 */
		inline void start()
		{
			this->start_time_ = clock::now();
			this->start_clock_ = std::clock();
		}

		/*!
		 * @brief			開始してからの実時間を返す
		 * @return			double　開始してからのdouble型実時間を返す
		 * @detail			開始してからのdouble型実時間を返す
		 *					テンプレート__Timeにはクラス内で定義している時間型を定義として渡す
		 */
		template<class __Time>
		constexpr double time() const
		{
			if (this->isStarted()) {
				return std::chrono::duration_cast <__Time> (clock::now() - this->start_time_).count();
			}
			else {
				return static_cast<double>(0.0);
			}
		}

		/*!
		 * @brief			開始してからのCPU時間を返す
		 * @return			double　開始してからのdouble型CPU時間を返す
		 * @detail			開始してからのdouble型CPU時間を返す
		 *					テンプレート__Timeにはクラス内で定義している時間型を定義として渡す
		 */
		template<class __Time>
		constexpr double cpu_time() const
		{
			if (this->isStarted()) {
				seconds sec(static_cast<double>(std::clock() - this->start_clock_) / static_cast<double>(CLOCKS_PER_SEC));
				return std::chrono::duration_cast <__Time> (sec).count();
			}
			else {
				return static_cast<double>(0.0);
			}
		}

	private:
		//!開始時間
		clock::time_point start_time_;

		//!開始クロック
		clock_t start_clock_;
	};
} /* namespace okl */