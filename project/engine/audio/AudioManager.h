#pragma once
#include <xaudio2.h>
#include <fstream>
#include <wrl.h>
#include <vector>
#include <unordered_map>
#include <memory>

namespace Engine {

#pragma comment(lib, "xaudio2.lib")    // XAudio2のライブラリ

/// <summary>
/// オーディオマネージャクラス
/// XAudio2を使用した音声の読み込み・再生・管理を行うシングルトン
/// </summary>
class AudioManager {
private:
		// --- namespace省略 ---
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
		// --- 構造体定義 ---
	/// <summary>
	/// チャンクヘッダ
	/// ファイル内のデータブロック識別用
	/// </summary>
	struct ChunkHeader {
		char id[4];   // チャンク毎のID
		int32_t size; // チャンクサイズ
	};

	/// <summary>
	/// RIFFヘッダチャンク
	/// ファイル形式を示すヘッダ
	/// </summary>
	struct RiffHeader {
		ChunkHeader chunk; // "RIFF"
		char type[4];      // "WAVE"
	};

	/// <summary>
	/// FMTチャンク
	/// 音声フォーマット情報を格納する
	/// </summary>
	struct FormatChunk {
		ChunkHeader chunk; // "fmt "
		WAVEFORMATEX fmt;  // 波形フォーマット
	};

	/// <summary>
	/// 音声データ構造体
	/// 読み込んだ音声データのバッファとフォーマットを保持する
	/// </summary>
	struct SoundData {
		WAVEFORMATEX wfex;       // 波形フォーマット
		std::vector<uint8_t> buffer; // バッファデータ
	};

		// --- メンバ関数 ---
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンスへのポインタ</returns>
	static AudioManager* GetInstance();

	/// <summary>
	/// 初期化処理
	/// XAudio2エンジンの作成、マスターボイスの生成を行う
	/// </summary>
	void Initialize();

	/// <summary>
	/// 終了処理
	/// 読み込んだ音声データの開放やXAudio2の破棄を行う
	/// </summary>
	void Finalize();

	/// <summary>
	/// WAVEファイルの読み込み
	/// 指定されたパスの音声ファイルをメモリにロードする
	/// </summary>
	/// <param name="filePath">読み込むファイルのパス (resources/audio/以下の相対パス)</param>
	void LoadWave(const std::string& filePath);

	/// <summary>
	/// WAVEファイルの再生
	/// </summary>
	/// <param name="filePath">再生するファイルのキーとなるパス</param>
	/// <param name="volume">音量 (0.0f ～ 1.0f, デフォルト1.0f)</param>
	/// <param name="loop">ループ再生するかどうか (デフォルト false)</param>
	void PlayWave(const std::string& filePath, float volume = 1.0f, bool loop = false);

	/// <summary>
	/// 音声停止
	/// 指定された音声の再生を停止し、ソースボイスを破棄する
	/// </summary>
	/// <param name="filePath">停止するファイルのキーとなるパス</param>
	void StopWave(const std::string& filePath);

private:
	// 外部からは絶対にインスタンス化できない「鍵」となる構造体を定義
	struct PrivateToken {};
public:
	// シングルトン(make_uniqueする用)
	explicit AudioManager(PrivateToken) {}
	~AudioManager();
	// コピー・ムーブの禁止
	AudioManager(const AudioManager&) = delete;
	AudioManager& operator=(const AudioManager&) = delete;

private:
		// --- シングルトン ---
	static std::unique_ptr<AudioManager> instance;
	
		// --- メンバ変数 ---
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice = nullptr;

	// 音声ファイルのルートパス
	const std::string audioFilePath = "resources/audio/";

	// 音声データ格納コンテナ (読み込み済みデータ)
	std::unordered_map<std::string, SoundData> soundData;

	// 再生中データ格納コンテナ (ソースボイス)
	std::unordered_map<std::string, IXAudio2SourceVoice*> playSoundData;
};

} // namespace Engine
