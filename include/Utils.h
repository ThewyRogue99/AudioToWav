#pragma once

#include "Audio.h"
#include <string>

namespace AudioToWav
{
	class Utils
	{
	public:
		static Audio LoadAudioFile(const std::wstring& FilePath);

	private:
		static Audio LoadWav_Aiff(const std::wstring& FilePath);
		static Audio LoadMp3(const std::wstring& FilePath);
		static Audio LoadOgg(const std::wstring& FilePath);
	};
}