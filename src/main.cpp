#define TSP

#include "tsp_types.hpp"
namespace okl {
	namespace problem = okl::tsp;
	static const std::string PROBLEM_NAME = "tsp";
}

int main(int argc, char* argv[]) {
	okl::CommandLine config_cmd;
	config_cmd.addOption("config_path", ' ', "../../../data/" + okl::PROBLEM_NAME + ".cfg", "�ݒ�t�@�C���p�X");
	config_cmd.parse(argc, argv);
	okl::Config _config(config_cmd.getParameter("config_path"));

	//����ꗗ�i�[�p�X
	std::string _instances_dir_path = _config["instances_dir_path"];
	//����g���q
	std::string _extension = _config["extension"];

	okl::CommandLine cmd;
	//���o�̓p�X�w��@�����Ŏw�肳�ꂽ�ꍇ�C�R���t�B�O�t�@�C���ł̎w����㏑��
	cmd.addOption("results_dir_path", ' ', _config["results_dir_path"], "���ʏo�̓f�B���N�g���p�X");
	cmd.addOption("log_dir_path", ' ', _config["log_dir_path"], "���O�o�̓f�B���N�g���p�X");
	cmd.addOption("instances_path", ' ', _config["instances_path"], "�ǂݍ��ݖ���ꗗ�t�@�C���i�[�p�X");

	//�p�����[�^�[�w��@�����Ŏw�肳�ꂽ�ꍇ�C�R���t�B�O�t�@�C���ł̎w����㏑��
	cmd.addOption("trial_num", 't', _config["trial_num"], "���s��");
	cmd.addOption("repeat_times", 'r', _config["repeat_times"], "�J��Ԃ���");
	cmd.addOption("run_time", 's', _config["run_time"], "���s����");
	cmd.addOption("pop_size", 'p', _config["pop_size"], "��W�c�T�C�Y");

	//��������w��@�����Ŏw�肳�ꂽ�ꍇ�C�R���t�B�O�t�@�C���ł̎w����㏑��
	const std::string undefine_str = "-1";
	cmd.addOption("file", 'f', undefine_str, "�ǂݍ��ݖ��t�@�C��");
	cmd.addOption("instance_br", 'b', undefine_str, "�ǂݍ��ݖ�����m�̍ŗǉ�");
	cmd.addOption("seed", ' ', undefine_str, "������s�V�[�h�l");
	cmd.addOption("type", ' ', "", "�����");

	//�����ǂݍ���
	cmd.parse(argc, argv);

	//����z��
	std::vector<okl::problem::instance_type> _instances;
	if (cmd.getParameter("file") != undefine_str &&
		cmd.getParameter("instance_br") != undefine_str &&
		cmd.getParameter("seed") != undefine_str) {
		//���Ⴊ�����Ŏw�肳��Ă���Ȃ�w�����ǂݍ���
		//����i�[�f�B���N�g��
		_instances_dir_path = cmd.getParameter("file").substr(0, cmd.getParameter("file").rfind("/") + 1);
		//�g���q
		_extension = cmd.getParameter("file").substr(cmd.getParameter("file").rfind("."), cmd.getParameter("file").size() - cmd.getParameter("file").rfind("."));
		//���ᖼ
		const auto _instance_name = cmd.getParameter("file").substr(cmd.getParameter("file").rfind("/") + 1, cmd.getParameter("file").rfind(".") - cmd.getParameter("file").rfind("/") - 1);
		auto _best_known_cost = static_cast<okl::problem::instance_type::cost_type>(std::stold(cmd.getParameter("instance_br").c_str()));
		if constexpr (std::is_floating_point_v<okl::problem::instance_type::cost_type>) {
			//�R�X�g�����������_�^�Ȃ���m�̍ŗǒlx�ɗL���������ōő�or�ŏ��̂��̂ɕύX
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
		//����w�肪�Ȃ��Ȃ����W��ǂݍ���
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