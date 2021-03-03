#include	"WaveFile.h"

using namespace Sample;

#pragma pack(push, 4)

typedef struct
{
	char			szRIFF[4];
	unsigned long	ulRIFFSize;
	char			szWAVE[4];
} WAVEFILEHEADER;

typedef struct
{
	char			szChunkName[4];
	unsigned long	ulChunkSize;
} RIFFCHUNK;

typedef struct
{
	unsigned short	usFormatTag;
	unsigned short	usChannels;
	unsigned long	ulSamplesPerSec;
	unsigned long	ulAvgBytesPerSec;
	unsigned short	usBlockAlign;
	unsigned short	usBitsPerSample;
	unsigned short	usSize;
	unsigned short  usReserved;
	unsigned long	ulChannelMask;
    GUID            guidSubFormat;
} WAVEFMT;

#pragma pack(pop)

/**
 * @brief		コンストラクタ
 */
WaveFile::WaveFile() {
	memset(&Info_, 0, sizeof(Info_));
}

/**
 * @brief		デストラクタ
 */
WaveFile::~WaveFile()
{
}


/**
 * @brief		解放処理
 */
void WaveFile::Release() {
	SAFE_DELETE(Info_.pData);
	if (Info_.pFile)
	{
		fclose(Info_.pFile);
		Info_.pFile = nullptr;
	}
}

/**
 * @brief		ファイルを開く
 * @param[in]	name	読み込むWAVE名
 * @return		WAVERESULT参照
 */
WAVERESULT WaveFile::Open(const char* name) {
	WAVEFILEHEADER	waveFileHeader;
	RIFFCHUNK		riffChunk;
	WAVEFMT			waveFmt;
	if (!name)
	{
		return WR_INVALIDPARAM;
	}
	memset(&Info_, 0, sizeof(Info_));
	fopen_s(&Info_.pFile, name, "rb");
	if (Info_.pFile == nullptr)
	{
		return WR_INVALIDFILENAME;
	}
	//ヘッダー読み込み
	fread(&waveFileHeader, 1, sizeof(WAVEFILEHEADER), Info_.pFile);
	if (!_strnicmp(waveFileHeader.szRIFF, "RIFF", 4) && !_strnicmp(waveFileHeader.szWAVE, "WAVE", 4))
	{
		while (fread(&riffChunk, 1, sizeof(RIFFCHUNK), Info_.pFile) == sizeof(RIFFCHUNK))
		{
			if (!_strnicmp(riffChunk.szChunkName, "fmt ", 4))
			{
				if (riffChunk.ulChunkSize <= sizeof(WAVEFMT))
				{
					fread(&waveFmt, 1, riffChunk.ulChunkSize, Info_.pFile);

					// Determine if this is a WAVEFORMATEX or WAVEFORMATEXTENSIBLE wave file
					if (waveFmt.usFormatTag == WAVE_FORMAT_PCM)
					{
						Info_.wfType = WF_EX;
						memcpy(&Info_.wfEXT.Format, &waveFmt, sizeof(PCMWAVEFORMAT));
					}
					else if (waveFmt.usFormatTag == WAVE_FORMAT_EXTENSIBLE)
					{
						Info_.wfType = WF_EXT;
						memcpy(&Info_.wfEXT, &waveFmt, sizeof(WAVEFORMATEXTENSIBLE));
					}
				}
				else
				{
					fseek(Info_.pFile, riffChunk.ulChunkSize, SEEK_CUR);
				}
			}
			else if (!_strnicmp(riffChunk.szChunkName, "data", 4))
			{
				Info_.ulDataSize = riffChunk.ulChunkSize;
				Info_.ulDataOffset = ftell(Info_.pFile);
				fseek(Info_.pFile, riffChunk.ulChunkSize, SEEK_CUR);
			}
			else
			{
				fseek(Info_.pFile, riffChunk.ulChunkSize, SEEK_CUR);
			}

			// Ensure that we are correctly aligned for next chunk
			if (riffChunk.ulChunkSize & 1)
			{
				fseek(Info_.pFile, 1, SEEK_CUR);
			}
		}

		if (Info_.ulDataSize && Info_.ulDataOffset && ((Info_.wfType == WF_EX) || (Info_.wfType == WF_EXT)))
		{
			return WR_OK;
		}
		else
		{
			fclose(Info_.pFile);
		}
	}
	return WR_BADWAVEFILE;
}

/**
 * @brief		読み込み
 * @param[in]	name	読み込むWAVE名
 * @return		WAVERESULT参照
 */
WAVERESULT WaveFile::Load(const char* name) {
	WAVERESULT wr = Open(name);
	if (wr != WR_OK)
	{
		return wr;
	}
	//メモリ確保
	Info_.pData = new char[Info_.ulDataSize];
	if (!Info_.pData)
	{
		return WR_OUTOFMEMORY;
	}
	fseek(Info_.pFile, Info_.ulDataOffset, SEEK_SET);
	if (fread(Info_.pData, 1, Info_.ulDataSize, Info_.pFile) != Info_.ulDataSize)
	{
		SAFE_DELETE(Info_.pData);
		fclose(Info_.pFile);
		Info_.pFile = nullptr;
		return WR_BADWAVEFILE;
	}
	fclose(Info_.pFile);
	Info_.pFile = nullptr;
	return WR_OK;
}

/**
 * @brief		読み込み
 * @param[in/out]	size	読み込むバッファ
 * @param[in]		size	読み込むサイズ
 * @param[in/out]	outSize	読み込んだサイズ
 * @return		WAVERESULT参照
 */
WAVERESULT WaveFile::Read(void* pData, unsigned long size, unsigned long* outSize){
	if (!pData || !outSize || size == 0)
	{
		return WR_INVALIDPARAM;
	}
	if (!Info_.pFile)
	{
		return WR_INVALIDFILE;
	}
	unsigned long ulOffset = ftell(Info_.pFile);
	if ((ulOffset - Info_.ulDataOffset + size) > Info_.ulDataSize)
	{
		size = Info_.ulDataSize - (ulOffset - Info_.ulDataOffset);
	}
	*outSize = (unsigned long)fread(pData, 1, size, Info_.pFile);
	return WR_OK;
}

/**
 * @brief		読み込み位置変更
 * @param[in]	Offset	位置
 * @return		WAVERESULT参照
 */
WAVERESULT WaveFile::Seek(unsigned long Offset) {
	if (!Info_.pFile)
	{
		return WR_INVALIDFILE;
	}
	// Seek into audio data
	fseek(Info_.pFile, Info_.ulDataOffset + Offset, SEEK_SET);
	return WR_OK;
}