#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    addAndMakeVisible(multiplePanelComponent);
    addAndMakeVisible(btn1);
    addAndMakeVisible(slider1);
    addAndMakeVisible(paddedComponent);

    for(int i = 0; i< 40; i++){
        auto c = new CustomComponent(juce::String(i));
        components.push_back(c);

        addAndMakeVisible(c);
    }

    setSize (600, 400);
    inspector.setVisible(true);
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
    paddedComponent.setBoundsAndPadding({300, 100, 150, 220}, 24, 32, 0 , 100);

    int i = 1;
    int offset = 2;
    for(auto c: components){
        c->setBounds({50, offset + i * 25, 20, 20});
        i++;
    }


    auto& props = btn1.getProperties();
    props.set ("paddingLeft", 12);
}
