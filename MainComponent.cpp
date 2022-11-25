#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 400);
    inspector.setVisible(true);

    addAndMakeVisible(multiplePanelComponent);
    addAndMakeVisible(btn1);
    addAndMakeVisible(slider1);
    addAndMakeVisible(paddedComponent);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::Font (16.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    multiplePanelComponent.setBounds(getLocalBounds());
    btn1.setBounds({60, 80, 100, 60});
    slider1.setBounds({50, 150, 160, 160});
    paddedComponent.setBoundsAndPadding({300, 100, 150, 220}, 24, 32);
}
