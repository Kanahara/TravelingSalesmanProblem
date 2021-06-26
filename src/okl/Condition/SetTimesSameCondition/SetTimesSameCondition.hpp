#pragma once
#include "../Condition.hpp"
#include <cstdint>
namespace okl {
	/*!
	 *  @brief      設定回数条件が一緒かどうか判定
	 *  @details    設定回数条件が一緒かどうか判定する
	 *				設定回数条件が一緒ならTrue　一度でも変化したなら回数を数えなおし
	 *				- __Condition				条件クラス
	 */
	template<class __Condition>
	class SetTimesSameCondition : public Condition
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		SetTimesSameCondition() : is_first_(true), before_state_(true), same_count_(0) {};

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~SetTimesSameCondition() = default;

		using seed_type = typename Condition::seed_type;
		using random_device_type = typename Condition::random_device_type;
		using size_type = typename Condition::size_type;

		/*!
		 * @brief				アルゴリズム名
		 * @return				std::string	このアルゴリズムの名前を返す
		 */
		static const std::string info() { return "指定回数(" + __Condition::info() + ")が同じなら"; }

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 */
		inline void clear() override
		{
			Condition::clear();
			this->is_first_ = true;
			this->before_state_ = true;
			this->same_count_ = 0;
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
		inline bool operator()(const typename __Solution::graph_type& __graph, const __SearchInfo& __search_info, const __Solution& __solution)
		{
			bool _state = this->condition_(__graph, __search_info, __solution);
			if (this->before_state_ != _state || this->is_first_) {
				this->same_count_ = 1;
				this->is_first_ = false;
			}
			else {
				this->same_count_++;
			}
			this->before_state_ = _state;

			if (this->same_count_ <= __search_info.condition_monitoring_times()) {
				return false;
			}
			else {
				this->same_count_ = 1;
				return true;
			}
		}
		template<class __SearchInfo, class __Solution>
		inline bool operator()(const typename __Solution::graph_type& __graph, const __SearchInfo& __search_info, const __Solution& __solution, const __Solution& __initial_solution, const __Solution& __best_solution)
		{
			return (*this)(__graph, __search_info, __solution);
		}

	protected:
		//!一回目か判定する変数
		bool is_first_;

		//!前の状態
		bool before_state_;

		//!条件が同じ回数
		std::size_t same_count_;

		//!条件クラス
		__Condition condition_;
	};
} /* namespace okl */