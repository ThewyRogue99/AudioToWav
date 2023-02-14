#pragma once

#include <cstdint>

namespace AudioToWav
{
	class Audio
	{
	public:
		int GetNumChannels() const { return Channels; }
		int GetNumSamples() const { return SamplesPerChannel * Channels; }
		int GetNumSamplesPerChannel() const { return SamplesPerChannel; }
		int GetSampleSize() const { return SampleSize; }
		uint32_t GetSampleRate() const { return SampleRate; }

		size_t GetDataSize() const { return DataSize; }
		const void* GetData() const { return Data; }

		bool Empty() const { return !Data; }

		void Clear()
		{
			delete[] Data;

			Data = nullptr;
			Channels = 0;
			SamplesPerChannel = 0;
			SampleSize = 0;
			SampleRate = 0;
		}

		friend class Utils;

	protected:
		Audio() = default;

		uint32_t SampleRate = 0;

		int Channels = 0;
		int SamplesPerChannel = 0;
		int SampleSize = 0;

		void* Data = nullptr;
		size_t DataSize = 0;
	};
}