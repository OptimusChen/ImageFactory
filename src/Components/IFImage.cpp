#include "Components/IFImage.hpp"

#include "GlobalNamespace/OVRInput.hpp"
#include "GlobalNamespace/PlayerVRControllersManager.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "GlobalNamespace/VRController.hpp"
#include "HMUI/Touchable.hpp"
#include "Presenters/PresentorManager.hpp"
#include "UnityEngine/Collider.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Physics.hpp"
#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/RaycastHit.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Time.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Vector3.hpp"
#include "Utils/FileUtils.hpp"
#include "VRUIControls/VRGraphicRaycaster.hpp"
#include "VRUIControls/VRPointer.hpp"
#include "beatsaber-hook/shared/utils/utils.h"
#include "include/PluginConfig.hpp"
#include "questui/shared/BeatSaberUI.hpp"

DEFINE_TYPE(ImageFactory::Components, IFImage);

using namespace UnityEngine;

bool isHolding = false;
float grabDist = 0.0f;
UnityEngine::Vector3 raycastPos;
UnityEngine::Quaternion raycastRot;
bool hasRendered = false;
bool hasSpawned = false;

float r = 0.0f;
float g = 0.0f;
float b = 0.0f;

custom_types::Helpers::Coroutine
ImageFactory::Components::IFImage::UpdateEveryTick() {
  while (!inSong) {
    if (screen) {
      x = screen->get_transform()->get_position().x;
      y = screen->get_transform()->get_position().y;
      z = screen->get_transform()->get_position().z;
      raycastRot = screen->get_transform()->get_rotation();
      angleX = raycastRot.get_eulerAngles().x;
      angleY = raycastRot.get_eulerAngles().y;
      angleZ = raycastRot.get_eulerAngles().z;
      r = image->get_color().r;
      g = image->get_color().g;
      b = image->get_color().b;
    }
    co_yield nullptr;
  }
  co_return;
}

void ImageFactory::Components::IFImage::Render() {
  screen = BeatSaberUI::CreateFloatingScreen(
      Vector2(scaleX * (width / 3), scaleY * (height / 3)), Vector3(x, y, z),
      Vector3(angleX, angleY, angleZ), 0.0f, false, true, 4);
  UnityEngine::Object::DontDestroyOnLoad(screen);

  image = BeatSaberUI::CreateImage(
      screen->get_transform(), sprite, Vector2(x, y),
      Vector2(scaleX * (width / 3), scaleY * (height / 3)));
  UnityEngine::Object::DontDestroyOnLoad(image);
  GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(
      reinterpret_cast<custom_types::Helpers::enumeratorT*>(
          custom_types::Helpers::CoroutineHelper::New(UpdateEveryTick())));
}

void ImageFactory::Components::IFImage::Despawn() {
  il2cpp_utils::getLogger().info("[ImageFactory] test1");
  il2cpp_utils::getLogger().info("[ImageFactory] test2");
  if (inSong) {
    if (inSongScreen) {
      inSongScreen->SetActive(true);
      if (inSongScreen->get_transform()) {
        inSongScreen->get_transform()->set_localPosition(
            Vector3(100.0f, 100.0f, 100.0f));
      }
      il2cpp_utils::getLogger().info("[ImageFactory] test3");
    }
    il2cpp_utils::getLogger().info("[ImageFactory] test4");
    if (inSongImage) {
      il2cpp_utils::getLogger().info("[ImageFactory] test5");
      inSongImage->get_gameObject()->SetActive(true);
      inSongImage->get_transform()->set_localPosition(
          Vector3(100.0f, 100.0f, 100.0f));
      inSongImage->set_color(Color(r, g, b, 0.1f));
    }
  }
  il2cpp_utils::getLogger().info("[ImageFactory] test6");

  il2cpp_utils::getLogger().info("[ImageFactory] test7");
  if (screen) {
    il2cpp_utils::getLogger().info("[ImageFactory] test8");
    screen->SetActive(true);
    screen->get_transform()->set_localPosition(Vector3(100.0f, 100.0f, 100.0f));
  } else {
    screen = BeatSaberUI::CreateFloatingScreen(
        Vector2(scaleX * (width / 3), scaleY * (height / 3)), Vector3(x, y, z),
        Vector3(angleX, angleY, angleZ), 0.0f, false, true, 4);
    UnityEngine::Object::DontDestroyOnLoad(screen);
    screen->SetActive(true);
    screen->get_transform()->set_localPosition(Vector3(100.0f, 100.0f, 100.0f));
  }
  il2cpp_utils::getLogger().info("[ImageFactory] test9");
  if (image) {
    il2cpp_utils::getLogger().info("[ImageFactory] test10");
    image->get_gameObject()->SetActive(true);
    image->get_transform()->set_localPosition(Vector3(100.0f, 100.0f, 100.0f));
    image->set_color(Color(r, g, b, 0.0f));
  } else {
    il2cpp_utils::getLogger().info("[ImageFactory] test10.1");
    image = BeatSaberUI::CreateImage(
        screen->get_transform(), sprite, Vector2(x, y),
        Vector2(scaleX * (width / 3), scaleY * (height / 3)));
    UnityEngine::Object::DontDestroyOnLoad(image);
    image->get_gameObject()->SetActive(true);
    image->get_transform()->set_localPosition(Vector3(100.0f, 100.0f, 100.0f));
    image->set_color(Color(r, g, b, 0.0f));
  }
  il2cpp_utils::getLogger().info("[ImageFactory] test11 %s", path.c_str());
}

void ImageFactory::Components::IFImage::Spawn() {
  il2cpp_utils::getLogger().info("[ImageFactory] Spawning Image");
  il2cpp_utils::getLogger().info(
      "[ImageFactory] Deleting existing images and screens");
  if (inSong) {
    il2cpp_utils::getLogger().info("[ImageFactory] creating floating screen");
    inSongScreen = BeatSaberUI::CreateFloatingScreen(
        Vector2(scaleX * (width / 3), scaleY * (height / 3)), Vector3(x, y, z),
        Vector3(angleX, angleY, angleZ), 0.0f, false, false, 4);
    UnityEngine::Object::DontDestroyOnLoad(inSongScreen);
    inSongScreen->SetActive(true);
    il2cpp_utils::getLogger().info("[ImageFactory] Creating Image");

    inSongImage = BeatSaberUI::CreateImage(
        inSongScreen->get_transform(), sprite, Vector2(x, y),
        Vector2(scaleX * (width / 3), scaleY * (height / 3)));
    UnityEngine::Object::DontDestroyOnLoad(inSongImage);
    inSongImage->set_color(Color(r, g, b, 0.0f));
    inSongImage->get_gameObject()->SetActive(true);
    il2cpp_utils::getLogger().info("[ImageFactory] done");
  }

  if (screen) {
    il2cpp_utils::getLogger().info("[ImageFactory] Creating Screen");
    screen = BeatSaberUI::CreateFloatingScreen(
        Vector2(scaleX * (width / 3), scaleY * (height / 3)), Vector3(x, y, z),
        Vector3(angleX, angleY, angleZ), 0.0f, false, true, 4);
    UnityEngine::Object::DontDestroyOnLoad(screen);
    il2cpp_utils::getLogger().info("[ImageFactory] Finished Creating Screen");
    screen->SetActive(true);
    il2cpp_utils::getLogger().info("[ImageFactory] Activated Screen");
  }
  if (image) {
    il2cpp_utils::getLogger().info("[ImageFactory] Creating Image");
    image = BeatSaberUI::CreateImage(
        screen->get_transform(), sprite, Vector2(x, y),
        Vector2(scaleX * (width / 3), scaleY * (height / 3)));
    UnityEngine::Object::DontDestroyOnLoad(image);
    il2cpp_utils::getLogger().info("[ImageFactory] Finished Creating Image");
    image->get_gameObject()->SetActive(true);
    image->set_color(Color(r, g, b, 1.0f));
    il2cpp_utils::getLogger().info("[ImageFactory] Activated Image %s",
                                   path.c_str());
  }
}

void ImageFactory::Components::IFImage::Update() {
  if (image) {
    UnityEngine::Object::Destroy(image);
  }
  image = BeatSaberUI::CreateImage(
      screen->get_transform(), sprite, Vector2(x, y),
      Vector2(scaleX * (width / 3), scaleY * (height / 3)));
  UnityEngine::Object::DontDestroyOnLoad(image);
  if (!enabled) {
    screen->SetActive(false);
    image->get_gameObject()->SetActive(false);
  } else {
    screen->SetActive(true);
    image->get_gameObject()->SetActive(true);
  }
}

void ImageFactory::Components::IFImage::ctor(UnityEngine::Sprite* sprite,
                                             Il2CppString* path) {
  this->sprite = sprite;
  this->width = sprite->get_textureRect().get_width();
  this->height = sprite->get_textureRect().get_height();
  this->x = 0.0f;
  this->y = 3.0f;
  this->z = 5.0f;
  this->name = FileUtils::GetFileName(to_utf8(csstrtostr(path)), false);
  raycastPos = Vector3(x, y, z);
  this->angleX = raycastRot.x;
  this->angleY = raycastRot.y;
  this->angleZ = raycastRot.z;
  this->enabled = true;
  scaleX = 1.0f;
  scaleY = 1.0f;
  this->presentationoption = "Everywhere";
  inSong = false;
  fileName = FileUtils::GetFileName(to_utf8(csstrtostr(path)), false);
  this->path = to_utf8(csstrtostr(path));
}
