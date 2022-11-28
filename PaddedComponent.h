#pragma once
#include "juce_gui_basics/juce_gui_basics.h"

/*
 * A couple basic helper functions to help components
 * deal with concepts like drop shadows
 *
 * The main point is to allow "bleed" around the normal content bounds
 *
 *           getLocalBounds()
 *  -----------------------------------
 *  |             paddingY            |
 *  |   ---------------------------   |
 *  |   |                         |   |
 *  |   |    getContentBounds()   |  <--- paddingX
 *  |   |                         |   |
 *  |   ---------------------------   |
 *  |                                 |
 *  |----------------------------------
 *
 */
template <typename T>
concept ConceptObject = std::is_base_of<juce::Component, T>::value;

class PaddedComponent : public juce::Component
{
public:
    juce::PopupMenu rightClickMenu;

    void mouseDown (const juce::MouseEvent& e) override
    {
        if (e.mods.isPopupMenu())
        {
            rightClickMenu.showMenuAsync (juce::PopupMenu::Options().withTargetComponent (this).withMousePosition());
        }
    }

    void paint(juce::Graphics &g) override {
        g.setColour(juce::Colours::cadetblue.withAlpha(0.2f));
        g.fillRect(getLocalBounds());

        g.setColour(juce::Colours::beige);
        g.fillRect(getContentBounds());;
    }

    explicit PaddedComponent (juce::String userFacingName = juce::String()) : juce::Component(userFacingName)
    {
        if (userFacingName.isNotEmpty())
            rightClickMenu.addItem (juce::String ("Read Manual for ") + userFacingName, [&]() { juce::URL (juce::String ("https://melatonin.dev/manual/sinemachine/") + userFacingName).launchInDefaultBrowser(); });
    }

    template <ConceptObject... Components>
    void addChildren (Components&... children)
    {
        for (const auto child : { children... })
        {
            addAndMakeVisible (child);
        }
    }

    // Achtung: JUCE is NOT like CSS padding, goes left, top, right, bottom
    void setBoundsAndPadding (juce::Rectangle<int> bounds, int l, int t, int r, int b)
    {
        paddingLeft = l;
        paddingTop = t;
        paddingRight = r;
        paddingBottom = b;
        setBounds (bounds.getX() - l, bounds.getY() - t, bounds.getWidth() + l + r, bounds.getHeight() + t + b);
        storePaddingsInProperties();
    }

    void setBoundsAndPadding (juce::Rectangle<int> newBounds, int newPaddingX, int newPaddingY)
    {
        paddingLeft = paddingRight = newPaddingX;
        paddingTop = paddingBottom = newPaddingY;
        setBounds (newBounds.expanded (newPaddingX, newPaddingY));
        storePaddingsInProperties();
    }

    void setBoundsAndPadding (juce::Rectangle<int> newBounds, int newPadding)
    {
        setBoundsAndPadding (newBounds, newPadding, newPadding);
    }

    /*
     * This is when you are passing bounds that include the padding
     * getLocalBounds is the bounds passed in
     * getContentBounds is reduced by the specified padding
     */
    void setBoundsReducedByPadding (juce::Rectangle<int> newBounds, int newPaddingX, int newPaddingY)
    {
        paddingLeft = paddingRight = newPaddingX;
        paddingTop = paddingBottom = newPaddingY;
        setBounds (newBounds);
        storePaddingsInProperties();
    }

    void setBoundsReducedByPadding (juce::Rectangle<int> newBounds, int newPadding)
    {
        setBoundsReducedByPadding (newBounds, newPadding, newPadding);
    }

    // Achtung: This is NOT like CSS padding...
    void setBoundsReducedByPadding (juce::Rectangle<int> newBounds, int l, int t, int r, int b)
    {
        paddingLeft = l;
        paddingTop = t;
        paddingRight = r;
        paddingBottom = b;
        setBounds (newBounds);
        storePaddingsInProperties();
    }

    int getContentWidth()
    {
        return getWidth() - paddingLeft - paddingRight;
    }

    juce::Rectangle<int> getContentBounds()
    {
      restorePaddingsFromProperties();

        auto bounds = getLocalBounds();
        auto newWidth = bounds.getWidth() - paddingLeft - paddingRight;
        auto newHeight = bounds.getHeight() - paddingTop - paddingBottom;
        return { bounds.getX() + paddingLeft, bounds.getY() + paddingTop, newWidth, newHeight };
    }

    //    // this assumes you don't want the component's contentBounds to be clickable
    //    // It'll just expose a click area in the padding to help close windows etc
    //    bool hitTest (int x, int y) override
    //    {
    //    }

    // gives the parent's coordinate for the content bounds (not the component bounds)
    juce::Rectangle<int> getContentBoundsInParent()
    {
        auto p = getBoundsInParent();
        return { p.getX() + paddingLeft, p.getY() + paddingTop, p.getWidth() - paddingLeft - paddingRight, p.getHeight() - paddingTop - paddingBottom };
    }

    int paddingLeft = 0;
    int paddingTop = 0;
    int paddingRight = 0;
    int paddingBottom = 0;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PaddedComponent)

    void storePaddingsInProperties()
    {
        auto& props = getProperties();
        props.set ("paddingLeft", paddingLeft);
        props.set ("paddingTop", paddingTop);
        props.set ("paddingRight", paddingRight);
        props.set ("paddingBottom", paddingBottom);
    }

    void restorePaddingsFromProperties()
    {
        auto& props = getProperties();
        paddingLeft = props.getWithDefault("paddingLeft", paddingLeft);
        paddingTop = props.getWithDefault("paddingTop", paddingTop);
        paddingRight = props.getWithDefault("paddingRight", paddingRight);
        paddingBottom = props.getWithDefault("paddingBottom", paddingBottom);
    }
};
