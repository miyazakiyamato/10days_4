#include "AudioManager.h"
#include <cassert>

namespace Engine {

std::unique_ptr<AudioManager> AudioManager::instance = nullptr;

AudioManager* AudioManager::GetInstance()
{
	if (instance == nullptr) {
		instance = std::make_unique<AudioManager>(PrivateToken{});
	}
	return instance.get();
}

void AudioManager::Initialize(){
	HRESULT result;

	//xAudioエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));

	//マスターボイスを生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result));
}

AudioManager::~AudioManager() {
	//xAudio2解放
	xAudio2.Reset();
	//音声データ解放
	soundData.clear();
}

void AudioManager::Finalize(){
	instance.reset();
}

void AudioManager::LoadWave(const std::string& filePath)
{
	//読み込み済みテクスチャを検索
	if (soundData.contains(filePath)) {
		//読み込み済みなら早期return
		return;
	}

	const std::string filename = audioFilePath + filePath;

	//ファイル入力ストリームのインスタンス
	std::ifstream file;
	//.wavファイルをバイナリモードで開く
	file.open(filename, std::ios_base::binary);
	//ファイルオープン失敗を検出する
	assert(file.is_open());

	//RIFFヘッダーの読み込み
	RiffHeader riff{};
	file.read((char*)&riff, sizeof(riff));
	//ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	//タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}
	// Formatチャンクの読み込み
	FormatChunk format{};
	while (file.good()) {
		// チャンクヘッダーの確認
		file.read((char*)&format, sizeof(ChunkHeader));
		if (file.eof()) {
			assert(0); // チャンクが見つからずファイルの終わりに到達した場合
		}
		if (strncmp(format.chunk.id, "fmt ", 4) == 0) {
			// チャンク本体の読み込み（16バイト、18バイト、または40バイトの場合の処理）
			if (format.chunk.size == 16 || format.chunk.size == 18) {
				// 16バイトまたは18バイトのフォーマットを読み込む
				assert(format.chunk.size <= sizeof(format.fmt));
				file.read((char*)&format.fmt, format.chunk.size);
			} else if (format.chunk.size == 40) {
				// 40バイトのフォーマットの場合、最初の18バイトのみ読み込む
				file.read((char*)&format.fmt, 18);
				// 残りの22バイトを無視する（スキップ）
				file.ignore(static_cast<std::streamsize>(format.chunk.size) - 18);
			} else {
				// サポートしていないフォーマットサイズの場合
				assert(0);
			}
			break; // 正常にfmtチャンクを読み込んだらループを抜ける
		} else {
			// fmtチャンクでない場合はスキップ
			file.ignore(format.chunk.size);
		}
	}

	// Dataチャンクの読み込み
	ChunkHeader data{};
	while (file.good()) {
		file.read((char*)&data, sizeof(data));
		if (file.eof()) {
			assert(0); // チャンクが見つからずファイルの終わりに到達した場合
		}
		// JUNKやその他の無視すべきチャンクを検出した場合
		if (strncmp(data.id, "data", 4) == 0) {
			break; // data チャンクが見つかったらループを抜ける
		} else {
			// 不要なチャンクはスキップ
			file.ignore(data.size);
		}
	}

	//追加した音声データの参照を取得
	SoundData& waveData = soundData[filePath];

	waveData.wfex = format.fmt;
	waveData.buffer.resize(data.size);

	//Dataチャンクのデータ部(波型データ)の読み込み
	file.read(reinterpret_cast<char*>(waveData.buffer.data()), data.size);

	//Waveファイルを閉じる
	file.close();
}

void AudioManager::PlayWave(const std::string& filePath, float volume, bool loop){
	//音声データの参照を取得
	SoundData& waveData = soundData[filePath];

	HRESULT result;

	//波型フォーマットを元にSourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &waveData.wfex);
	assert(SUCCEEDED(result));

	//生成する波型データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = waveData.buffer.data();
	buf.AudioBytes = static_cast<UINT32>(waveData.buffer.size());
	buf.Flags = XAUDIO2_END_OF_STREAM;

	// ループの設定
	buf.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;

	//波型データの生成
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
	result = pSourceVoice->SetVolume(volume);

	if (loop)
	{
		if (playSoundData.contains(filePath)) {
			return;
		}
		playSoundData[filePath] = pSourceVoice;
	}
}

void AudioManager::StopWave(const std::string& filePath)
{
	IXAudio2SourceVoice* pSourceVoice = playSoundData[filePath];

	//波型データの生成
	pSourceVoice->Stop();

	playSoundData.erase(filePath);
}

} // namespace Engine
