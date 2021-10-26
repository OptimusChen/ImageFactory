#include "Hooks/Impl/PresentorHooks.hpp"

#include "GlobalNamespace/AudioTimeSyncController.hpp"
#include "GlobalNamespace/LevelCompletionResults.hpp"
#include "GlobalNamespace/MainMenuViewController.hpp"
#include "GlobalNamespace/NoteController.hpp"
#include "GlobalNamespace/NoteData.hpp"
#include "GlobalNamespace/NoteWasCutDelegate.hpp"
#include "GlobalNamespace/PauseAnimationController.hpp"
#include "GlobalNamespace/PauseController.hpp"
#include "GlobalNamespace/PauseMenuManager.hpp"
#include "GlobalNamespace/ResultsViewController.hpp"
#include "GlobalNamespace/ScoreController.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "GlobalNamespace/StandardLevelScenesTransitionSetupDataSO.hpp"
#include "Presenters/PresentorManager.hpp"
#include "System/Action.hpp"
#include "System/Action_1.hpp"
#include "System/Action_2.hpp"
#include "System/Action_3.hpp"
#include "System/Threading/Tasks/Task.hpp"
#include "System/Threading/Tasks/TaskStatus.hpp"
#include "System/Threading/Tasks/Task_1.hpp"
#include "UnityEngine/AsyncOperation.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/WaitForSeconds.hpp"
#include "Utils/FileUtils.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "extern/beatsaber-hook/shared/utils/il2cpp-utils-methods.hpp"
#include "include/PluginConfig.hpp"
#include "main.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

#define MakeDelegate(DelegateType, varName) \
  (il2cpp_utils::MakeDelegate<DelegateType>(classof(DelegateType), varName))

using namespace UnityEngine;
using namespace GlobalNamespace;
using namespace ImageFactory::Presentors;
using namespace ImageFactory::Hooks;

UnityEngine::GameObject* GO;
void createImagesFromConfig() {
  GO = GameObject::New_ctor(
      il2cpp_utils::createcsstr("IFIMAGEGO", il2cpp_utils::StringType::Manual));
  std::string s = getPluginConfig().Images.GetValue();
  Il2CppString* csstr = il2cpp_utils::createcsstr(s);
  ConfigDocument& config = getPluginConfig().config->config;
  il2cpp_utils::getLogger().info("[IF] test2");
  for (int i = 0; i < csstr->Split('/')->get_Length(); i++) {
    Il2CppString* csfileName = csstr->Split('/')->get(i);
    std::string fileName = to_utf8(csstrtostr(csfileName));
    if (fileName.compare("") == 1) {
      if (config.HasMember(fileName)) {
        rapidjson::Value& configValue = config[fileName];
        il2cpp_utils::getLogger().info("[IF] test1");
        il2cpp_utils::getLogger().info("[IF] test3");
        IFImage* image = GO->AddComponent<IFImage*>();
        image->ctor(BeatSaberUI::FileToSprite(
                        "/sdcard/ModData/com.beatgames.beatsaber/Mods/"
                        "ImageFactory/Images/" +
                        image->fileName),
                    il2cpp_utils::createcsstr(
                        fileName.substr(0, fileName.find_last_of("_"))));
        il2cpp_utils::getLogger().info("[IF] test4");

        if (config.HasMember(image->fileName + "_" + std::to_string(i))) {
          il2cpp_utils::getLogger().info("[IF] test5");
          rapidjson::Value& configValue =
              config[image->fileName + "_" + std::to_string(i)];
          il2cpp_utils::getLogger().info("[IF] test6");
          image->Render();
          il2cpp_utils::getLogger().info("[IF] test7");
          image->internalName = image->fileName + "_" + std::to_string(i);
          image->sprite =
              BeatSaberUI::FileToSprite(configValue["path"].GetString());
          image->path = configValue["path"].GetString();
          image->x = configValue["x"].GetFloat();
          image->y = configValue["y"].GetFloat();
          image->z = configValue["z"].GetFloat();
          image->angleX = configValue["angleX"].GetFloat();
          image->angleY = configValue["angleY"].GetFloat();
          image->angleZ = configValue["angleZ"].GetFloat();
          image->scaleX = configValue["scaleX"].GetFloat();
          image->scaleY = configValue["scaleY"].GetFloat();
          image->width = configValue["width"].GetFloat();
          image->height = configValue["height"].GetFloat();
          image->name = configValue["name"].GetString();
          image->presentationoption =
              configValue["presentationOption"].GetString();
          image->enabled = configValue["enabled"].GetBool();
          image->path = configValue["path"].GetString();

          std::string extraData = configValue["extraData"].GetString();
          if (extraData.compare("") == 1) {
            Il2CppString* csExtraData = il2cpp_utils::createcsstr(extraData);
            il2cpp_utils::getLogger().info("[IF] test7.1");
            ::Array<Il2CppString*>* pairs = csExtraData->Split('/');
            for (int j = 0; j < pairs->get_Length(); j++) {
              il2cpp_utils::getLogger().info("[IF] test7.2");
              Il2CppString* csPair = pairs->get(j);

              if (to_utf8(csstrtostr(csPair)).compare("") == 1) {
                std::string key =
                    to_utf8(csstrtostr(csPair->Split(';')->get(0)));
                std::string val =
                    to_utf8(csstrtostr(csPair->Split(';')->get(1)));

                il2cpp_utils::getLogger().info("[IF] test7.3");
                image->SetExtraData(key, val);
                il2cpp_utils::getLogger().info("[IF] test7.4");
              }
            }
          }

          il2cpp_utils::getLogger().info("[IF] test8");
          image->fileName =
              FileUtils::GetFileName(configValue["path"].GetString(), false);
          il2cpp_utils::getLogger().info("[IF] test9");
          PresentorManager::Parse(
              image, il2cpp_utils::createcsstr(image->presentationoption));
          il2cpp_utils::getLogger().info("[IF] test10");
          image->Update();
          il2cpp_utils::getLogger().info("[IF] test11");
        }
      }
    }
  }

  PresentorManager::SpawnInMenu();
  GO->SetActive(false);
}

custom_types::Helpers::Coroutine DespawnImage(IFImage* image, float delay) {
  co_yield reinterpret_cast<System::Collections::IEnumerator*>(
      CRASH_UNLESS(WaitForSeconds::New_ctor(delay)));
  image->Despawn();
  co_return;
}

void DespawnAfterSeconds(IFImage* image, float delay) {
  GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(
      reinterpret_cast<custom_types::Helpers::enumeratorT*>(
          custom_types::Helpers::CoroutineHelper::New(
              DespawnImage(image, delay))));
}

custom_types::Helpers::Coroutine WaitForMenuLoad() {
  co_yield reinterpret_cast<System::Collections::IEnumerator*>(
      CRASH_UNLESS(WaitForSeconds::New_ctor(0.5f)));
  createImagesFromConfig();
  co_return;
}

bool hasLoadedImagesFromConfig = false;

MAKE_HOOK_MATCH(MainMenuViewController_DidActivate,
                &MainMenuViewController::DidActivate, void,
                MainMenuViewController* self, bool firstActivation,
                bool addedToHierarchy, bool screenSystemEnabling) {
  if (PresentorManager::MAP->size() == 0) {
    GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(
        reinterpret_cast<custom_types::Helpers::enumeratorT*>(
            custom_types::Helpers::CoroutineHelper::New(WaitForMenuLoad())));
  }
  MainMenuViewController_DidActivate(self, firstActivation, addedToHierarchy,
                                     screenSystemEnabling);
}

MAKE_HOOK_MATCH(ResultsViewController_DidActivate,
                &GlobalNamespace::ResultsViewController::DidActivate, void,
                ResultsViewController* self, bool firstActivation,
                bool addedToHierarchy, bool screenSystemEnabling) {
  ResultsViewController_DidActivate(self, firstActivation, addedToHierarchy,
                                    screenSystemEnabling);
  PresentorManager::SpawnforAllWithExtraData(PresentorManager::RESULTS_SCREEN,
                                             "results_when", "Finished");
  if (self->levelCompletionResults->energy == 0.0f) {
    PresentorManager::SpawnforAllWithExtraData(PresentorManager::RESULTS_SCREEN,
                                               "results_when", "Failed");

  } else {
    PresentorManager::SpawnforAllWithExtraData(PresentorManager::RESULTS_SCREEN,
                                               "results_when", "Passed");
  }
}

MAKE_HOOK_MATCH(ResultsViewController_DidDeactivate,
                &GlobalNamespace::ResultsViewController::DidDeactivate, void,
                ResultsViewController* self, bool removedFromHierarchy,
                bool screenSystemEnabling) {
  ResultsViewController_DidDeactivate(self, removedFromHierarchy,
                                      screenSystemEnabling);
  PresentorManager::SpawnInMenu();
}

MAKE_HOOK_MATCH(SongEnd, &StandardLevelScenesTransitionSetupDataSO::Finish,
                void, StandardLevelScenesTransitionSetupDataSO* self,
                LevelCompletionResults* levelCompletionResults) {
  SongEnd(self, levelCompletionResults);
  il2cpp_utils::getLogger().info("[ImageFactory] test1");
  PresentorManager::SpawnInMenu();
  for (std::pair<IFImage*, std::string> pair : *PresentorManager::MAP) {
    il2cpp_utils::getLogger().info("[ImageFactory] test2");

    pair.first->inSong = false;
    il2cpp_utils::getLogger().info("[ImageFactory] test3");
    il2cpp_utils::getLogger().info("[ImageFactory] test4");
  }

  il2cpp_utils::getLogger().info("[ImageFactory] test6");
}

MAKE_HOOK_MATCH(ScoreController_Start, &ScoreController::Start, void,
                ScoreController* self) {
  ScoreController_Start(self);
  self->add_comboBreakingEventHappenedEvent(MakeDelegate(
      System::Action*, std::function([]() {
        PresentorManager::DeSpawnforAll(PresentorManager::FULL_COMBO);
      })));
}
MAKE_HOOK_MATCH(SongStart, &AudioTimeSyncController::StartSong, void,
                AudioTimeSyncController* self, float startTimeOffset) {
  for (std::pair<IFImage*, std::string> pair : *PresentorManager::MAP) {
    il2cpp_utils::getLogger().info("[ImageFactory] test2");

    pair.first->inSong = true;
  }

  PresentorManager::DeSpawnforAll(PresentorManager::IN_MENU);
  il2cpp_utils::getLogger().info("[ImageFactory] Spawning In Song");
  PresentorManager::SpawnforAll(PresentorManager::IN_SONG);
  il2cpp_utils::getLogger().info("[ImageFactory] Spawning Everywhere");
  PresentorManager::SpawnforAll(PresentorManager::EVERYWHERE);
  il2cpp_utils::getLogger().info("[ImageFactory] Spawning Full Combo");
  PresentorManager::SpawnforAll(PresentorManager::FULL_COMBO);
  il2cpp_utils::getLogger().info("[ImageFactory] Finished");
  SongStart(self, startTimeOffset);
}

MAKE_HOOK_MATCH(SongUpdate, &AudioTimeSyncController::Update, void,
                AudioTimeSyncController* self) {
  SongUpdate(self);
}

MAKE_HOOK_MATCH(PauseStart, &PauseMenuManager::ShowMenu, void,
                PauseMenuManager* self) {
  PresentorManager::SpawnforAll(PresentorManager::IN_PAUSE);
  PauseStart(self);
}

MAKE_HOOK_MATCH(
    PauseAnimationFinish,
    &PauseController::HandlePauseMenuManagerDidFinishResumeAnimation, void,
    PauseController* self) {
  PresentorManager::DeSpawnforAll(PresentorManager::IN_PAUSE);
  PauseAnimationFinish(self);
}

void PresentorHooks::AddHooks() {
  INSTALL_HOOK(getLogger(), MainMenuViewController_DidActivate);
  INSTALL_HOOK(getLogger(), ResultsViewController_DidActivate);
  INSTALL_HOOK(getLogger(), ResultsViewController_DidDeactivate);
  INSTALL_HOOK(getLogger(), SongEnd);
  INSTALL_HOOK(getLogger(), SongStart);
  INSTALL_HOOK(getLogger(), SongUpdate);
  INSTALL_HOOK(getLogger(), PauseAnimationFinish);
  INSTALL_HOOK(getLogger(), PauseStart);
  INSTALL_HOOK(getLogger(), ScoreController_Start);
}