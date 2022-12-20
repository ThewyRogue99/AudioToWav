#include "Audio.h"
#include "Utils.h"

#include "AudioFile.h"

#define MINIMP3_IMPLEMENTATION
#define MINIMP3_FLOAT_OUTPUT
#define MINIMP3_ONLY_MP3
#include "minimp3_ex.h"

#include <filesystem>

#include "stb_vorbis.h"

namespace AudioToWav
{
	Audio Utils::LoadWav_Aiff(const std::wstring& FilePath)
	{
		Audio result;

		AudioFile<float> f;
		if (f.load(FilePath))
		{
			result.Samples = f.samples;
			result.SampleRate = f.getSampleRate();

			return result;
		}

		return Audio();
	}

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

	Audio Utils::LoadMp3(const std::wstring& FilePath)
	{
		Audio result;

		mp3dec_t mp3d;
		mp3dec_file_info_t info;
		if (!mp3dec_load_w(&mp3d, FilePath.c_str(), &info, NULL, NULL))
		{
			int numSamples = (int)info.samples / info.channels;

			for (size_t channel = 0; channel < info.channels; channel++)
			{
				std::vector<mp3d_sample_t> ChannelSamples(numSamples);
				for (int i = 0; i < numSamples; i++)
				{
					ChannelSamples[i] = (info.buffer + (channel * numSamples))[i];
				}

				result.SampleRate = info.hz;
				result.Samples.push_back(ChannelSamples);
			}

			free(info.buffer);

			return result;
		}

		return Audio();
	}

	Audio Utils::LoadOgg(const std::wstring& FilePath)
	{
		std::ifstream fh(FilePath, std::ios::in  | std::ios::binary | std::ios::ate);
		const size_t sz = fh.tellg();
		if (sz <= 0) {
			// error handling here
		}
		fh.seekg(0, std::ios::beg);
		
		uint8_t* Buff = nullptr;
		if (fh.is_open())
		{
			Buff = new uint8_t[sz];
			fh.read((char*)Buff, sz);

			fh.close();

			int channels, sample_rate;

			int error;
			stb_vorbis* f = stb_vorbis_open_memory(Buff, (int)sz, &error, nullptr);

			float** output;
			int numSamples = stb_vorbis_get_frame_float(f, &channels, &output);
			stb_vorbis_info inf = stb_vorbis_get_info(f);
			sample_rate = inf.sample_rate;

			delete[] Buff;

			if (numSamples)
			{
				Audio result;

				for (int channel = 0; channel < channels; channel++)
				{
					std::vector<float> ChannelSamples(numSamples);
					for (int i = 0; i < numSamples; i++)
					{
						ChannelSamples[i] = output[channel][i];
					}

					result.SampleRate = sample_rate;
					result.Samples.push_back(ChannelSamples);
				}

				stb_vorbis_close(f);

				return result;
			}
		}

		return Audio();
	}
}