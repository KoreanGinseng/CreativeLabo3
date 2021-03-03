#pragma once

#include	"../Common.h"

#ifndef _WAVEFORMATEX_
#define _WAVEFORMATEX_
typedef struct tWAVEFORMATEX
{
	WORD    wFormatTag;
	WORD    nChannels;
	DWORD   nSamplesPerSec;
	DWORD   nAvgBytesPerSec;
	WORD    nBlockAlign;
	WORD    wBitsPerSample;
	WORD    cbSize;
} WAVEFORMATEX;
#endif /* _WAVEFORMATEX_ */

#ifndef _WAVEFORMATEXTENSIBLE_
#define _WAVEFORMATEXTENSIBLE_
typedef struct {
	WAVEFORMATEX    Format;
	union {
		WORD wValidBitsPerSample;       /* bits of precision  */
		WORD wSamplesPerBlock;          /* valid if wBitsPerSample==0 */
		WORD wReserved;                 /* If neither applies, set to zero. */
	} Samples;
	DWORD           dwChannelMask;      /* which channels are */
										/* present in stream  */
	GUID            SubFormat;
} WAVEFORMATEXTENSIBLE, * PWAVEFORMATEXTENSIBLE;
#endif // !_WAVEFORMATEXTENSIBLE_

namespace Sample {

	enum WAVEFILETYPE
	{
		WF_EX = 1,
		WF_EXT = 2
	};

	enum WAVERESULT
	{
		WR_OK = 0,
		WR_INVALIDFILENAME = -1,
		WR_BADWAVEFILE = -2,
		WR_INVALIDPARAM = -3,
		WR_INVALIDFILE = -4,
		WR_NOTSUPPORTEDYET = -5,
		WR_WAVEMUSTBEMONO = -6,
		WR_WAVEMUSTBEWAVEFORMATPCM = -7,
		WR_WAVESMUSTHAVESAMEBITRESOLUTION = -8,
		WR_WAVESMUSTHAVESAMEFREQUENCY = -9,
		WR_WAVESMUSTHAVESAMEBITRATE = -10,
		WR_WAVESMUSTHAVESAMEBLOCKALIGNMENT = -11,
		WR_OFFSETOUTOFDATARANGE = -12,
		WR_FILEERROR = -13,
		WR_OUTOFMEMORY = -14,
		WR_INVALIDSPEAKERPOS = -15,
		WR_INVALIDWAVEFILETYPE = -16,
		WR_NOTWAVEFORMATEXTENSIBLEFORMAT = -17
	};

	typedef struct
	{
		WAVEFILETYPE	wfType;
		WAVEFORMATEXTENSIBLE wfEXT;		// For non-WAVEFORMATEXTENSIBLE wavefiles, the header is stored in the Format member of wfEXT
		char* pData;
		unsigned long	ulDataSize;
		FILE* pFile;
		unsigned long	ulDataOffset;
	} WAVEFILEINFO, * LPWAVEFILEINFO;

	class WaveFile
	{
	private:
		/** Waveファイル情報 */
		WAVEFILEINFO			Info_;

	public:
		/**
		 * @brief		コンストラクタ
		 */
		WaveFile();
		/**
		 * @brief		デストラクタ
		 */
		virtual ~WaveFile();

		/**
		 * @brief		解放処理
		 */
		void Release();

		/**
		 * @brief		ファイルを開く
		 * @param[in]	name	読み込むWAVE名
		 * @return		WAVERESULT参照
		 */
		WAVERESULT Open(const char* name);

		/**
		 * @brief		読み込み
		 * @param[in]	name	読み込むWAVE名
		 * @return		WAVERESULT参照
		 */
		WAVERESULT Load(const char* name);

		/**
		 * @brief		読み込み
		 * @param[in/out]	size	読み込むバッファ
		 * @param[in]		size	読み込むサイズ
		 * @param[in/out]	outSize	読み込んだサイズ
		 * @return		WAVERESULT参照
		 */
		WAVERESULT Read(void* pData, unsigned long size, unsigned long* outSize);

		/**
		 * @brief		読み込み位置変更
		 * @param[in]	Offset	位置
		 * @return		WAVERESULT参照
		 */
		WAVERESULT Seek(unsigned long Offset);

		const WAVEFILETYPE Type() const { return Info_.wfType; }
		const WAVEFORMATEX& ExHeader() const { return Info_.wfEXT.Format; }
		const WAVEFORMATEXTENSIBLE& ExtensibleHeader() const { return Info_.wfEXT; }
		const char* Data() const { return Info_.pData; }
		const unsigned long Size() const { return Info_.ulDataSize; }
		const unsigned long Offset() const {
			if (!Info_.pFile)
			{
				return 0;
			}
			return ftell(Info_.pFile) - Info_.ulDataOffset;
		}
		const unsigned long Frequency() const { return Info_.wfEXT.Format.nSamplesPerSec; }
	};
	using WaveFilePtr = std::shared_ptr< WaveFile >;

}