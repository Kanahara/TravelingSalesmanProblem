#pragma once

#include "../Condition.hpp"
#include <cstdint>

namespace okl {
	/*!
	 *  @brief      モードが指定のものか判定
	 *  @details    モードが指定のものか判定する
	 *				モードが指定のものかTrue
	 *				- __ModeType			切り替えるモード番号
	 */
	template<std::uint_least8_t __ModeType>
	class ModeIs : public Condition
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		ModeIs() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~ModeIs() = default;

		using seed_type = typename Condition::seed_type;
		using random_device_type = typename Condition::random_device_type;
		using size_type = typename Condition::size_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "モードが" + std::to_string(__ModeType) + "かどうか"; }

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @return			bool	現在のTrue　Falseを返す
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		constexpr bool operator()(const typename __Solution::graph_type& __graph, const __SearchInfo& __search_info, const __Solution& __solution) const
		{
			return __search_info.mode() == __ModeType;
		}
		template<class __SearchInfo, class __Solution>
		constexpr bool operator()(const typename __Solution::graph_type& __graph, const __SearchInfo& __search_info, const __Solution& __solution, const __Solution& __initial_solution, const __Solution& __best_solution) const
		{
			return (*this)(__graph, __search_info, __solution);
		}
	};
} /* namespace okl */