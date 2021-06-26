#pragma once


namespace okl {
	//出力ストリーム演算子
	template<class __Cost>
	inline std::ostream& operator<<(std::ostream& __os, const Instance<__Cost>& __instance) {
		__os << __instance.name() << ", " << __instance.initial_seed() << ", " << __instance.best_known_cost() << "," << __instance.type();
		__os << __os.widen('\n');
		return __os;
	}
}/* namespace okl */