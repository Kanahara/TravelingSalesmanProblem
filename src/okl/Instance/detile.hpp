#include "Instance.hpp"
#pragma once
namespace okl {
	/*!
	 * @brief			例題読み込み
	 * @return			std::vector<Instance<__Cost>>	例題配列を返す
	 * @detail			実装により例題配列の型は変更される
	 * 					 - _CostComparisonFunction				コスト比較関数
	 */
	template<class __Cost>
	template<class _CostComparisonFunction>
	inline const std::vector<Instance<__Cost>> Instance<__Cost>::load(const std::string& __path)
	{
		using cost_comparison_function = _CostComparisonFunction;
		std::ifstream _fin(__path);
		std::vector<Instance<__Cost>> _problems;

		while (_fin.fail())
		{
			std::cout << __path << std::endl;
			std::cout << "問題例一覧入力ファイルをオープンできません" << std::endl;
			_fin.open(__path);
		}

		//utf8 bom skip
		_fin.imbue(std::locale());
		int dst[3];
		for (auto& i : dst) i = _fin.get();
		constexpr int utf8[] = { 0xEF, 0xBB, 0xBF };
		if (!std::equal(std::begin(dst), std::end(dst), utf8)) _fin.seekg(0);

		while (!_fin.eof()) {
			std::string _baffer;
			std::getline(_fin, _baffer);
			std::istringstream _stream(_baffer);
			std::seed_seq::result_type _initial_seed = 0;
			std::string _name, _type = "";
			cost_type _best_known_cost = 0;
			if constexpr (std::is_floating_point_v<cost_type>) {
				//コストが浮動小数点型なら既知の最良値xに有効数字内で最大or最小のものに変更
				std::string _best_known_cost_str;
				_stream >> _name >> _best_known_cost_str >> _initial_seed >> _type;
				_best_known_cost = okl::sto_significant_digits<cost_type, cost_comparison_function>(_best_known_cost_str);
			}
			else {
				_stream >> _name >> _best_known_cost >> _initial_seed >> _type;
			}
			_problems.emplace_back(_name, _best_known_cost, _initial_seed, _type);
		}

		return _problems;
	}
} /* namespace okl */