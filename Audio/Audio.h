#pragma once
#include<Windows.h>
#include<xaudio2.h>
#include<wrl.h>

//オーディオコールバック
class XAudio2VoiceCallback : public IXAudio2VoiceCallback
{
public:
	// ボイス処理パスの開始時
	//STDMETHOD_(void, onVoiceProcessingPassStart) (THIS_ UINT32 BytesRequired) {};
	void OnVoiceProcessingPassStart(UINT32 BytesRequired) {};
	// ボイス処理パスの終了時
	STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS) {};
	// バッファストリームの再生が終了した時
	STDMETHOD_(void, OnStreamEnd) (THIS) {};
	// バッファの使用開始時
	STDMETHOD_(void, OnBufferStart) (THIS_ void* pBufferContext) {};
	// バッファの末尾に達した時
	STDMETHOD_(void, OnBufferEnd) (THIS_ void* pBufferContext) {
		// バッファを解放する
		delete[] pBufferContext;
	};
	// 再生がループ位置に達した時
	STDMETHOD_(void, OnLoopEnd) (THIS_ void* pBufferContext) {};
	// ボイスの実行エラー時
	STDMETHOD_(void, OnVoiceError) (THIS_ void* pBufferContext, HRESULT Error) {};
};

//音声データ
struct SoundData
{
	//波形フォーマット
	WAVEFORMATEX wfex;

	//バッファの先頭アドレス
	BYTE* pBuffer;

	//バッファのサイズ
	unsigned int bufferSize;

	//停止するかどうか
	bool IsStop;

	//ファイルパス
	const char* filename;

	//ソースボイス
	IXAudio2SourceVoice* pSourceVoice = nullptr;

	//バッファー
	XAUDIO2_BUFFER buf{};
};

//オーディオクラス
class Audio
{
private:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//音構造体
	//チャンクヘッダ
	struct ChunkHeader
	{
		char id[4];
		int  size;
	};

	//RIFFヘッダチャンク
	struct RiffHeader
	{
		ChunkHeader chunk;
		char type[4];
	};

	//FMTチャンク
	struct FormatChunk
	{
		ChunkHeader chunk;
		WAVEFORMATEX fmt;
	};

public:
	//デストラクタ
	~Audio();

	//初期化
	void Initialize();

	//サウンド読み込み
	SoundData loadSoundWave(const char* filename, SoundData& sound);

	//サウンド再生
	void playSoundWave(SoundData& soundData, bool IsLoop);

	//サウンド再生(音量指定)
	void playSoundWave_Val(SoundData& soundData, bool IsLoop, float val);

	//停止
	void stopSoundWave(SoundData& soundData);

	//一時停止
	void stopOnlySoundWave(SoundData& soundData);

	//音声データ解放
	void unLoadSoundWave(SoundData* soundData);

private:
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice = nullptr;
	XAudio2VoiceCallback voiceCallback;

	float valume = 0.5f;
};