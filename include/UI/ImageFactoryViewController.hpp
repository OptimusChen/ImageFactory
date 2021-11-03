#pragma once
#include "HMUI/ViewController.hpp"
#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(
    ImageFactory::ViewControllers, ImageFactoryViewController,
    HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate,
                            il2cpp_utils::FindMethodUnsafe("HMUI",
                                                           "ViewController",
                                                           "DidActivate", 3),
                            bool firstActivation, bool addedToHierarchy,
                            bool screenSystemEnabling);
    public
    : std::function<void()>
        reset;
    void set_resetConfigFunction(std::function<void()> resetConfigFunction);)