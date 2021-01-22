#include <JuceHeader.h>

#include "StreamBotPlugin.hpp"

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
  return new StreamBot();
}
