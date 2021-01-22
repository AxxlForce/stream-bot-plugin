#pragma once

class StreamBot : public AudioProcessor
{
  using AudioProcessor::processBlock;

public:
  StreamBot()
    : AudioProcessor(BusesProperties().withInput("Input", AudioChannelSet::stereo()).withOutput("Output", AudioChannelSet::stereo()))
  {
    addParameter(threshold = new AudioParameterFloat("threshold", "Threshold", 0.0f, 1.0f, 0.5f));
    addParameter(service = new AudioParameterChoice("service", "Service", StringArray("Discord", "Skype", "Teams"), 0, ""));
  }

  void prepareToPlay(double, int) override {}

  void releaseResources() override {}

  void processBlock(AudioBuffer<float>& buffer, MidiBuffer&) override
  {
    auto mainInputOutput = getBusBuffer(buffer, true, 0);

    for (int j = 0; j < buffer.getNumSamples(); ++j)
    {
      for (int i = 0; i < mainInputOutput.getNumChannels(); ++i)
        *mainInputOutput.getWritePointer(i, j) = *mainInputOutput.getReadPointer(i, j);
    }
  }

  AudioProcessorEditor* createEditor() override { return new GenericAudioProcessorEditor(*this); }

  bool hasEditor() const override { return true; }

  const String getName() const override { return "StreamBot"; }

  bool acceptsMidi() const override { return false; }

  bool producesMidi() const override { return false; }

  double getTailLengthSeconds() const override { return 0.0; }

  int getNumPrograms() override { return 1; }

  int getCurrentProgram() override { return 0; }

  void setCurrentProgram(int) override {}

  const String getProgramName(int) override { return {}; }

  void changeProgramName(int, const String&) override {}

  void getStateInformation(MemoryBlock& destData) override
  {
    MemoryOutputStream stream(destData, true);

    stream.writeFloat(*threshold);
  }

  void setStateInformation(const void* data, int sizeInBytes) override
  {
    MemoryInputStream stream(data, static_cast<size_t>(sizeInBytes), false);

    threshold->setValueNotifyingHost(stream.readFloat());
  }

private:
  AudioParameterFloat* threshold;
  AudioParameterChoice* service;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StreamBot)
};
