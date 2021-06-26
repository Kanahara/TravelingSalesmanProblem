#pragma once

namespace okl {
	/*!
	* @brief			グラフクラス用出力ストリーム
	* @return			std::ostream	出力ストリーム
	* @detail			グラフの情報だけ表示
	*/
	template<typename __Vertex, typename __Edge, typename __Degree, class __Vertices>
	inline std::ostream& operator<<(std::ostream& __os, const Graph<__Vertex, __Edge, __Degree, __Vertices>& __graph) {
		if (__graph.isUndirected()) {
			__os << "無向グラフ\t";
		}
		else {
			__os << "有向グラフ\t";
		}
		__os << "頂点数:" << __graph.vertices().size() << "\t";
		__os << "辺数:" << __graph.edge_size() << "\t";
		__os << "辺密度:" << __graph.density() << __os.widen('\n');

		return __os;
	}
}/* namespace okl */