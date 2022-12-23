#include "Audio.h"
#include "Utils.h"

#include <AudioFile.h>

#define MINIMP3_IMPLEMENTATION
#define MINIMP3_FLOAT_OUTPUT
#define MINIMP3_ONLY_MP3
#include <minimp3_ex.h>

#include <filesystem>

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <vorbis/vorbisenc.h>

namespace AudioToWav
{
	Audio Utils::LoadAudioFile(const std::wstring& FilePath)
	{
		std::filesystem::path fPath = FilePath;

		if (fPath.has_extension() && std::filesystem::exists(FilePath))
		{
			std::filesystem::path ext = fPath.extension();

			if (ext == L".wav" || ext == L".aiff")
				return LoadWav_Aiff(FilePath);
			else if (ext == L".mp3")
				return LoadMp3(FilePath);
			else if (ext == L".ogg")
				return LoadOgg(FilePath);
		}

		return Audio();
	}

	Audio Utils::LoadWav_Aiff(const std::wstring& FilePath)
	{
		Audio result;

		AudioFile<float> f;
		if (f.load(FilePath))
		{
			result.SampleSize = sizeof(float);
			result.Channels = f.getNumChannels();
			result.SamplesPerChannel = f.getNumSamplesPerChannel();
			result.SampleRate = f.getSampleRate();

			result.Samples = new float[result.GetNumSamples()];

			for (int i = 0; i < result.Channels; i++)
			{
				memcpy(
					(float*)result.Samples + (i * result.SamplesPerChannel),
					f.samples[i].data(),
					result.SamplesPerChannel * sizeof(float)
				);
			}

			return result;
		}

		return Audio();
	}

	Audio Utils::LoadMp3(const std::wstring& FilePath)
	{
		Audio result;

		mp3dec_t mp3d;
		mp3dec_file_info_t info;
		if (!mp3dec_load_w(&mp3d, FilePath.c_str(), &info, NULL, NULL))
		{
			result.Samples = new float[info.samples];

			memcpy(result.Samples, info.buffer, info.samples * sizeof(float));

			result.SampleSize = sizeof(float);
			result.Channels = info.channels;
			result.SamplesPerChannel = info.samples / info.channels;
			result.SampleRate = info.hz;

			free(info.buffer);

			return result;
		}

		return Audio();
	}

	static bool ReadOggToBuffer(OggVorbis_File* vf, short** Buffer)
	{
		if (Buffer && *Buffer)
		{
			for (size_t size = 0, offset = 0, sel = 0;
				(size = ov_read(vf, (char*)(*Buffer) + offset, 4096, 0, 2, 1, (int*)&sel)) != 0;
				offset += size) {

				if (size < 0)
					return false;
			}

			return true;
		}

		return false;
	}

	Audio Utils::LoadOgg(const std::wstring& FilePath)
	{
		FILE* f = _wfopen(FilePath.c_str(), L"rb");

		OggVorbis_File vf;
		int res = ov_open_callbacks(f, &vf, NULL, 0, OV_CALLBACKS_NOCLOSE);
		if (res >= 0)
		{
			Audio result;
			
			vorbis_info* vi = ov_info(&vf, -1);

			result.SampleSize = sizeof(short);
			result.SampleRate = vi->rate;
			result.Channels = vi->channels;
			result.SamplesPerChannel = ov_pcm_total(&vf, -1);

			short* Buffer = new short[result.SamplesPerChannel * result.Channels];
			if (ReadOggToBuffer(&vf, &Buffer))
			{
				result.Samples = Buffer;

				ov_clear(&vf);
				fclose(f);

				return result;
			}

			// Release file
			delete[] Buffer;
			ov_clear(&vf);
			fclose(f);
		}

		return Audio();
	}
}