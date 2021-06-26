#pragma once
#include "../Operator.hpp"

namespace okl {
	/*!
	 *  @brief      選択淘汰処理関数基底クラス
	 *  @details    遺伝的アルゴリズムなどで使用される選択淘汰処理を行う関数基底クラス
	 */
	class SelectionOperator : public Operator
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		SelectionOperator() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~SelectionOperator() = default;

		using random_device_type = typename Operator::random_device_type;
		using seed_type = typename Operator::seed_type;
		using size_type = typename Operator::size_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "選択"; }
	};
} /* namespace okl::pmp::cpmp */