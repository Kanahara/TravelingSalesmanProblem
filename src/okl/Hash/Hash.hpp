#include <cstdint>
#include "../Vertex/module.hpp"

namespace okl {
	template<class __T>
	class Hash {
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		Hash() = default;

		/*!
		 * @brief				デストラクタ
		 */
		~Hash() = default;

		using result_type = std::size_t;

		/*!
		 * @brief			ハッシュ値を計算する
		 * @detail			ハッシュ値を計算する
		 */
		result_type operator()(const __T& __key);
	};

	template<>
	class Hash<unsigned char> {
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		Hash() = default;
		/*!
		 * @brief				デストラクタ
		 */
		~Hash() = default;

		using result_type = unsigned char;
		using argument_type = unsigned char;

		/*!
		 * @brief			ハッシュ値を計算する
		 * @detail			ハッシュ値を計算する
		 */
		constexpr result_type operator()(const argument_type __key) const { return __key; };
	};

	template<>
	class Hash<unsigned short> {
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		Hash() = default;
		/*!
		 * @brief				デストラクタ
		 */
		~Hash() = default;

		using result_type = unsigned short;
		using argument_type = unsigned short;

		/*!
		 * @brief			ハッシュ値を計算する
		 * @detail			ハッシュ値を計算する
		 */
		constexpr result_type operator()(const argument_type __key) const { return __key; };
	};

	template<>
	class Hash<unsigned int> {
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		Hash() = default;
		/*!
		 * @brief				デストラクタ
		 */
		~Hash() = default;

		using result_type = unsigned int;
		using argument_type = unsigned int;

		/*!
		 * @brief			ハッシュ値を計算する
		 * @detail			ハッシュ値を計算する
		 */
		constexpr result_type operator()(const argument_type __key) const { return __key; };
	};

	template<>
	class Hash<unsigned long> {
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		Hash() = default;
		/*!
		 * @brief				デストラクタ
		 */
		~Hash() = default;

		using result_type = unsigned long;
		using argument_type = unsigned long;

		/*!
		 * @brief			ハッシュ値を計算する
		 * @detail			ハッシュ値を計算する
		 */
		constexpr result_type operator()(const argument_type __key) const { return __key; };
	};

	template<>
	class Hash<unsigned long long> {
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		Hash() = default;
		/*!
		 * @brief				デストラクタ
		 */
		~Hash() = default;

		using result_type = unsigned long long;
		using argument_type = unsigned long long;

		/*!
		 * @brief			ハッシュ値を計算する
		 * @detail			ハッシュ値を計算する
		 */
		constexpr result_type operator()(const argument_type __key) const { return __key; };
	};

	template<typename __Id>
	class Hash<okl::Vertex<__Id>> {
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		Hash() = default;
		/*!
		 * @brief				デストラクタ
		 */
		~Hash() = default;

		using argument_type = okl::Vertex<__Id>;
		using result_type = typename okl::Vertex<__Id>::id_type;

		/*!
		 * @brief			ハッシュ値を計算する
		 * @detail			ハッシュ値を計算する
		 */
		constexpr result_type operator()(const argument_type& __key) const { return __key.id(); };
	};
} /* namespace okl */