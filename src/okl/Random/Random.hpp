#pragma once
#include <random>
#include <algorithm>
#include <cassert>

namespace okl {
	/*!
	 *  @brief      乱数生成クラス
	 *  @details      乱数生成クラス
	 *				　乱数生成するための汎用的なクラス
	 *				　主にc++11以降で実装された乱数関連をクラスとしてラップしたもの
	 *				- __Engin			乱数生成器型
	 */
	template<class __RandomEngin = std::mt19937_64>
	class Random {
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		Random() : seed_(0) { this->engin_.seed(this->seed_); }

		/*!
		 * @brief				コンストラクタ
		 * @param[in]		__seed シード
		 * @detail			シードを設定するコンストラクタ
		 * 					- seed_typeは標準ライブラリによって定義される
		 */
		Random(const std::seed_seq::result_type __seed) : seed_(__seed) { this->engin_.seed(this->seed_); }

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~Random() = default;

		using random_engine = __RandomEngin;
		using seed_type = std::seed_seq::result_type;

		/*!
		 * @brief			設定されているシードを取得
		 * @detail			設定されているシードを取得する
		 */
		constexpr seed_type seed() const
		{
			return this->seed_;
		}

		/*!
		 * @brief			シードを設定
		 * @param[in]		__seed シード
		 * @detail			シードを設定する
		 * 					- seed_typeは標準ライブラリによって定義される
		 */
		inline void seed(const seed_type __seed)
		{
			this->seed_ = __seed;
			this->engin_.seed(seed_);
		}

		/*!
		 * @brief			整数一様分布乱数を返す
		 * @param[in]		__min 範囲の最小
		 * @param[in]		__max 範囲の最大
		 * @return			int_type　整数一様分布乱数を返す
		 * @detail			整数一様分布乱数を取得を設定する
		 * 					__min <= x <=　max の範囲のxを返す
		 * 					- __Int				整数型
		 */
		template<typename __Int>
		inline __Int uniform_int(const __Int __min, const __Int __max)
		{
			std::uniform_int_distribution<__Int > _uniform_int_distribution(__min, __max);
			return _uniform_int_distribution(this->engin_);
		}

		/*!
		 * @brief			実数一様分布乱数を返す
		 * @param[in]		__min 範囲の最小
		 * @param[in]		__max 範囲の最大
		 * @return			real_number_type　実数一様分布乱数を返す
		 * @detail			実数一様分布乱数を取得を設定する
		 * 					__min <= x <　max の範囲のxを返す
		 * 					- __Real			実数値型
		 */
		template<typename __Real>
		inline __Real uniform_real(const __Real __min, const __Real __max)
		{
			std::uniform_real_distribution<__Real> _uniform_real_distribution(__min, __max);
			return _uniform_real_distribution(this->engin_);
		}

		/*!
		 * @brief			確率pでTrueを返す
		 * @param[in]		__p Trueとなる確率
		 * @return			bool　確率pでTrue　確率(1-p)でFalseを返す
		 * @detail			確率pでTrue　確率(1-p)でFalseを返す
		 */
		inline bool bernoulli(const double __p)
		{
			std::bernoulli_distribution _bernoulli_distribution(__p);
			return _bernoulli_distribution(this->engin_);
		}

		/*!
		 * @brief			標準正規分布乱数を返す
		 * @param[in]		__u 平均
		 * @param[in]		__sigma 標準偏差
		 * @return			real_number_type　標準正規分布乱数を返す
		 * @detail			__u 平均　__sigma 標準偏差からなる標準正規分布乱数
		 * 					- __Real			実数値型
		 */
		template<typename __Real>
		inline __Real normal(const __Real __u, const __Real __sigma)
		{
			std::normal_distribution<__Real> _normal_distribution(__u, __sigma);
			return _normal_distribution(this->engin_);
		}

		/*!
		 * @brief			レヴィ分布乱数を返す
		 * @param[in]		__beta スケーリング指数
		 * @return			real_number_type　レヴィ分布乱数を返す
		 * @detail			レヴィ分布乱数を返す
		 * 					- __Real			実数値型
		 */
		template<typename __Real>
		inline __Real levy(const __Real __beta)
		{
			const auto a = std::pow((std::tgamma(1 + __beta) * std::sin(M_PI * __beta / 2)) / (std::tgamma((1 + __beta) / 2) * __beta * std::pow(2, (__beta - 1) / 2)), (1 / __beta));
			const auto u = this->uniform_real(0, std::pow(a, 2));
			const auto v = this->normal(0.0, 1.0);
			const auto s = u / (std::pow(std::abs(v), (1 / this->beta_)));
			return s;
		}

		/*!
		 * @brief			ガンマ分布乱数を返す
		 * @param[in]		__alpha スケーリング指数
		 * @param[in]		__beta スケーリング指数
		 * @return			real_number_type　ガンマ分布乱数を返す
		 * @detail			ガンマ分布乱数を返す
		 * 					- __Real			実数値型
		 */
		template<typename __Real>
		inline __Real gamma(const __Real __alpha, const __Real __beta)
		{
			std::gamma_distribution<> _gamma_distribution(__alpha, __beta);
			return _gamma_distribution(this->engin_);
		}

		/*!
		 * @brief			ベータ分布乱数を返す
		 * @param[in]		__alpha スケーリング指数
		 * @param[in]		__beta スケーリング指数
		 * @return			real_number_type　ベータ分布乱数を返す
		 * @detail			ベータ分布乱数を返す
		 * 					- __Real			実数値型
		 */
		template<typename __Real>
		inline __Real beta(const __Real __alpha, const __Real __beta)
		{
			const __Real _r1 = this->gamma(__alpha, 1.0);
			const __Real _r2 = this->gamma(__beta, 1.0);
			return _r1 / (_r1 + _r2);
		}

		/*!
		 * @brief			離散確率分布乱数を返す
		 * @return			int_type　選択インデックスを返す
		 * @detail			整数のインデックスごとに離散した確率分布を返す
		 * 					- int_typeはテンプレート__Intによって定義される
		 */
		template<typename _Key, typename _Iterator>
		inline _Key discrete(_Iterator __first, _Iterator __last)
		{
			std::discrete_distribution<_Key> _discrete_distribution(__first, __last);
			return _discrete_distribution(this->engin_);
		}

		/*!
		 * @brief			イテレータの範囲をシャッフルする
		 * @param[in]		__first 開始位置イテレータ
		 * @param[in]		__last 終了位置イテレータ
		 * @detail			イテレータで定義される__firstから__last-1までの要素をシャッフルする
		 */
		template<typename _RandomAccessIterator>
		inline void shuffle(_RandomAccessIterator __first, _RandomAccessIterator __last)
		{
			std::shuffle(__first, __last, this->engin_);
		}

		/*!
		 * @brief			コンテナからランダムに要素を一つ選択する
		 * @param[in]		__container 選択する要素コンテナ
		 * @return			const auto&　コンテナの要素を一つ参照型で返す
		 * @detail			コンテナからランダムに要素を一つ選択する
		 *					コンテナにsize()が定義されていないと使用することはできない
		 * 					- コンテナはテンプレート_Containerによって定義される
		 */
		template<class _Container>
		inline const auto& random_element(const _Container& __container)
		{
			assert(0 < __container.size());
			return __container[this->uniform_int(static_cast<typename _Container::size_type>(0), static_cast<typename _Container::size_type>(__container.size() - 1))];
		}

		/*!
		 * @brief			使用しているランダムエンジンを取得
		 * @return			random_engine　使用しているランダムエンジンを返す
		 * @detail			使用しているランダムエンジンを返す
		 * 					- random_engineはstd::mt19937_64が定義されている定義される
		 */
		inline random_engine& engin()
		{
			return this->engin_;
		}

	private:
		//!シード
		seed_type seed_;

		//!疑似乱数生成器
		random_engine engin_;
	};
} /* namespace okl */