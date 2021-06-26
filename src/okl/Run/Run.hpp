#pragma once

#include "../Timer/module.hpp"
#include "../Random/module.hpp"
#include <vector>
#include <limits>

namespace okl {
	/*!
	 * @brief			指定の試行回数探索を行う
	 * @param[in]		__max_times 試行回数
	 * @param[in]		__seed 初期シード
	 * @param[in]		__graph グラフクラス
	 * @param[in]		__search 探索クラス
	 * @param[out]		__search_info 探索情報クラス
	 * @param[out]		__solution 解クラス
	 * @detail			指定の試行回数探索を行う
	 */
	template<class __Graph, class __Search, class __SearchInfo, class __Solution>
	void run(const std::size_t __max_times, std::seed_seq::result_type __seed, __Graph& __graph, __Search& __search, __SearchInfo& __search_info, __Solution& __solution)
	{
		Random<> _seed_generator(__seed);

		const auto _initial_solution = __solution;

		for (std::size_t _times = 1; _times <= __max_times; _times++)
		{
			std::cout << "-" << _times << std::flush;
			__search.clear();
			__search_info.clear();
			__solution = _initial_solution;
			__search.seed(_seed_generator.uniform_int(std::numeric_limits < std::seed_seq::result_type >::min(), std::numeric_limits < std::seed_seq::result_type >::max()));

			__search_info.startTime();

			__search(__graph, __search_info, __solution);
			__search_info.writeResult();
			__search_info.writeLog();
		}
		std::cout << std::endl;
	}
} /* namespace okl */