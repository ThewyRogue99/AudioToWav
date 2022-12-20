#pragma once

#include <vector>

namespace AudioToWav
{
	class Audio
	{
	public:
		int GetNumChannels() const { return (int)Samples.size(); }
		int GetNumSamples() const { return GetNumSamplesPerChannel() * GetNumChannels(); }
		int GetNumSamplesPerChannel() const { return (int)Samples[0].size(); }
		uint32_t GetSampleRate() const { return SampleRate; }

		bool Empty() const { return Samples.size() < 1; }

		friend class Utils;

	protected:
		Audio() = default;

		uint32_t SampleRate = 0;

		std::vector<std::vector<float>> Samples = { };
	};
}