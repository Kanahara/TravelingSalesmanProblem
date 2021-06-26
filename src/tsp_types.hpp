#pragma once
#include "okl/module.hpp"

namespace okl::tsp {
	//using id_type = std::uint_least8_t;		//     1 <= ���_�� <=        256
	using id_type = std::uint_least16_t;		//   257 <= ���_�� <=      65535
	//using id_type = std::uint_least32_t;		// 65536 <= ���_�� <= 4294957295
	using edge_type = std::uint_least16_t;
	using degree_type = okl::make_twice_byte_t<id_type>;
	using cost_type = std::int_least64_t;

	using vertex_type = okl::tsp::Vertex<id_type, double>;

	using stack_type = okl::Stack<id_type>;
	using set_type = okl::HashSet<id_type>;

	using graph_type = okl::MatrixGraph<vertex_type, edge_type, degree_type, std::vector<vertex_type>, SymmetricMatrix<edge_type, std::uint_fast64_t>>;

	using solution_type = okl::tsp::Solution<graph_type, cost_type, set_type>;


	using instance_type = okl::Instance<cost_type>;

	using cost_comparison_function = typename solution_type::cost_comparison_function;

	using result_type = okl::Result<cost_type, cost_comparison_function>;

	using info_type = std::tuple<cost_type, double, std::size_t, std::size_t, std::size_t, std::size_t, std::size_t, std::size_t, std::size_t, std::size_t>;

	using search_info_type = okl::SearchInfo<result_type, info_type>;

	//���̑�--------------------------------------------------------------------------------------------------------------------------------------

	//�Ǐ��T�����J�E���g�@���͗p
	using ls_count = okl::CountKeyOperator<0>;

	//--����---------------------------------------------------------------------------------------------------------------------------------------
	//�ŗǉ����ǂ��Ȃ��Ă�����
	using best_cost_is_better_than_before = okl::CostIsBetterThanBefore<cost_type, cost_comparison_function>;

	//�ŗǉ����ǂ��Ȃ��Ă��Ȃ��Ȃ�
	using not_best_cost_is_better_than_before = okl::NotCondition<best_cost_is_better_than_before>;

	//�w��񐔌J��Ԃ����Ȃ�
	using reached_repeat_times = okl::ReachedRepeatTimes;

	//���m�̍ŗǉ����Z�o
	using calculated_best_known_cost = okl::CalculatedBestKnownCost;

	//���m�̍ŗǉ����Z�o�܂��͎w��񐔌J��Ԃ����Ȃ�
	using reached_repeat_times_or_calculated_best_known_cost = okl::OrCondition<reached_repeat_times, calculated_best_known_cost>;

	//---------------------------------------------------------------------------------------------------------------------------------------------

	using start = okl::tsp::RandomRouteOrderOperator;
	//using start = okl::tsp::NearestNeighborRouteOrderOperator<set_type>;

	using end_condition = okl::OrCondition< okl::OrCondition<okl::TimerCondition, calculated_best_known_cost>, okl::ReachedRepeatTimes>;
	//using end_condition = best_cost_is_better_then_best_known_cost;
	//using end_condition = reached_repeat_times_or_calculated_best_known_cost;

	using _ls = okl::LocalSearch < solution_type, okl::tsp::BestImprovementRouteSwapOperator, not_best_cost_is_better_than_before>;

	using ls = okl::SearchPrePostOperator<ls_count, _ls, okl::NoneOperator>;

	using mls = okl::MultiStartLocalSearch<ls, start, start, end_condition>;

	using search = mls;
} /* namespace okl::tsp */