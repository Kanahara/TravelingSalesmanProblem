#pragma once

#include "../Condition.hpp"
#include <cstdint>

namespace okl {
	/*!
	 *  @brief      設定回数繰り返したか判定
	 *  @details    設定回数繰り返したか判定する
	 *				設定回数繰り返したらTrue
	 */
	class ReachedRepeatTimes : public Condition
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		ReachedRepeatTimes() : repeat_times_(0) {};

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~ReachedRepeatTimes() = default;

		using seed_type = typename Condition::seed_type;
		using random_device_type = typename Condition::random_device_type;
		using size_type = typename Condition::size_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "設定回数繰り返すまで"; }

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 */
		inline void clear() override
		{
			Condition::clear();
			this->repeat_times_ = 0;
		}

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @return			bool	現在のTrue　Falseを返す
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		inline bool operator()(const typename __Solution::graph_type& __graph, const __SearchInfo& __search_info, const __Solution& __solution)
		{
			++this->repeat_times_;

			return __search_info.repeat_times() <= this->repeat_times_ && __search_info.repeat_times() != 0;
		}


		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		inline bool operator()(const typename __Solution::graph_type& __graph, __SearchInfo& __search_info, __Solution& __solution, const __Solution& __initial_solution, const __Solution& __best_solution)
		{
			return (*this)(__graph, __search_info, __solution);
		}

	private:
		//!現在の繰り返し回数
		std::size_t repeat_times_;
	};
} /* namespace okl */