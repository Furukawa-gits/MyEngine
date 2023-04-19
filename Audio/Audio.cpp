#include"Audio.h"
#include<fstream>
#include<cassert>

#pragma comment(lib,"xaudio2.lib")

//デストラクタ
Audio::~Audio()
{
	xAudio2.Reset();
}

//初期化
void Audio::Initialize()
{
	HRESULT result;

	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	result = xAudio2->CreateMasteringVoice(&masterVoice);
}

//サウンド読み込み
SoundData Audio::loadSoundWave(const char* filename, SoundData& sound)
{
	HRESULT result;

	//ファイル入力ストリームのインスタンス
	std::ifstream file;
	//.wavファイルをバイナリモードで開く
	file.open(filename, std::ios_base::binary);
	//ファイルオープン失敗を検出
	assert(file.is_open());


	RiffHeader riff;
	FormatChunk format = {};
	ChunkHeader data;

	//RIFFヘッダーの読み込み
	file.read((char*)&riff, sizeof(riff));

	//ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}

	//タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}

	//Formatチャンクの読み込み
	file.read((char*)&format, sizeof(ChunkHeader));

	//JUNKチャンクを検出した場合
	if (strncmp(format.chunk.id, "JUNK ", 4) == 0)
	{
		file.seekg(format.chunk.size, std::ios_base::cur);
		file.read((char*)&format, sizeof(ChunkHeader));
	}

	//チャンクヘッダーの確認
	if (strncmp(format.chunk.id, "fmt ", 4) != 0)
	{
		assert(0);
	}

	//チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	//JUNKチャンクを検出した場合
	if (strncmp(format.chunk.id, "JUNK ", 4) == 0)
	{
		file.seekg(format.chunk.size, std::ios_base::cur);
		file.read((char*)&format.fmt, format.chunk.size);
	}

	//Dataチャンクの読み込み
	file.read((char*)&data, sizeof(data));

	//JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK ", 4) == 0)
	{
		file.seekg(data.size, std::ios_base::cur);

		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data ", 4) != 0)
	{
		file.seekg(data.size, std::ios_base::cur);

		file.read((char*)&data, sizeof(data));
	}

	//Dataチャンクのデータ部(波形データ)の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK ", 4) == 0)
	{
		file.seekg(data.size, std::ios_base::cur);
		file.read((char*)&data, sizeof(data));
	}

	//ファイルを閉じる
	file.close();

	//returnするための音声データ
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	//波形フォーマットをもとにサウンドボイスの再生
	result = xAudio2->CreateSourceVoice(&soundData.pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	//再生する波形データの設定
	soundData.buf.pAudioData = soundData.pBuffer;
	soundData.buf.AudioBytes = soundData.bufferSize;

	sound.filename = filename;
	sound.pSourceVoice = soundData.pSourceVoice;
	sound.buf = soundData.buf;

	return soundData;
}

//サウンド再生
void Audio::playSoundWave(SoundData& soundData, bool IsLoop)
{
	HRESULT result;

	if (IsLoop == true)
	{
		soundData.buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	soundData.buf.Flags = XAUDIO2_END_OF_STREAM;

	//波形データの再生
	result = soundData.pSourceVoice->SetVolume(valume);
	result = soundData.pSourceVoice->SubmitSourceBuffer(&soundData.buf);
	result = soundData.pSourceVoice->Start(0);
}

//サウンド再生(音量指定)
void Audio::playSoundWave_Val(SoundData& soundData, bool IsLoop, float val)
{
	HRESULT result;

	if (IsLoop == true)
	{
		soundData.buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	soundData.buf.Flags = XAUDIO2_END_OF_STREAM;

	//波形データの再生
	result = soundData.pSourceVoice->SetVolume(val);
	result = soundData.pSourceVoice->SubmitSourceBuffer(&soundData.buf);
	result = soundData.pSourceVoice->Start(0);
}

//停止
void Audio::stopSoundWave(SoundData& soundData)
{
	soundData.pSourceVoice->Stop(0);
	soundData.pSourceVoice->FlushSourceBuffers();
	soundData.pSourceVoice->SubmitSourceBuffer(&soundData.buf);
}

//一時停止
void Audio::stopOnlySoundWave(SoundData& soundData)
{
	soundData.pSourceVoice->Stop(0);
}

//音声データ解放
void Audio::unLoadSoundWave(SoundData* soundData)
{
	//バッファのメモリを解放
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}
