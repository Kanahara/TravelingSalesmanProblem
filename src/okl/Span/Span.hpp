#pragma once

namespace okl {
	template<typename __Iterator>
	class Span {
	public:
		/*!
		 * @brief				�R���X�g���N�^
		 */
		Span(const __Iterator __begin, const __Iterator __end)
			: begin_(__begin), end_(__end) {}

		/*!
		 * @brief				�f�X�g���N�^
		 */
		virtual ~Span() = default;

		using iterator = __Iterator;
		using const_iterator = const iterator;
		using size_type = typename iterator::difference_type;
		using value_type = typename iterator::value_type;
		using key_type = size_type;

		/*!
		 * @brief			�擪�C�e���[�^��Ԃ�
		 * @return			iterator	�擪�C�e���[�^��Ԃ�
		 * @detail			�擪�̗v�f�̃C�e���[�^��Ԃ�
		 */
		iterator begin()
		{
			return this->begin_;
		}

		/*!
		 * @brief			�����̎��̃C�e���[�^��Ԃ�
		 * @return			iterator	�����̎��̃C�e���[�^��Ԃ�
		 * @detail			�����̗v�f�̎��̃C�e���[�^��Ԃ�
		 */
		iterator end()
		{
			return this->end_;
		}

		/*!
		 * @brief			�����̃C�e���[�^��Ԃ�
		 * @return			iterator	�����̃C�e���[�^��Ԃ�
		 * @detail			�����̗v�f�̃C�e���[�^��Ԃ�
		 */
		iterator back()
		{
			return this->end() - 1;
		}

		/*!
		 * @brief			�ǂݎ���p�擪�C�e���[�^��Ԃ�
		 * @return			iterator	�ǂݎ���p�擪�C�e���[�^��Ԃ�
		 * @detail			�擪�̗v�f�̓ǂݎ���p�C�e���[�^��Ԃ�
		 */
		constexpr const_iterator begin() const
		{
			return this->begin_;
		}

		/*!
		 * @brief			�ǂݎ���p�����̎��̃C�e���[�^��Ԃ�
		 * @return			iterator	�ǂݎ���p�����̎��C�e���[�^��Ԃ�
		 * @detail			�����̗v�f�̎��̃C�e���[�^��Ԃ�
		 */
		constexpr const_iterator end() const
		{
			return this->end_;
		}

		/*!
		 * @brief			�����̃C�e���[�^��Ԃ�
		 * @return			iterator	�����̃C�e���[�^��Ԃ�
		 * @detail			�����̗v�f�̃C�e���[�^��Ԃ�
		 */
		constexpr const_iterator back() const
		{
			return this->end() - 1;
		}

		/*!
		 * @brief			�ǂݎ���p�擪�C�e���[�^��Ԃ�
		 * @return			iterator	�ǂݎ���p�擪�C�e���[�^��Ԃ�
		 * @detail			�擪�̗v�f�̓ǂݎ���p�C�e���[�^��Ԃ�
		 */
		constexpr const_iterator cbegin() const
		{
			return this->begin();
		}

		/*!
		 * @brief			�ǂݎ���p�����̎��̃C�e���[�^��Ԃ�
		 * @return			iterator	�ǂݎ���p�����̎��C�e���[�^��Ԃ�
		 * @detail			�����̗v�f�̎��̃C�e���[�^��Ԃ�
		 */
		constexpr const_iterator cend() const
		{
			return this->end();
		}

		/*!
		 * @brief			�擪�C�e���[�^��ݒ�
		 * @param[in]		__begin �擪�C�e���[�^
		 * @detail			�擪�C�e���[�^��ݒ肷��
		 * 					- �����^iterator�̓e���v���[�g__Iterator�ɂ���Ē�`�����
		 */
		inline void setBegin(iterator __begin)
		{
			this->begin_ = __begin;
		}

		/*!
		 * @brief			�����̎��C�e���[�^��ݒ�
		 * @param[in]		__end �����̎��C�e���[�^
		 * @detail			�����̎��C�e���[�^��ݒ肷��
		 * 					- �����^iterator�̓e���v���[�g__Iterator�ɂ���Ē�`�����
		 */
		inline void setEnd(iterator __end)
		{
			this->end_ = __end;
		}

		/*!
		 * @brief			��W��������
		 * @return			bool	�v�f��0�Ȃ�True�@�󂶂�Ȃ��Ȃ�False
		 * @detail			��W����������s��
		 */
		constexpr bool isEmpty() const
		{
			return this->size() == 0;
		}

		/*!
		 * @brief			�v�f����Ԃ�
		 * @return			size_type	�v�f����Ԃ�
		 * @detail			�W�����̗v�f����Ԃ�
		 */
		constexpr size_type size() const
		{
			return std::distance(this->begin_, this->end_);
		}

		/*!
		 * @brief			�w��̃C���f�b�N�X�ɓ����Ă���v�f���擾
		 * @detail			�w��̃C���f�b�N�X�ɓ����Ă���v�f���擾����
		 * @param[in]		__index �i�[�ԍ�
		 * @return			value_type&	�w��v�f�̎Q�Ƃ�Ԃ�
		 * 					- �v�f�^value_type�̓e���v���[�g__Value�ɂ���Ē�`�����
		 */
		const value_type& operator[](const key_type __index) const&
		{
			assert(__index < this->size());
			return *(this->begin_ + __index);
		}

		/*!
		 * @brief			�w��̃C���f�b�N�X�ɓ����Ă���v�f���擾
		 * @detail			�w��̃C���f�b�N�X�ɓ����Ă���v�f���擾����
		 * @param[in]		__index �i�[�ԍ�
		 * @return			value_type&	�w��v�f�̎Q�Ƃ�Ԃ�
		 * 					- �v�f�^value_type�̓e���v���[�g__Value�ɂ���Ē�`�����
		 */
		inline value_type operator[](const key_type __index)&&
		{
			assert(__index < this->size());
			return std::move(*(this->begin_ + __index));
		}

	private:
		//!�擪�C�e���[�^
		iterator begin_;

		//!�����̎��̃C�e���[�^
		iterator end_;
	};
} /* namespace okl */