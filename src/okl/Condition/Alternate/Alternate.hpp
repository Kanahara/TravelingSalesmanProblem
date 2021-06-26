#pragma once

#include "../Condition.hpp"

namespace okl {
	/*!
	 *  @brief      True　Falseを交互に返す　（オルタネイトスイッチ）
	 *  @details    True　Falseを交互に返す条件クラス
	 *				初期値はTrue
	 */
	class Alternate : public Condition
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		Alternate() : switch_(false) {}

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~Alternate() = default;

		using seed_type = typename Condition::seed_type;
		using random_device_type = typename Condition::random_device_type;
		using size_type = typename Condition::size_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "交互"; }

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 */
		inline void clear() override
		{
			Condition::clear();
			this->switch_ = false;
		}

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @return			bool	現在のTrue　Falseを返す
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		inline bool operator()(const typename __Solution::graph_type& __graph, const __SearchInfo& __search_info, const __Solution& __solution, const __Solution& __initial_solution, const __Solution& __best_solution)
		{
			this->switch_ = !this->switch_;
			return this->switch_;
		}
		template<class __SearchInfo, class __Solution, class _Population>
		inline typename std::enable_if<!std::is_same<__Solution, _Population>::value, bool>::type operator()(const typename __Solution::graph_type& __graph, const __SearchInfo& __search_info, const _Population& __population, const __Solution& __initial_solution, const __Solution& __best_solution)
		{
			return (*this)(__graph, __search_info, __population.individual(*__population.present_generation().begin()), __initial_solution, __best_solution);
		}

	private:
		//!現在の状態を保持
		bool switch_;
	};
} /* namespace okl */