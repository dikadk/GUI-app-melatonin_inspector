#pragma once

// CMake builds don't use an AppConfig.h, so it's safe to include juce module headers
// directly. If you need to remain compatible with Projucer-generated builds, and
// have called `juce_generate_juce_header(<thisTarget>)` in your CMakeLists.txt,
// you could `#include <JuceHeader.h>` here instead, to make all your module headers visible.
#include "PaddedComponent.h"
#include "melatonin_inspector/melatonin_inspector.h"
#include <juce_gui_extra/juce_gui_extra.h>

//==============================================================================
struct SampleLnF : juce::LookAndFeel_V4
{
    SampleLnF()
    {
        setColour (juce::ResizableWindow::backgroundColourId, juce::Colours::crimson);
    }
};

namespace juce
{
    template <>
    struct VariantConverter<Colour>
    {
        static Colour fromVar (const var& v) { return Colour ((uint32) static_cast<int> (v)); }

        static var toVar (Colour v) { return { static_cast<int> (v.getARGB()) }; }
    };
}

struct PanelComp : juce::Component
{
    PanelComp (const juce::String& name) : juce::Component (name)
    {
        setLookAndFeel (&lnf);
    }

    ~PanelComp()
    {
        setLookAndFeel (nullptr);
    }

    void paint (juce::Graphics& g) override
    {
        auto defColor = getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId);
        auto bgColor = getProperties().getWithDefault ("Color", juce::VariantConverter<juce::Colour>::toVar (defColor));
        g.fillAll (juce::VariantConverter<juce::Colour>::fromVar (bgColor));
    }

    SampleLnF lnf;
};

struct CustomComponent : juce::Component
{
    CustomComponent (const juce::String& name) : juce::Component (name)
    {
    }

    void paint(juce::Graphics &g) override {
        g.fillAll(juce::Colours::goldenrod);
    }
};

struct MultiplePanelComponent : juce::Component
{
    MultiplePanelComponent()
    {
        auto compNames = { "Left", "Center", "Right" };
        for (auto s : compNames)
        {
            auto c = new PanelComp (s);

            comps.add (c);
            addAndMakeVisible (c);
        }

        comps[0]->getProperties().set ("Color", juce::VariantConverter<juce::Colour>::toVar (juce::Colours::darkolivegreen));
        comps[0]->setOpaque(true);

        comps[1]->setBufferedToImage(true);
    }

    void resized() override
    {
        auto r = getLocalBounds();

        r.removeFromLeft (getWidth() / 4);
        comps[0]->setBounds (r.removeFromLeft (getWidth() / 8));
        r.removeFromLeft (20);
        comps[1]->setBounds (r.removeFromLeft (getWidth() / 3));
        r.removeFromLeft (50);
        comps[2]->setBounds (r);
    }

    juce::OwnedArray<juce::Component> comps;
};

/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::Component
{
public:
    //==============================================================================
    MainComponent();

    //==============================================================================
    void paint (juce::Graphics&) override;

    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    melatonin::Inspector inspector { *this, false };

    MultiplePanelComponent multiplePanelComponent;
    juce::TextButton btn1 { "Button1" };
    juce::Slider slider1 { "SliderOne" };

    juce::OwnedArray<CustomComponent> components;

    PaddedComponent paddedComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
