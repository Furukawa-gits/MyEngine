#include"Audio.h"
#include<fstream>
#include<cassert>

#pragma comment(lib,"xaudio2.lib")

//�f�X�g���N�^
Audio::~Audio()
{
	xAudio2.Reset();
}

//������
void Audio::Initialize()
{
	HRESULT result;

	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	result = xAudio2->CreateMasteringVoice(&masterVoice);
}

//�T�E���h�ǂݍ���
SoundData Audio::LoadSoundWave(const char* filename, SoundData& sound)
{
	HRESULT result;

	//�t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;
	//.wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(filename, std::ios_base::binary);
	//�t�@�C���I�[�v�����s�����o
	assert(file.is_open());


	RiffHeader riff;
	FormatChunk format = {};
	ChunkHeader data;

	//RIFF�w�b�_�[�̓ǂݍ���
	file.read((char*)&riff, sizeof(riff));

	//�t�@�C����RIFF���`�F�b�N
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}

	//�^�C�v��WAVE���`�F�b�N
	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}

	//Format�`�����N�̓ǂݍ���
	file.read((char*)&format, sizeof(ChunkHeader));

	//JUNK�`�����N�����o�����ꍇ
	if (strncmp(format.chunk.id, "JUNK ", 4) == 0)
	{
		file.seekg(format.chunk.size, std::ios_base::cur);
		file.read((char*)&format, sizeof(ChunkHeader));
	}

	//�`�����N�w�b�_�[�̊m�F
	if (strncmp(format.chunk.id, "fmt ", 4) != 0)
	{
		assert(0);
	}

	//�`�����N�{�̂̓ǂݍ���
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	//JUNK�`�����N�����o�����ꍇ
	if (strncmp(format.chunk.id, "JUNK ", 4) == 0)
	{
		file.seekg(format.chunk.size, std::ios_base::cur);
		file.read((char*)&format.fmt, format.chunk.size);
	}

	//Data�`�����N�̓ǂݍ���
	file.read((char*)&data, sizeof(data));

	//JUNK�`�����N�����o�����ꍇ
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

	//Data�`�����N�̃f�[�^��(�g�`�f�[�^)�̓ǂݍ���
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//JUNK�`�����N�����o�����ꍇ
	if (strncmp(data.id, "JUNK ", 4) == 0)
	{
		file.seekg(data.size, std::ios_base::cur);
		file.read((char*)&data, sizeof(data));
	}

	//�t�@�C�������
	file.close();

	//return���邽�߂̉����f�[�^
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	//�g�`�t�H�[�}�b�g�����ƂɃT�E���h�{�C�X�̍Đ�
	result = xAudio2->CreateSourceVoice(&soundData.pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	//�Đ�����g�`�f�[�^�̐ݒ�
	soundData.buf.pAudioData = soundData.pBuffer;
	soundData.buf.AudioBytes = soundData.bufferSize;

	sound.filename = filename;
	sound.pSourceVoice = soundData.pSourceVoice;
	sound.buf = soundData.buf;

	return soundData;
}

//�T�E���h�Đ�
void Audio::PlaySoundWave(SoundData& soundData, bool IsLoop)
{
	HRESULT result;

	if (IsLoop == true)
	{
		soundData.buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	soundData.buf.Flags = XAUDIO2_END_OF_STREAM;

	//�g�`�f�[�^�̍Đ�
	result = soundData.pSourceVoice->SetVolume(valume);
	result = soundData.pSourceVoice->SubmitSourceBuffer(&soundData.buf);
	result = soundData.pSourceVoice->Start(0);
}

//�T�E���h�Đ�(���ʎw��)
void Audio::PlaySoundWave_Val(SoundData& soundData, bool IsLoop, float val)
{
	HRESULT result;

	if (IsLoop == true)
	{
		soundData.buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	soundData.buf.Flags = XAUDIO2_END_OF_STREAM;

	//�g�`�f�[�^�̍Đ�
	result = soundData.pSourceVoice->SetVolume(val);
	result = soundData.pSourceVoice->SubmitSourceBuffer(&soundData.buf);
	result = soundData.pSourceVoice->Start(0);
}

//��~
void Audio::StopSoundWave(SoundData& soundData)
{
	soundData.pSourceVoice->Stop(0);
	soundData.pSourceVoice->FlushSourceBuffers();
	soundData.pSourceVoice->SubmitSourceBuffer(&soundData.buf);
}

//�ꎞ��~
void Audio::StopOnlySoundWave(SoundData& soundData)
{
	soundData.pSourceVoice->Stop(0);
}

//�����f�[�^���
void Audio::UnLoadSoundWave(SoundData* soundData)
{
	//�o�b�t�@�̃����������
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}
