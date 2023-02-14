#include "Audio.h"
#include "Utils.h"

#include <AudioFile.h>

#define MINIMP3_IMPLEMENTATION
#define MINIMP3_FLOAT_OUTPUT
#define MINIMP3_ONLY_MP3
#include <minimp3_ex.h>

#include <filesystem>

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

			result.Data = new float[result.GetNumSamples()];

			for (int i = 0; i < result.Channels; i++)
			{
				memcpy(
					(float*)result.Data + (i * result.SamplesPerChannel),
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
			result.Data = new float[info.samples];

			memcpy(result.Data, info.buffer, info.samples * sizeof(float));

			result.SampleSize = sizeof(float);
			result.Channels = info.channels;
			result.SamplesPerChannel = info.samples / info.channels;
			result.SampleRate = info.hz;
			result.DataSize = info.samples * sizeof(float);

			free(info.buffer);

			return result;
		}

		return Audio();
	}
}