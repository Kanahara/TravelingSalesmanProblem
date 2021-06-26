#pragma once
#include "../Condition.hpp"
#include <cstdint>
namespace okl {
	/*!
	 *  @brief      設定回数条件がTrueかどうか判定
	 *  @details    設定回数条件がTrueかどうか判定する
	 *				設定回数条件がTrueならTrue　一度でも変化したなら回数を数えなおし
	 *				- __Condition				条件クラス
	 *				- __N						設定回数
	 */
	template<class __Condition, std::size_t __N>
	class SetTimesSatisfyCondition : public Condition
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		SetTimesSatisfyCondition() : is_first_(true), satisfy_count_(0) {};

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~SetTimesSatisfyCondition() = default;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return std::to_string(__N) + "回<" + __Condition::info() + ">がTrueなら"; }

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 */
		inline void clear() override
		{
			Condition::clear();
			this->is_first_ = true;
			this->satisfy_count_ = 0;
			this->condition_.clear();
		}

		/*!
		 * @brief			サイズ設定
		 * @param[in]		__size 頂点数
		 * @detail			全要素をメモリ確保を行う
		 */
		inline void resize(const size_type __size) override
		{
			Condition::resize(__size);
			this->condition_.resize(__size);
		}

		/*!
		 * @brief			シードを設定
		 * @param[in]		__seed シード
		 * @detail			シードを設定する
		 * 					- seed_typeは標準ライブラリによって定義される
		 */
		inline void seed(const seed_type __seed) override
		{
			Condition::seed(__seed);
			this->condition_.seed(this->random_device_.uniform_int(std::numeric_limits < std::seed_seq::result_type >::min(), std::numeric_limits < std::seed_seq::result_type >::max()));
		}

		/*!
		 * @brief			クラスに関連付けられた演算を実行
		 * @return			bool	現在のTrue　Falseを返す
		 * @detail			クラスに関連付けられた演算を実行する
		 */
		template<class __SearchInfo, class __Solution>
		inline bool operator()(const typename __Solution::graph_type& __graph, __SearchInfo& __search_info, const __Solution& __solution, const __Solution& __initial_solution, const __Solution& __best_solution)
		{
			if (this->condition_(__graph, __search_info, __solution, __initial_solution, __best_solution) != true || this->is_first_) {
				this->satisfy_count_ = 0;
				this->is_first_ = false;
			}
			else {
				this->satisfy_count_++;
			}

			if (this->satisfy_count_ < __N) {
				return false;
			}
			else {
				this->satisfy_count_ = 0;
				return true;
			}
		}

	protected:
		//!一回目か判定する変数
		bool is_first_;

		//!条件がTrueの回数
		std::size_t satisfy_count_;

		//!条件クラス
		__Condition condition_;
	};
} /* namespace okl */