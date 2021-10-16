#pragma once
#include "HMUI/ViewController.hpp"
#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(
    ImageFactory::ViewControllers, ImageCreationViewController,
    HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate,
                            il2cpp_utils::FindMethodUnsafe("HMUI",
                                                           "ViewController",
                                                           "DidActivate", 3),
                            bool firstActivation, bool addedToHierarchy,
                            bool screenSystemEnabling);
    public
    : std::function<void(std::string)>
        createImageFunction;
    void set_createImageFunction(
        std::function<void(std::string)> createImageFunction););