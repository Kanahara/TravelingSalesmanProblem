#define TSP

#include "tsp_types.hpp"
namespace okl {
	namespace problem = okl::tsp;
	static const std::string PROBLEM_NAME = "tsp";
}

int main(int argc, char* argv[]) {
	okl::CommandLine config_cmd;
	config_cmd.addOption("config_path", ' ', "../../../data/" + okl::PROBLEM_NAME + ".cfg", "設定ファイルパス");
	config_cmd.parse(argc, argv);
	okl::Config _config(config_cmd.getParameter("config_path"));

	//問題例一覧格納パス
	std::string _instances_dir_path = _config["instances_dir_path"];
	//問題例拡張子
	std::string _extension = _config["extension"];

	okl::CommandLine cmd;
	//入出力パス指定　引数で指定された場合，コンフィグファイルでの指定を上書き
	cmd.addOption("results_dir_path", ' ', _config["results_dir_path"], "結果出力ディレクトリパス");
	cmd.addOption("log_dir_path", ' ', _config["log_dir_path"], "ログ出力ディレクトリパス");
	cmd.addOption("instances_path", ' ', _config["instances_path"], "読み込み問題例一覧ファイル格納パス");

	//パラメーター指定　引数で指定された場合，コンフィグファイルでの指定を上書き
	cmd.addOption("trial_num", 't', _config["trial_num"], "試行回数");
	cmd.addOption("repeat_times", 'r', _config["repeat_times"], "繰り返し回数");
	cmd.addOption("run_time", 's', _config["run_time"], "実行時間");
	cmd.addOption("pop_size", 'p', _config["pop_size"], "母集団サイズ");

	//問題例引数指定　引数で指定された場合，コンフィグファイルでの指定を上書き
	const std::string undefine_str = "-1";
	cmd.addOption("file", 'f', undefine_str, "読み込み問題ファイル");
	cmd.addOption("instance_br", 'b', undefine_str, "読み込み問題例既知の最良解");
	cmd.addOption("seed", ' ', undefine_str, "問題例実行シード値");
	cmd.addOption("type", ' ', "", "問題種類");

	//引数読み込み
	cmd.parse(argc, argv);

	//問題例配列
	std::vector<okl::problem::instance_type> _instances;
	if (cmd.getParameter("file") != undefine_str &&
		cmd.getParameter("instance_br") != undefine_str &&
		cmd.getParameter("seed") != undefine_str) {
		//問題例が引数で指定されているなら指定問題を読み込む
		//問題例格納ディレクトリ
		_instances_dir_path = cmd.getParameter("file").substr(0, cmd.getParameter("file").rfind("/") + 1);
		//拡張子
		_extension = cmd.getParameter("file").substr(cmd.getParameter("file").rfind("."), cmd.getParameter("file").size() - cmd.getParameter("file").rfind("."));
		//問題例名
		const auto _instance_name = cmd.getParameter("file").substr(cmd.getParameter("file").rfind("/") + 1, cmd.getParameter("file").rfind(".") - cmd.getParameter("file").rfind("/") - 1);
		auto _best_known_cost = static_cast<okl::problem::instance_type::cost_type>(std::stold(cmd.getParameter("instance_br").c_str()));
		if constexpr (std::is_floating_point_v<okl::problem::instance_type::cost_type>) {
			//コストが浮動小数点型なら既知の最良値xに有効数字内で最大or最小のものに変更
			_best_known_cost = okl::sto_significant_digits<okl::problem::instance_type::cost_type, okl::problem::cost_comparison_function>(cmd.getParameter("instance_br"));
		}
		_instances.emplace_back(
			_instance_name,
			_best_known_cost,
			std::stoul(cmd.getParameter("seed").c_str()),
			cmd.getParameter("type")
		);
	}
	else {
		//問題例指定がないなら問題例集を読み込む
		_instances = okl::problem::instance_type::load<okl::problem::cost_comparison_function>(cmd.getParameter("instances_path"));
	}

	for (const auto& _instance : _instances) {
		okl::problem::search _search;
		okl::problem::graph_type _graph;
		okl::problem::search_info_type _search_info;
		okl::problem::solution_type _solution;
		okl::problem::load(_instances_dir_path + _instance.name() + _extension, _instance, _graph, _search_info, _solution);
		_search.resize(static_cast<okl::problem::search::size_type>(_graph.vertices().size()));

		std::cout << _instance;
		std::cout << _graph;
		std::cout << okl::format(okl::problem::search::info()) << std::endl;
		
		_search_info.setBestKnownCost(_instance.best_known_cost());
		_search_info.setRepeatTimes(std::stoul(cmd.getParameter("repeat_times").c_str()));
		_search_info.setRunTime(std::stod(cmd.getParameter("run_time").c_str()));
		_search_info.setResultOutputPath(cmd.getParameter("results_dir_path") + _instance.name());
		_search_info.setLogOutputPath(cmd.getParameter("log_dir_path") + _instance.name());
		_search_info.setPopSize(std::stoul(cmd.getParameter("pop_size").c_str()));
		
		okl::run(std::stoul(cmd.getParameter("trial_num").c_str()), _instance.initial_seed(), _graph, _search, _search_info, _solution);
	}
}