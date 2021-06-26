#pragma once
#include <string>

namespace okl::tsp {
	/*!
	* @brief			巡回セールスマン問題のアスキー形式の問題を読み込む
	* @param[in]		__path			問題ファイルパス
	* @param[in]		__instance		問題クラス
	* @param[in]		__graph			グラフクラス
	* @param[in]		__search_info	探索情報クラス
	* @param[in]		__solution		解クラス
	*/
	template<
		class __Instance,
		class __Graph,
		class __SearchInfo,
		class __Solution>
		void load(const std::string& __path, const __Instance& __instance, __Graph& __graph, __SearchInfo& __search_info, __Solution& __solution)
	{
		using id_type = typename __Graph::id_type;
		using edge_type = typename __Graph::edge_type;
		using vertex_type = typename __Graph::vertex_type;
		using size_type = typename __Graph::size_type;
		using position_type = typename vertex_type::position_type;

		std::ifstream _fin(__path);

		while (_fin.fail())
		{
			std::cout << __path << std::endl;
			std::cout << "TSP入力ファイルをオープンできません" << std::endl;
			_fin.open(__path);
		}

		size_type _size;
		std::string _baffer = "";
		while (_baffer != "NODE_COORD_SECTION")
		{
			std::getline(_fin, _baffer);
			_baffer.erase(remove(_baffer.begin(), _baffer.end(), ' '), _baffer.end());
			const std::string _dimension_str = "DIMENSION:";
			const auto _pos = _baffer.find(_dimension_str);
			if (_pos != std::string::npos) {
				std::istringstream _stream(_baffer.substr(_dimension_str.size(), _baffer.size() - _dimension_str.size()));

				//ノード数N
				std::size_t _load_size;

				_stream >> _load_size;

				//ノード数N
				_size = static_cast<size_type>(_load_size);

				__graph.resize(_size);
				__solution.resize(_size);
				__search_info.resize(__graph.vertices().size());

				break;
			}
		}
		std::getline(_fin, _baffer);
		std::getline(_fin, _baffer);
		for (id_type _id = 0; _id < _size; _id++)
		{
			std::getline(_fin, _baffer);
			std::istringstream _stream(_baffer);

			vertex_type _vertex;

			id_type _dummy_id;
			position_type _x, _y;

			_stream >> _dummy_id >> _x >> _y;

			_vertex.setId(_id);
			_vertex.setPostion(_x, _y);
			__graph.addVertex(_vertex);

			__solution.addNode(_id);
		}

		for (const auto& _start_vertex : __graph.vertices())
		{
			for (const auto& _end_vertex : __graph.vertices())
			{
				__graph.addDirectedEdge(_start_vertex.id(), _end_vertex.id(), static_cast<edge_type>(vertex_type::distance(_start_vertex, _end_vertex, __instance.type())));
			}
		}
		__graph.updateInfo();
	}
} /* namespace okl::tsp */