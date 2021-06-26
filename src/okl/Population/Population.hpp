#pragma once

#include "../HashSet/module.hpp"

namespace okl {
	/*!
	 *  @brief      結果クラス
	 *  @details      結果クラス
	 *				- __Individual			個体型
	 *				- __MaxPopulationSize	最大母集団サイズ
	 *				- __SolutionContainer	解コンテナ型
	 *				- __Set					ID集合型
	 */
	template<
		class __Individual,
		std::size_t __MaxPopulationSize,
		class __Individuals = std::vector<__Individual>,
		class __Set = okl::HashSet<typename __Individual::id_type>>
		class Population
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		Population() = default;

		/*!
		 * @brief				デストラクタ
		 */
		virtual ~Population() = default;

		using individual_type = __Individual;
		using individuals_type = __Individuals;
		using set_type = __Set;
		using id_type = typename set_type::value_type;
		using key_type = typename set_type::key_type;
		using size_type = typename set_type::size_type;

		/*!
		 * @brief			母集団の個体数を返す
		 * @return			size_type	個体数を返す
		 * @detail			母集団ないの生きている個体数(現世代+次世代)を返す
		 */
		constexpr size_type size() const
		{
			return static_cast<size_type>(this->present_generation().size() + this->next_generation().size());
		}

		/*!
		 * @brief			最大個体数を返す
		 * @return			size_type	最大個体数返す
		 * @detail			存在できる最大の個体数(__MaxPopulationSize)を返す
		 */
		constexpr size_type capacity() const
		{
			return __MaxPopulationSize;
		}

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 *					メモリ解放は行わない
		 */
		inline void clear()
		{
			std::for_each(this->individuals_.begin(), this->individuals_.end(),
				[](auto& _individual) {_individual.clear(); });
			this->present_generation_.clear();
			this->next_generation_.clear();
			this->corpses_.clear();
			for (id_type _individual_id = 0; _individual_id < static_cast<id_type>(__MaxPopulationSize); ++_individual_id) {
				this->corpses_.insert(_individual_id);
			}
		}

		/*!
		 * @brief			サイズ設定
		 * @param[in]		__size サイズ
		 * @detail			全要素をメモリ確保を行う
		 */
		inline void resize(const size_type __size)
		{
			this->individuals_.resize(__MaxPopulationSize);
			std::for_each(this->individuals_.begin(), this->individuals_.end(),
				[__size](auto& _individual) {_individual.resize(__size); });
			this->present_generation_.reserve(__size);
			this->next_generation_.reserve(__size);
			this->corpses_.reserve(__size);
			for (id_type _individual_id = 0; _individual_id < static_cast<id_type>(__MaxPopulationSize); ++_individual_id) {
				this->corpses_.insert(_individual_id);
			}
		}

		/*!
		 * @brief			現代番号集合を返す
		 * @param[in]		__individual_id 個体ID
		 * @return			set_type		現代番号集合の参照を返す
		 * @detail
		 *					 - key_typeはテンプレートkey_typeによって定義される
		 *					 - individual_typeはテンプレート__Individualによって定義される
		 */
		inline individual_type& individual(const key_type __individual_id)
		{
			assert(this->present_generation().contains(__individual_id) || this->next_generation().contains(__individual_id));
			return this->individuals_[__individual_id];
		}

		/*!
		 * @brief			現代番号集合を返す
		 * @return			set_type		現代番号集合の参照を返す
		 * @detail
		 *					 - set_typeはテンプレート__Setによって定義される
		 */
		inline const set_type& present_generation() const
		{
			return this->present_generation_;
		}

		/*!
		 * @brief			現代番号集合をソートする
		 * @detail
		 */
		inline void sortPresentGeneration()
		{
			this->present_generation_.sort();
		}

		/*!
		 * @brief			現代番号集合をソートする
		 * @param[in]		__compare 比較関数
		 * @detail			現代番号集合を比較関数によりソートする
		 */
		template<typename _Compare>
		inline void sortPresentGeneration(_Compare __compare)
		{
			this->present_generation_.sort(__compare);
		}

		/*!
		 * @brief			次世代番号集合を返す
		 * @return			set_type		次世代番号集合の参照を返す
		 * @detail
		 *					 - set_typeはテンプレート__Setによって定義される
		 */
		inline const set_type& next_generation() const
		{
			return this->next_generation_;
		}

		/*!
		 * @brief			次世代番号集合をソートする
		 * @detail
		 */
		inline void sortNextGeneration()
		{
			this->next_generation_.sort();
		}

		/*!
		 * @brief			次世代番号集合をソートする
		 * @param[in]		__compare 比較関数
		 * @detail			現代番号集合を比較関数によりソートする
		 */
		template<typename _Compare>
		inline void sortNextGeneration(_Compare __compare)
		{
			this->next_generation_.sort(__compare);
		}

		/*!
		 * @brief			集団未使用番号を返す
		 * @return			set_type		集団未使用番号(死んでいる個体番号)の参照を返す
		 * @detail
		 *					 - set_typeはテンプレート__Setによって定義される
		 */
		inline const set_type& corpses() const
		{
			return this->corpses;
		}

		/*!
		 * @brief			指定個体を殺す
		 * @param[in]		__individual_id 個体ID
		 * @detail			指定個体IDをcorpsesに移動する
		 *					 - key_typeはテンプレートkey_typeによって定義される
		 */
		inline void kill(const key_type __individual_id)
		{
			assert(this->present_generation().contains(__individual_id) || this->next_generation().contains(__individual_id));
			this->corpses_.insert(__individual_id);
			if (this->present_generation().contains(__individual_id)) {
				this->present_generation_.erase(__individual_id);
			}
			else if (this->next_generation().contains(__individual_id)) {
				this->next_generation_.erase(__individual_id);
			}
		}

		/*!
		 * @brief			新たな個体を返す
		 * @detail			新たな個体を作成可能な個体参照を返す
		 *					新たな個体IDをnext_generationに移動しcorpsesから削除する
		 *					 - individual_typeはテンプレート__Individualによって定義される
		 */
		inline individual_type& born()
		{
			const auto _individua_id = this->corpses_.back();
			this->corpses_.erase(_individua_id);
			this->next_generation_.insert(_individua_id);
			return this->individuals_[_individua_id];
		}

		/*!
		 * @brief			世代を進める
		 * @detail			次世代(next_generation)の個体IDを現世代(present_generation)にすべて移動する
		 */
		inline void advanceGeneration()
		{
			while (!this->next_generation().isEmpty())
			{
				const auto _individual_id = this->next_generation_.back();
				this->advanceGeneration(_individual_id);
			}
		}

		/*!
		 * @brief			世代を進める
		 * @param[in]		__individual_id 次世代個体ID
		 * @detail			次世代(next_generation)の指定個体IDを現世代(present_generation)に移動する
		 *					 - key_typeはテンプレートkey_typeによって定義される
		 */
		inline void advanceGeneration(const key_type __individual_id)
		{
			this->next_generation_.erase(__individual_id);
			this->present_generation_.insert(__individual_id);
		}

		/*!
		 * @brief			世代交代する
		 * @detail			現世代をすべて削除し、次世代(next_generation)の個体IDを現世代(present_generation)にすべて移動する
		 */
		inline void changeGeneration()
		{
			while (!this->present_generation_().isEmpty())
			{
				const auto _individual_id = this->present_generation_.back();
				this->present_generation_.erase(_individual_id);
				this->corpses_.insert(_individual_id);
			}
			this->advanceGeneration();
		}

	private:
		//!解集団
		individuals_type individuals_;

		//!現代番号集合
		set_type present_generation_;

		//!次世代番号集合
		set_type next_generation_;

		//!集団未使用番号(死んでいる個体番号)
		set_type corpses_;
	};
} /* namespace okl */