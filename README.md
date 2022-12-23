# AudioToWav

AudioToWav is a C++ library that is made for game engines to help loading audio files for OpenAL.

## Supported audio file formats

- .aiff

- .wav

- .mp3

- .ogg

This library can only load audio files in this format.

## Usage

    AudioToWav::Audio audio = AudioToWav::Utils::LoadAudioFile(L"your_audio_file_path");
    
    int channels = audio.GetNumChannels();
    int samples = audio.GetNumSamples();
    int sample_rate = audio.GetSampleRate();
	const void* data = audio.GetData();
    
    ...
    
    ...
    
    ...
    
    (use your OpenAL code here)

## How to build

This is a static C++ library and it requires premake to create a project and be linked. You can use premake with any option of your choice and link it with your project.

## Dependencies

This library is a wrapper for the libraries given below.
- [adamstark](https://github.com/adamstark)/**[AudioFile](https://github.com/adamstark/AudioFile)**
- [lieff](https://github.com/lieff)/**[minimp3](https://github.com/lieff/minimp3)**
- [xiph](https://github.com/xiph)/**[vorbis](https://github.com/xiph/vorbis)**
- [gcp](https://github.com/gcp)/**[libogg](https://github.com/gcp/libogg)**