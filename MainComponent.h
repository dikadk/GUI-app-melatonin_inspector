#pragma once

// CMake builds don't use an AppConfig.h, so it's safe to include juce module headers
// directly. If you need to remain compatible with Projucer-generated builds, and
// have called `juce_generate_juce_header(<thisTarget>)` in your CMakeLists.txt,
// you could `#include <JuceHeader.h>` here instead, to make all your module headers visible.
#include <juce_gui_extra/juce_gui_extra.h>
#include "melatonin_inspector/melatonin_inspector.h"
#include "PaddedComponent.h"

//==============================================================================
struct SampleLnF: juce::LookAndFeel_V4{
    SampleLnF(){
        setColour(juce::ResizableWindow::backgroundColourId, juce::Colours::crimson);
    }
};

struct PanelComp: juce::Component{

    PanelComp(const juce::String& name): juce::Component(name){
        setLookAndFeel(&lnf);
    }

    ~PanelComp(){
        setLookAndFeel(nullptr);
    }

    void paint(juce::Graphics &g) override {
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    }

    SampleLnF lnf;
};

struct CustomComponent: juce::Component{
    CustomComponent(const juce::String& name): juce::Component(name){
    }

    void paint(juce::Graphics &g) override {
        g.fillAll(juce::Colours::goldenrod);
    }
};

struct MultiplePanelComponent : juce::Component {
    MultiplePanelComponent() {
        auto compNames = {"Left","Center","Right"};
        for(auto s: compNames){
            auto c = new PanelComp(s);

            comps.add(c);
            addAndMakeVisible(c);
        }
    }

    void resized() override {
        auto r = getLocalBounds();

        //comps[0]->setBounds();
        r.removeFromLeft(getWidth() / 3);
        r.removeFromLeft(20);
        comps[1]->setBounds(r.removeFromLeft(getWidth() / 3));
        r.removeFromLeft(50);
        comps[2]->setBounds(r);
    }

    juce::OwnedArray<juce::Component> comps;
};

/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::Component {
public:
    //==============================================================================
    MainComponent();

    //==============================================================================
    void paint(juce::Graphics &) override;

    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    melatonin::Inspector inspector{*this, false};

    MultiplePanelComponent multiplePanelComponent;
    juce::TextButton btn1{"Button1"};
    juce::Slider slider1{"SliderOne"};

    juce::OwnedArray<CustomComponent> components;

    PaddedComponent paddedComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
