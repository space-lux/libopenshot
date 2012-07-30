#include "../include/AudioBufferSource.h"

using namespace std;
using namespace openshot;

// Default constructor
AudioBufferSource::AudioBufferSource(AudioSampleBuffer *audio_buffer)
		: position(0), start(0), repeat(false)
{
	// Create new buffer
	buffer = audio_buffer;
}

// Destructor
AudioBufferSource::~AudioBufferSource()
{
	// Clear and delete the buffer
	buffer = NULL;
};

// Get the next block of audio samples
void AudioBufferSource::getNextAudioBlock (const AudioSourceChannelInfo& info)
{
	int buffer_samples = buffer->getNumSamples();
	int buffer_channels = buffer->getNumChannels();

	if (info.numSamples > 0) {
		int start = position;
		int number_to_copy = 0;

		// Determine how many samples to copy
		if (start + info.numSamples <= buffer_samples)
		{
			// copy the full amount requested
			number_to_copy = info.numSamples;
		}
		else if (start > buffer_samples)
		{
			// copy nothing
			number_to_copy = 0;
		}
		else if (buffer_samples - start > 0)
		{
			// only copy what is left in the buffer
			number_to_copy = buffer_samples - start;
		}
		else
		{
			// copy nothing
			number_to_copy = 0;
		}

		// Determine if any samples need to be copied
		if (number_to_copy > 0)
		{
			// Loop through each channel and copy some samples
			for (int channel = 0; channel < buffer_channels; channel++)
				info.buffer->copyFrom(channel, info.startSample, *buffer, channel, start, number_to_copy);

			// Update the position of this audio source
			position += number_to_copy;
		}

	}

}

// Prepare to play this audio source
void AudioBufferSource::prepareToPlay(int, double) { };

// Release all resources
void AudioBufferSource::releaseResources() { };

// Set the next read position of this source
void AudioBufferSource::setNextReadPosition (long long newPosition)
{
	// set position (if the new position is in range)
	if (newPosition > 0 && newPosition < buffer->getNumSamples())
		position = newPosition;
};

// Get the next read position of this source
long long AudioBufferSource::getNextReadPosition() const
{
	// return the next read position
	return position;
};

// Get the total length (in samples) of this audio source
long long AudioBufferSource::getTotalLength() const
{
	// Get the length
	return buffer->getNumSamples();
};

// Determines if this audio source should repeat when it reaches the end
bool AudioBufferSource::isLooping() const
{
	// return if this source is looping
	return repeat;
};

// Set if this audio source should repeat when it reaches the end
void AudioBufferSource::setLooping (bool shouldLoop)
{
	// Set the repeat flag
	repeat = shouldLoop;
};

