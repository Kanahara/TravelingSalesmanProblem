#pragma once

#include <cstdint>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include "../Timer/module.hpp"
#include "../Result/module.hpp"

namespace okl {
	/*!
	 *  @brief      探索用情報クラス
	 *  @details    探索用情報クラス
	 *				探索のための情報
	 *				探索による情報を蓄積する
	 *				クラス内実装で解決できない変数等を扱う
	 *				- __Result				結果クラス
	 *				- __Info				分析用時系列データ型
	 */
	template<
		class __Result,
		class __Info>
		class SearchInfo
	{
	public:
		/*!
		 * @brief				コンストラクタ
		 */
		SearchInfo()
		{
			//静的パラメーター
			this->repeat_times_ = 0;
			this->best_known_cost_ = 0;
			this->run_time_ = 0;
			this->pop_size_ = 0;
			this->result_file_path_ = "";
			this->log_file_path_ = "";

			this->clear();
		}

		/*!
			 * @brief				デストラクタ
		 */
		virtual ~SearchInfo() = default;

		using timer_type = Timer;
		using info_type = __Info;
		using history_container_type = std::vector<info_type>;
		using result_type = __Result;
		using size_type = std::size_t;
		using time_type = typename result_type::time_type;
		using cost_type = typename result_type::cost_type;
		using large_cost_type = std::size_t;
		using mode_type = std::uint_least8_t;
		using key_type = std::uint_least8_t;

		/*!
		 * @brief			全要素初期化
		 * @detail			全要素を初期化処理を行う
		 *					メモリ解放は行わない
		 */
		inline virtual void clear()
		{
			this->search_stagnation_times_ = 0;
			this->condition_monitoring_times_ = 0;
			this->timer_.clear();
			this->history_.clear();
			this->best_result_.clear();
			std::fill(this->counts_.begin(), this->counts_.end(), 0);
			this->mode_ = std::numeric_limits<mode_type>::max();
			std::fill(this->modes_cost_sum_.begin(), this->modes_cost_sum_.end(), 0);
			std::fill(this->modes_count_.begin(), this->modes_count_.end(), 0);
			std::fill(this->modes_above_avg_count_.begin(), this->modes_above_avg_count_.end(), 0);
			this->all_count_ = 0;
			this->all_cost_sum_ = 0;
		}

		/*!
		 * @brief			サイズ設定
		 * @param[in]		__size 頂点数
		 * @detail			全要素をメモリ確保を行う
		 */
		inline virtual void resize(const size_type __size)
		{
			this->history_.reserve(std::numeric_limits<std::uint_least8_t>::max());
			this->counts_.resize(std::numeric_limits<std::uint_least8_t>::max(), 0);
			this->modes_cost_sum_.resize(std::numeric_limits<std::uint_least8_t>::max(), 0);
			this->modes_count_.resize(std::numeric_limits<std::uint_least8_t>::max(), 0);
			this->modes_above_avg_count_.resize(std::numeric_limits<std::uint_least8_t>::max(), 0);
		}

		/*!
		 * @brief			内部のタイマーを計測を開始
		 * @detail			内部のタイマーを計測を開始する
		 */
		inline void startTime()
		{
			this->timer_.start();
		}

		/*!
		 * @brief			計測開始からの実時間を取得
		 * @detail			計測開始からの実時間を取得する
		 */
		constexpr time_type time() const
		{
			return this->timer_.template cpu_time< timer_type::seconds>();
		}

		/*!
		 * @brief			計測開始からのCPU時間を取得
		 * @detail			計測開始からのCPU時間を取得する
		 */
		constexpr time_type cpu_time() const
		{
			return this->timer_.template time< timer_type::seconds>();
		}

		/*!
		 * @brief			設定した繰り返し回数を取得
		 * @detail			設定した繰り返し回数を取得する
		 * 					ReachedRepeatTimes() で使用する
		 */
		constexpr size_type repeat_times() const
		{
			return this->repeat_times_;
		}

		/*!
		 * @brief			繰り返し回数を設定
		 * @detail			繰り返し回数を設定する
		 * 					ReachedRepeatTimes() で使用する
		 */
		inline void setRepeatTimes(const size_type __repeat_times)
		{
			this->repeat_times_ = __repeat_times;
		}

		/*!
		 * @brief			探索停滞回数を取得
		 * @detail			探索停滞回数を取得する
		 */
		constexpr size_type search_stagnation_times() const
		{
			return this->search_stagnation_times_;
		}

		/*!
		 * @brief			探索停滞回数を+1する
		 * @detail			探索停滞回数を+1する
		 */
		inline void countSearchStagnation()
		{
			++this->search_stagnation_times_;
		}

		/*!
		 * @brief			探索停滞回数をクリアする
		 * @detail			探索停滞回数を0にする
		 */
		inline void clearSearchStagnation()
		{
			this->search_stagnation_times_ = 0;
		}

		/*!
		 * @brief			条件監視回数を取得
		 * @detail			条件監視回数を取得する
		 * 					NTimesSameCondition() で使用する
		 */
		constexpr size_type condition_monitoring_times() const
		{
			return this->condition_monitoring_times_;
		}

		/*!
		 * @brief			条件監視回数を設定
		 * @detail			条件監視回数を設定する
		 * 					NTimesSameCondition() で使用する
		 */
		inline void setConditionMonitoringTimes(const size_type __condition_monitoring_times)
		{
			this->condition_monitoring_times_ = __condition_monitoring_times;
		}

		/*!
		 * @brief			算出結果を設定
		 * @param[in]		__cost 解の値
		 * @detail			算出結果を設定する
		 * 					- 数値型cost_typeはテンプレート__Resultのcost_typeによって定義される
		 */
		inline virtual void setResult(const cost_type __cost)
		{
			result_type _result;
			_result.set(__cost, this->cpu_time());
			if (__cost > 8062) {
				std::cout << "ERROR_d" << std::endl;
				char c; std::cin >> c;
			}

			if (this->best_result_ < _result) {
				this->best_result_ = _result;
			}
		}

		/*!
		 * @brief			解の値が最良の結果を取得
		 * @return			 result_type&	解の値が最良の結果の参照を返す
		 * @detail			解の値が最大の結果を取得する
		 * 					- result_typeはテンプレート__Resultによって定義される
		 */
		inline const result_type& best_result() const
		{
			return this->best_result_;
		}

		/*!
		 * @brief			既知の最良解の値を取得
		 * @return			cost_type	既知の最良解の値を返す
		 * @detail			既知の最良解の値を取得する
		 * 					CalculatedBestKnownCost() で使用する
		 * 					- 数値型cost_typeはテンプレート__Resultのcost_typeによって定義される
		 */
		constexpr cost_type best_known_cost() const
		{
			return this->best_known_cost_;
		}

		/*!
		 * @brief			既知の最良解の値を設定
		 * @detail			既知の最良解の値を設定する
		 * 					- 数値型cost_typeはテンプレート__Resultのcost_typeによって定義される
		 */
		inline virtual void setBestKnownCost(const cost_type __best_known_cost)
		{
			this->best_known_cost_ = __best_known_cost;
		}

		/*!
		 * @brief			ログ記録用アウトプットストリームを取得
		 * @return			std::ofstream	アウトプットストリーム
		 * @detail			ログ記録用アウトプットストリームを取得する
		 */
		std::ofstream log() const
		{
			std::ofstream log_("./output.log", std::ios::in | std::ios::out | std::ios::app);
			return log_;
		}

		/*!
		 * @brief			指定したキーに関連付けられているデータをクリア
		 * @param[in]		__key キー
		 * @detail			指定したキーに関連付けられているデータをクリアする
		 */
		inline void clearKey(const key_type __key)
		{
			this->counts_[__key] = 0;
		}

		/*!
		 * @brief			指定したキーをカウントアップ(+1)する
		 * @param[in]		__key キー
		 * @detail			指定したキーをカウントアップ(+1)する
		 *					アルゴリズムの分析に用いる
		 */
		inline void countKey(const key_type __key)
		{
			++this->counts_[__key];
		}

		/*!
		 * @brief			指定したキーに関連付けられているデータを取得
		 * @param[in]		__key キー
		 * @return			std::size_t	キーに関連付けられているデータを返す
		 * @detail			指定したキーに関連付けられているデータを取得する
		 *					アルゴリズムの分析に用いる
		 */
		constexpr std::size_t key_data(const key_type __key) const
		{
			return this->counts_[__key];
		}

		/*!
		 * @brief			探索時系列情報を設定
		 * @param[in]		__info 情報
		 * @detail			探索時系列情報を履歴に設定する
		 *					アルゴリズムの分析に用いる
		 * 					- info_typeはテンプレート__Infoによって定義される
		 */
		inline void setInfo(const info_type& __info)
		{
			this->history_.push_back(__info);
		}

		/*!
		 * @brief			探索時系列履歴を取得
		 * @detail			探索時系列情報を取得する
		 * @return			history_container_type&	探索時系列履歴の参照を返す
		 *					アルゴリズムの分析に用いる
		 * 					- history_container_typeはinfo_typeの配列によって定義される
		 */
		inline const history_container_type& history() const
		{
			return this->history_;
		}

		/*!
		 * @brief			母集団サイズを取得
		 * @detail			母集団サイズを取得する
		 */
		constexpr size_type pop_size() const
		{
			return this->pop_size_;
		}

		/*!
		 * @brief			母集団サイズを設定
		 * @param[in]		__size サイズ
		 * @detail			母集団サイズを設定する
		 */
		inline void setPopSize(size_type __size)
		{
			this->pop_size_ = __size;
		}

		/*!
		 * @brief			実行時間を取得
		 * @detail			実行時間を取得する
		 */
		constexpr double run_time() const
		{
			return this->run_time_;
		}

		/*!
		 * @brief			実行時間を設定
		 * @param[in]		__time 時間
		 * @detail			実行時間を設定する
		 */
		inline void setRunTime(double __time)
		{
			this->run_time_ = __time;
		}

		/*!
		 * @brief			現在のモードを取得
		 * @detail			現在のモードを取得する
		 */
		constexpr mode_type mode() const
		{
			return this->mode_;
		}

		/*!
		 * @brief			モードを設定
		 * @param[in]		__mode 情報
		 * @detail			モードを設定する
		 *					動作の切り替えに用いる
		 */
		inline void setMode(const mode_type __mode)
		{
			this->mode_ = __mode;
		}

		/*!
		 * @brief			指定モードのコスト選択回数を取得
		 * @detail			指定モードのコスト選択回数を取得する
		 */
		constexpr std::size_t mode_count(const mode_type __mode) const
		{
			return this->modes_count_[__mode];
		}

		/*!
		 * @brief			全てのモードのコスト選択回数を取得
		 * @detail			全てのモードのコスト選択回数を取得する
		 */
		constexpr size_type all_count() const
		{
			return this->all_count_;
		}

		/*!
		 * @brief			指定モードのコスト合計を取得
		 * @detail			指定モードのコスト合計する
		 */
		constexpr large_cost_type mode_cost_sum(const mode_type __mode) const
		{
			return this->modes_cost_sum_[__mode];
		}

		/*!
		 * @brief			コスト平均を取得
		 * @detail			コスト平均を取得する
		 */
		constexpr double cost_avg() const
		{
			if (this->all_count_ == 0) {
				return 0.0;
			}
			else {
				return static_cast<double>(this->all_cost_sum_) / static_cast<double>(this->all_count_);
			}
		}

		/*!
		 * @brief			指定モードのコスト平均を取得
		 * @detail			指定モードのコスト平均を取得する
		 */
		constexpr double mode_cost_avg(const mode_type __mode) const
		{
			return static_cast<double>(this->mode_cost_sum(__mode)) / static_cast<double>(this->mode_count(__mode));
		}

		/*!
		 * @brief			指定モードの平均コスト以上回数を取得
		 * @detail			指定モードの平均コスト以上回数を取得する
		 */
		constexpr size_type mode_cost_above_avg_count(const mode_type __mode) const
		{
			return this->modes_above_avg_count_[__mode];
		}

		/*!
		 * @brief			指定モードのコスト評価値合計を取得
		 * @detail			指定モードのコスト評価値合計を取得する
		 */
		inline virtual double mode_expected_score_sum(const mode_type __mode) const
		{
			return static_cast<double>(this->mode_cost_sum(__mode));
		}

		/*!
		 * @brief			指定モードのコスト評価値平均を取得
		 * @detail			指定モードのコスト評価値平均を取得する
		 */
		inline double mode_expected_score_avg(const mode_type __mode) const
		{
			return static_cast<double>(this->mode_expected_score_sum(__mode)) / static_cast<double>(this->mode_count(__mode));
		}

		/*!
		 * @brief			現在のモードでのコストの回数をカウント
		 */
		inline void countMode()
		{
			++this->modes_count_[this->mode()];
		}

		/*!
		 * @brief			現在のモードでのコストを設定
		 * @param[in]		__cost 解の値
		 * @detail			モードごと算出結果を設定する
		 * 					- 数値型cost_typeはテンプレート__Resultのcost_typeによって定義される
		 */
		inline void setModeCost(const cost_type& __cost)
		{
			if (this->cost_avg() <= static_cast<double>(__cost)) {
				++this->modes_above_avg_count_[this->mode()];
			}
			++this->all_count_;
			const auto _cost = static_cast<large_cost_type>(__cost);
			this->all_cost_sum_ += _cost;

			this->modes_cost_sum_[this->mode()] += _cost;
		}

		/*!
		 * @brief			結果をファイルに書き込むパスを設定する
		 * @param[in]		__path ファイルパス
		 * @detail			ファイルに書き込む　パスにファイルの先頭名も含むことが可能
		 *					例:/path/path/aaa_		aaa_XXX.xxxとなる
		 */
		inline void setResultOutputPath(const std::string __path)
		{
			this->result_file_path_ = __path;
			this->result_out_.open(__path + ".csv");
		}

		/*!
		 * @brief			ログをファイルに書き込むパスを設定する
		 * @param[in]		__path ファイルパス
		 * @detail			ファイルに書き込む　パスにファイルの先頭名も含むことが可能
		 *					例:/path/path/aaa_		aaa_XXX.xxxとなる
		 */
		inline virtual void setLogOutputPath(const std::string __path)
		{
			this->log_file_path_ = __path;
			this->history_out_.open(__path + "_log_.csv");
			this->count_data_out_.open(__path + "_count_.csv");
			this->mode_count_out_.open(__path + "_mode_count_.csv");
			this->mode_cost_avg_out_.open(__path + "_mode_cost_avg_.csv");
		}

		/*!
		 * @brief			ログをすべて閉じる
		 * @detail			ログファイルをすべて閉じる
		 */
		inline virtual void closeLogFiles()
		{
			this->result_out_.close();
			this->history_out_.close();
			this->count_data_out_.close();
			this->mode_count_out_.close();
			this->mode_cost_avg_out_.close();
		}


		/*!
		 * @brief			ログを再オープンする
		 * @detail			ログファイルをすべて閉じ再オープンする
		 */
		inline virtual void reopenLogFiles() 
		{
			this->closeLogFiles();
			this->result_out_.open(this->result_file_path_ + ".csv");
			this->history_out_.open(this->log_file_path_ + "_log_.csv");
			this->count_data_out_.open(this->log_file_path_ + "_count_.csv");
			this->mode_count_out_.open(this->log_file_path_ + "_mode_count_.csv");
			this->mode_cost_avg_out_.open(this->log_file_path_ + "_mode_cost_avg_.csv");
		}

		/*!
		 * @brief			結果をファイルに書き込む
		 * @detail			結果をファイルに書き込む
		 */
		inline void writeResult()
		{
			const auto _cpu_time = this->cpu_time();
			this->result_out_ << this->best_result() << ", " << _cpu_time << std::endl;
		}

		/*!
		 * @brief			ログをファイルに書き込む
		 * @detail			ログをファイルに書き込む
		 */
		inline virtual void writeLog()
		{
			this->writeHistory();

			for (std::size_t _index = 0; _index < this->counts_.size(); ++_index) {
				if (this->key_data(static_cast<key_type>(_index)) != 0) {
					this->count_data_out_ << _index << "," << this->key_data(static_cast<key_type>(_index)) << std::endl;
				}
			}

			double _sum = 0.0;
			for (mode_type _mode = 0; _mode < std::numeric_limits<mode_type>::max(); ++_mode) {
				_sum += static_cast<double>(this->mode_count(_mode));
			}
			if (_sum != 0) {
				//モードごとの局所探索回数を記録
				for (std::uint_least8_t _mode = 0; _mode < std::numeric_limits<mode_type>::max(); ++_mode) {
					const auto _value = static_cast<double>(this->mode_count(_mode)) / _sum;
					if (_value != 0) {
						this->mode_count_out_ << static_cast<std::uint16_t>(_mode) << "," << std::fixed << std::setprecision(16) << _value << std::endl;
						this->mode_count_out_ << std::defaultfloat;
					}
				}

				//モードごとの平均コストを記録
				for (std::uint_least8_t _mode = 0; _mode < std::numeric_limits<mode_type>::max(); ++_mode) {
					const auto _value = static_cast<double>(this->mode_cost_avg(_mode)) / _sum;
					if (_value != 0) {
						this->mode_cost_avg_out_ << static_cast<std::uint16_t>(_mode) << "," << std::fixed << std::setprecision(16) << _value << std::endl;
						this->mode_cost_avg_out_ << std::defaultfloat;
					}
				}
			}
		}

		/*!
		* @brief			代入処理を行う
		* @param[in]		__other 代入元対象
		* @return			SearchInfo&	自分自身の参照を返す
		* @detail			自身の要素格納場所情報を削除し代入元の要素の情報を代入する
		*/
		SearchInfo& operator=(const SearchInfo& __other)
		{
			std::copy(__other.counts_.begin(), __other.counts_.end(), this->counts_.begin());
			this->timer_ = __other.timer_;
			this->repeat_times_ = __other.repeat_times();
			this->search_stagnation_times_ = __other.search_stagnation_times();
			this->condition_monitoring_times_ = __other.condition_monitoring_times();
			this->best_result_ = __other.best_result();
			this->best_known_cost_ = __other.best_known_cost();
			std::copy(__other.history_.begin(), __other.history_.end(), this->history_.begin());
			this->pop_size_ = __other.pop_size_;
			this->run_time_ = __other.run_time_;
			this->mode_ = __other.mode_;
			std::copy(__other.modes_cost_sum_.begin(), __other.modes_cost_sum_.end(), this->modes_cost_sum_.begin());
			std::copy(__other.modes_count_.begin(), __other.modes_count_.end(), this->modes_count_.begin());
			std::copy(__other.modes_above_avg_count_.begin(), __other.modes_above_avg_count_.end(), this->modes_above_avg_count_.begin());
			this->all_count_ = __other.all_count_;
			this->all_cost_sum_ = __other.all_cost_sum_;
			this->setResultOutputPath(__other.result_file_path_);
			this->setLogOutputPath(__other.log_file_path_);
			return (*this);
		}

	//protected:
		//!カウンター用　コンテナ
		std::vector<size_type> counts_;

	//private:
		//!タイマー
		timer_type timer_;

		//!繰り返し回数 clear関数では初期化されない
		size_type repeat_times_;

		//!探索停滞回数
		size_type search_stagnation_times_;

		//!条件監視回数
		size_type condition_monitoring_times_;

		//!コストが最良の結果を保持
		result_type best_result_;

		//!既知の最良解の値 clear関数では初期化されない
		cost_type best_known_cost_;

		//!履歴 コンテナ
		history_container_type history_;

		//!母集団サイズ clear関数では初期化されない
		size_type pop_size_;

		//!実行時間 clear関数では初期化されない
		double run_time_;

		//!現在のモード
		mode_type mode_;

		//!モードごとで算出されたコストの合計
		std::vector<large_cost_type> modes_cost_sum_;

		//!モードごとの回数
		std::vector<size_type> modes_count_;

		//!モードごとの設定時点平均コスト以上回数
		std::vector<size_type> modes_above_avg_count_;

		//!全てのモード呼び出しカウント
		size_type all_count_;

		//!全てのモードのコスト合計
		large_cost_type all_cost_sum_;

		//!結果出力パス clear関数では初期化されない
		std::string result_file_path_;

		//!結果ファイル出力ストリーム clear関数では初期化されない
		std::ofstream result_out_;

		//!ログ出力パス clear関数では初期化されない
		std::string log_file_path_;

		//!履歴ファイル出力ストリーム clear関数では初期化されない
		std::ofstream history_out_;

		//!カウントデータ出力ストリーム clear関数では初期化されない
		std::ofstream count_data_out_;

		//!モード選択回数出力ストリーム clear関数では初期化されない
		std::ofstream mode_count_out_;

		//!モード平均コスト出力ストリーム clear関数では初期化されない
		std::ofstream mode_cost_avg_out_;

		/*!
		 * @brief			履歴をファイルに書き込む
		 * @detail			履歴をファイルに書き込む
		 */
		template<std::size_t _N = 0>
		inline void writeHistory()
		{
			if constexpr (_N < std::tuple_size<__Info>::value) {
				for (const auto& _tuple : this->history()) {
					this->history_out_ << std::get<_N>(_tuple) << ",";
				}
				this->history_out_ << std::endl;

				this->writeHistory<_N + 1>();
			}
		}
	};
} /* namespace okl */