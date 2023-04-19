#pragma once
#include<Windows.h>
#include<xaudio2.h>
#include<wrl.h>

//�I�[�f�B�I�R�[���o�b�N
class XAudio2VoiceCallback : public IXAudio2VoiceCallback
{
public:
	// �{�C�X�����p�X�̊J�n��
	//STDMETHOD_(void, onVoiceProcessingPassStart) (THIS_ UINT32 BytesRequired) {};
	void OnVoiceProcessingPassStart(UINT32 BytesRequired) {};
	// �{�C�X�����p�X�̏I����
	STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS) {};
	// �o�b�t�@�X�g���[���̍Đ����I��������
	STDMETHOD_(void, OnStreamEnd) (THIS) {};
	// �o�b�t�@�̎g�p�J�n��
	STDMETHOD_(void, OnBufferStart) (THIS_ void* pBufferContext) {};
	// �o�b�t�@�̖����ɒB������
	STDMETHOD_(void, OnBufferEnd) (THIS_ void* pBufferContext) {
		// �o�b�t�@���������
		delete[] pBufferContext;
	};
	// �Đ������[�v�ʒu�ɒB������
	STDMETHOD_(void, OnLoopEnd) (THIS_ void* pBufferContext) {};
	// �{�C�X�̎��s�G���[��
	STDMETHOD_(void, OnVoiceError) (THIS_ void* pBufferContext, HRESULT Error) {};
};

//�����f�[�^
struct SoundData
{
	//�g�`�t�H�[�}�b�g
	WAVEFORMATEX wfex;

	//�o�b�t�@�̐擪�A�h���X
	BYTE* pBuffer;

	//�o�b�t�@�̃T�C�Y
	unsigned int bufferSize;

	//��~���邩�ǂ���
	bool IsStop;

	//�t�@�C���p�X
	const char* filename;

	//�\�[�X�{�C�X
	IXAudio2SourceVoice* pSourceVoice = nullptr;

	//�o�b�t�@�[
	XAUDIO2_BUFFER buf{};
};

//�I�[�f�B�I�N���X
class Audio
{
private:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//���\����
	//�`�����N�w�b�_
	struct ChunkHeader
	{
		char id[4];
		int  size;
	};

	//RIFF�w�b�_�`�����N
	struct RiffHeader
	{
		ChunkHeader chunk;
		char type[4];
	};

	//FMT�`�����N
	struct FormatChunk
	{
		ChunkHeader chunk;
		WAVEFORMATEX fmt;
	};

public:
	//�f�X�g���N�^
	~Audio();

	//������
	void Initialize();

	//�T�E���h�ǂݍ���
	SoundData loadSoundWave(const char* filename, SoundData& sound);

	//�T�E���h�Đ�
	void playSoundWave(SoundData& soundData, bool IsLoop);

	//�T�E���h�Đ�(���ʎw��)
	void playSoundWave_Val(SoundData& soundData, bool IsLoop, float val);

	//��~
	void stopSoundWave(SoundData& soundData);

	//�ꎞ��~
	void stopOnlySoundWave(SoundData& soundData);

	//�����f�[�^���
	void unLoadSoundWave(SoundData* soundData);

private:
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice = nullptr;
	XAudio2VoiceCallback voiceCallback;

	float valume = 0.5f;
};