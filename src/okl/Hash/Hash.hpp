#include <cstdint>
#include "../Vertex/module.hpp"

namespace okl {
	template<class __T>
	class Hash {
	public:
		/*!
		 * @brief				�R���X�g���N�^
		 */
		Hash() = default;

		/*!
		 * @brief				�f�X�g���N�^
		 */
		~Hash() = default;

		using result_type = std::size_t;

		/*!
		 * @brief			�n�b�V���l���v�Z����
		 * @detail			�n�b�V���l���v�Z����
		 */
		result_type operator()(const __T& __key);
	};

	template<>
	class Hash<unsigned char> {
	public:
		/*!
		 * @brief				�R���X�g���N�^
		 */
		Hash() = default;
		/*!
		 * @brief				�f�X�g���N�^
		 */
		~Hash() = default;

		using result_type = unsigned char;
		using argument_type = unsigned char;

		/*!
		 * @brief			�n�b�V���l���v�Z����
		 * @detail			�n�b�V���l���v�Z����
		 */
		constexpr result_type operator()(const argument_type __key) const { return __key; };
	};

	template<>
	class Hash<unsigned short> {
	public:
		/*!
		 * @brief				�R���X�g���N�^
		 */
		Hash() = default;
		/*!
		 * @brief				�f�X�g���N�^
		 */
		~Hash() = default;

		using result_type = unsigned short;
		using argument_type = unsigned short;

		/*!
		 * @brief			�n�b�V���l���v�Z����
		 * @detail			�n�b�V���l���v�Z����
		 */
		constexpr result_type operator()(const argument_type __key) const { return __key; };
	};

	template<>
	class Hash<unsigned int> {
	public:
		/*!
		 * @brief				�R���X�g���N�^
		 */
		Hash() = default;
		/*!
		 * @brief				�f�X�g���N�^
		 */
		~Hash() = default;

		using result_type = unsigned int;
		using argument_type = unsigned int;

		/*!
		 * @brief			�n�b�V���l���v�Z����
		 * @detail			�n�b�V���l���v�Z����
		 */
		constexpr result_type operator()(const argument_type __key) const { return __key; };
	};

	template<>
	class Hash<unsigned long> {
	public:
		/*!
		 * @brief				�R���X�g���N�^
		 */
		Hash() = default;
		/*!
		 * @brief				�f�X�g���N�^
		 */
		~Hash() = default;

		using result_type = unsigned long;
		using argument_type = unsigned long;

		/*!
		 * @brief			�n�b�V���l���v�Z����
		 * @detail			�n�b�V���l���v�Z����
		 */
		constexpr result_type operator()(const argument_type __key) const { return __key; };
	};

	template<>
	class Hash<unsigned long long> {
	public:
		/*!
		 * @brief				�R���X�g���N�^
		 */
		Hash() = default;
		/*!
		 * @brief				�f�X�g���N�^
		 */
		~Hash() = default;

		using result_type = unsigned long long;
		using argument_type = unsigned long long;

		/*!
		 * @brief			�n�b�V���l���v�Z����
		 * @detail			�n�b�V���l���v�Z����
		 */
		constexpr result_type operator()(const argument_type __key) const { return __key; };
	};

	template<typename __Id>
	class Hash<okl::Vertex<__Id>> {
	public:
		/*!
		 * @brief				�R���X�g���N�^
		 */
		Hash() = default;
		/*!
		 * @brief				�f�X�g���N�^
		 */
		~Hash() = default;

		using argument_type = okl::Vertex<__Id>;
		using result_type = typename okl::Vertex<__Id>::id_type;

		/*!
		 * @brief			�n�b�V���l���v�Z����
		 * @detail			�n�b�V���l���v�Z����
		 */
		constexpr result_type operator()(const argument_type& __key) const { return __key.id(); };
	};
} /* namespace okl */