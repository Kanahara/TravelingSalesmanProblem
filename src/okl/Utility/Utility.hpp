#pragma once
#include <string>
#include <vector>
#include <stack>
#include <regex>
#include <unordered_map>
#include <tuple>
namespace okl {
	std::string format(std::string _target_string, const std::string _indent = "\t") {
		std::string _format_string = "";

		std::string r;
		_target_string = std::regex_replace(_target_string, std::regex(R"(\+\))"), ")");
		_target_string = std::regex_replace(_target_string, std::regex(R"(\+\))"), ")");
		_target_string = std::regex_replace(_target_string, std::regex(R"(\+\+)"), "+");
		_target_string = std::regex_replace(_target_string, std::regex(R"(\+\+)"), "+");
		_target_string = std::regex_replace(_target_string, std::regex(R"(\+\+)"), "+");
		_target_string = std::regex_replace(_target_string, std::regex(R"(\+\+)"), "+");
		_target_string = std::regex_replace(_target_string, std::regex(R"(\+\+)"), "+");

		auto generate_indent = [](const std::string _indent, const std::size_t __size) {
			std::string __indent = "";
			for (std::size_t _depth = 0; _depth < __size; ++_depth) {
				__indent += _indent;
			}
			return __indent;
		};

		std::stack<char> _stack;
		std::stack<char> _kagi_stack;
		for (const auto _c : _target_string) {
			if (_c == '(') {
				_stack.push(_c);
				_format_string += "\n" + generate_indent(_indent, _stack.size()) + "(";
			}
			else if (_c == ')' && _stack.top() == '(') {
				_stack.pop();
				_format_string += ")";
			}
			else if (_c == '<') {
				_stack.push(_c);
				_format_string += '\n' + generate_indent(_indent, _stack.size()) + '<';
			}
			else if (_c == '>' && _stack.top() == '<') {
				_stack.pop();
				_format_string += ">";
			}
			else if (_c == '+') {
				_format_string += '\n' + generate_indent(_indent, _stack.size()) + "+";
			}
			else {
				_format_string += _c;
			}
		}

		return _format_string;
	}

	class Config {
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		Config() = default;
		Config(const std::string __path) {
			this->load(__path);
		}

		/*!
		 * @brief				デストラクタ
		 */
		~Config() = default;

		/*!
		 * @brief			コンフィグファイルを読み込む
		 * @param[in]		__path 読み込みパス
		 * @detail			コンフィグファイルを読み込む
		 */
		void load(const std::string __path) {
			//ファイル読み込み
			std::ifstream _fin(__path);

			if (_fin.fail())
			{
				std::cout << __path << std::endl;
				std::cout << "コンフィグファイルをオープンできません" << std::endl;
				_fin.open(__path);
			}
			else {
				while (!_fin.eof()) {
					std::string _baffer;
					std::getline(_fin, _baffer);
					const auto _pos = _baffer.find("\r");
					if (_pos != std::string::npos) {
						_baffer.erase(_pos);
					}
					if (_baffer.find("\n") != std::string::npos) {
						_baffer.erase(_pos);
					}
					std::string _key = _baffer.substr(0, _baffer.find("="));
					std::string _value = _baffer.substr(_baffer.find("=") + 1, _baffer.size() - _baffer.find("=") - 1);
					this->config_map_[_key] = _value;
				}
			}
		}

		/*!
		 * @brief			設定値を取得する
		 * @param[in]		__key コンフィグ名
		 * @detail			設定値を文字列で取得する
		 */
		std::string get(const std::string __key) {
			return this->config_map_[__key];
		}

		/*!
		 * @brief			設定値を取得する
		 * @param[in]		__key コンフィグ名
		 * @detail			設定値を文字列で取得する
		 */
		std::string operator[](const std::string __key) {
			return this->config_map_[__key];
		}

	private:
		//!コンフィグ格納コンテナ
		std::unordered_map<std::string, std::string> config_map_;
	};

	class CommandLine {
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		CommandLine() = default;

		/*!
		 * @brief				デストラクタ
		 */
		~CommandLine() = default;

		/*!
		 * @brief			コマンドラインを解析
		 * @param[in]		__argc コマンドライン引数の数
		 * @param[in]		__argc コマンドライン引数配列
		 * @detail			コマンドラインを解析する
		 */
		void parse(int __argc, char* __argv[]) {
			std::vector<std::string> _str_argv;
			for (int _index = 0; _index < __argc; ++_index) {
				_str_argv.emplace_back(__argv[_index]);
			}

			for (std::size_t _index = 0; _index < _str_argv.size(); ++_index) {
				const auto& _arg = _str_argv[_index];
				if (_arg.find("-") != std::string::npos) {
					const auto _key = _arg.substr(1, _arg.size());
					if (this->getParameterIndex(_key) != std::numeric_limits<std::size_t>::max()) {
						++_index;
						this->parameter_map_[this->getParameterIndex(_key)] = _str_argv[_index];
					}
				}
			}
		}

		/*!
		 * @brief			コマンドラインのオプションの追加
		 * @param[in]		__option オプション文字列
		 * @param[in]		__short_opttion ショートオプション文字
		 * @param[in]		__defautl_parameter デフォルトパラメータ
		 * @param[in]		__info --helpオプション時に表示される情報
		 * @detail			コマンドラインのオプションの追加する
		 */
		void addOption(const std::string __option, const char __short_opttion = ' ', const std::string __defautl_parameter = "", const std::string __info = "") {
			const auto _index = this->commands_.size();
			this->commands_.emplace_back(std::make_tuple(__option, __short_opttion, __defautl_parameter, __info));
			this->option_map_.try_emplace(__option, _index);
			if (__short_opttion != ' ') {
				this->short_option_map_.try_emplace(__short_opttion, _index);
			}
		}

		/*!
		 * @brief			パラメータの取得
		 * @param[in]		__key オプションキー
		 * @return			オプションを文字列で返す
		 * @detail			キーに紐付けられたのパラメータの取得
		 */
		std::string getParameter(const std::string __key) {
			const auto _index = this->getParameterIndex(__key);
			if (this->parameter_map_.find(_index) == this->parameter_map_.end()) {
				//デフォルトパラメータを返す
				return std::get<2>(this->commands_[_index]);
			}
			else {
				//設定パラメータを返す
				return this->parameter_map_[_index];
			}
		}

	private:
		//!オプションとコマンド格納配列要素を紐付けMAP
		std::unordered_map<std::string, std::size_t> option_map_;

		//!オプションとショートコマンド格納配列要素を紐付けMAP
		std::unordered_map<char, std::size_t> short_option_map_;

		//!キーと設定パラメータを紐付けMAP
		std::unordered_map<std::size_t, std::string> parameter_map_;

		//!コマンド格納配列
		std::vector<std::tuple<std::string, char, std::string, std::string>> commands_;

		/*!
		 * @brief			キーのパラメータインデックスの取得
		 * @param[in]		__key オプションキー
		 * @return			パラメータ格納インデックスを返す
		 * @detail			キーのパラメータインデックスの取得
		 */
		std::size_t getParameterIndex(const std::string __key) {
			if (__key.size() == 1 && this->short_option_map_.find(__key[0]) != this->short_option_map_.end()) {
				return this->short_option_map_[__key[0]];
			}
			else if (this->option_map_.find(__key) != this->option_map_.end()) {
				return this->option_map_[__key];
			}
			return std::numeric_limits<std::size_t>::max();
		}
	};

	/*!
	* @brief			小数点以下指定四捨五入
	* @param[in]		__value 四捨五入する数値
	* @param[in]		__decimal_places 小数点以下の桁数
	* @return			__valueを四捨五入した値
	* @detail			round((__value * 10^__decimal_places) / 10^__decimal_places)を返す
	* 					 - __RealType							実数値型
	* 					 - __Number								乗数値型
	*/
	template<typename __RealType, typename __Number>
	constexpr __RealType round(__RealType __value, __Number __decimal_places) {
		return std::round(__value * std::pow(static_cast<__RealType>(10.0), __decimal_places - 1)) / std::pow(static_cast<__RealType>(10.0), __decimal_places - 1);
	}

	/*!
	* @brief			有効数字内の最大or最小の数値
	* @param[in]		__real_type_value_string 四捨五入する数値
	* @return			有効数字内の最大or最小の数値
	* @detail			a<x<bの有効数字内の最大or最小の値を返す
	*					aは小数点以下の有効数字+1の桁で四捨五入した場合xになる最小の値
	*					bは小数点以下の有効数字+1の桁で四捨五入した場合xになる最大の値
	* 					 - __RealType							実数値型
	* 					 - _CostComparisonFunction				比較関数
	*/
	template<typename __RealType, class __CostComparisonFunction>
	constexpr __RealType sto_significant_digits(const std::string __real_type_value_string) {
		using real_type = __RealType;
		using cost_comparison_function = __CostComparisonFunction;

		//小数点以下の有効桁数
		std::int_fast64_t _significant_difits_after_decimal_point = 0;
		if (const auto _decimal_point_index = __real_type_value_string.find_last_of("."); _decimal_point_index != std::string::npos) {
			_significant_difits_after_decimal_point = static_cast<std::int_fast64_t>(__real_type_value_string.size()) - _decimal_point_index - 1;
		}
		//既知の最良値xに有効数字内で最大or最小のものを探索
		real_type _value = 0;
		if constexpr (std::is_same_v<real_type, float>) {
			_value = std::stof(__real_type_value_string);
		}
		else if constexpr (std::is_same_v<real_type, double>) {
			_value = std::stod(__real_type_value_string);
		}
		else if constexpr (std::is_same_v<real_type, long double>) {
			_value = std::stold(__real_type_value_string);
		}
		const auto _value_sto_round = static_cast<real_type>(okl::round(_value, _significant_difits_after_decimal_point + 1));

		//Δ = 10^(-小数点以下の有効数字) / 2　例:0.xx→0.005
		auto _delta = std::pow(static_cast<real_type>(10.0), -_significant_difits_after_decimal_point) / static_cast<real_type>(2.0);
		if (cost_comparison_function()(static_cast<real_type>(-1.0), static_cast<real_type>(1.0))) {
			_delta *= static_cast<real_type>(-1.0);
		}
		_value += _delta;

		while (cost_comparison_function()(okl::round(_value, _significant_difits_after_decimal_point + 1), _value_sto_round))
		{
			_value = std::nextafter(_value, std::numeric_limits<real_type>::min());
		}
		while (cost_comparison_function()(_value, okl::round(std::nextafter(_value, std::numeric_limits<real_type>::max()), _significant_difits_after_decimal_point + 1)))
		{
			_value = std::nextafter(_value, std::numeric_limits<real_type>::max());
		}
		return _value;
	}

	template<std::size_t __Numerator, std::size_t __Denominator, class Real = double>
	class RealNumber {
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		RealNumber() = default;

		/*!
		 * @brief				コンストラクタ
		 */
		~RealNumber() = default;

		using real_type = Real;

		static const std::string info() { return std::to_string(static_cast<real_type>(__Numerator) / static_cast<real_type>(__Denominator)); }

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		const real_type operator()() { return this->p_; };

	private:
		//!実数値:P
		const real_type p_ = static_cast<real_type>(__Numerator) / static_cast<real_type>(__Denominator);
	};

	template<typename __Type>
	class make_twice_byte {
	public:
		using type = __Type;
	};

	template<>
	class make_twice_byte<char> {
	public:
		using type = short;
	};

	template<>
	class make_twice_byte<signed char> {
	public:
		using type = short;
	};

	template<>
	class make_twice_byte<signed short> {
	public:
		using type = int;
	};

	template<>
	class make_twice_byte<signed int> {
	public:
		using type = long;
	};

	template<>
	class make_twice_byte<signed long> {
	public:
		using type = long long;
	};

	template<>
	class make_twice_byte<unsigned char> {
	public:
		using type = unsigned short;
	};

	template<>
	class make_twice_byte<unsigned short> {
	public:
		using type = unsigned int;
	};

	template<>
	class make_twice_byte<unsigned int> {
	public:
		using type = unsigned long;
	};

	template<>
	class make_twice_byte<unsigned long> {
	public:
		using type = unsigned long long;
	};

	template<typename __Type>
	using make_twice_byte_t = typename make_twice_byte<__Type>::type;
} /* namespace okl */

namespace std {
	namespace
	{
		// Code from boost
		// Reciprocal of the golden ratio helps spread entropy
		//     and handles duplicates.
		// See Mike Seymour in magic-numbers-in-boosthash-combine:
		//     http://stackoverflow.com/questions/4948780

		template <class T>
		inline void hash_combine(std::size_t& seed, T const& v)
		{
			seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}

		// Recursive template code derived from Matthieu M.
		template <class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
		struct HashValueImpl
		{
			static void apply(size_t& seed, Tuple const& tuple)
			{
				HashValueImpl<Tuple, Index - 1>::apply(seed, tuple);
				hash_combine(seed, std::get<Index>(tuple));
			}
		};

		template <class Tuple>
		struct HashValueImpl<Tuple, 0>
		{
			static void apply(size_t& seed, Tuple const& tuple)
			{
				hash_combine(seed, std::get<0>(tuple));
			}
		};
	}

	template <typename ... TT>
	struct hash<std::tuple<TT...>>
	{
		size_t
			operator()(std::tuple<TT...> const& tt) const
		{
			size_t seed = 0;
			HashValueImpl<std::tuple<TT...> >::apply(seed, tt);
			return seed;
		}
	};
}