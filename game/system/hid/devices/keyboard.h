#pragma once

#include "input_device.h"

// A simple way to store the current state of the keyboard
// if we are polling every frame, we need a point of reference to know
// how the PadData has to be modified
//
// For example if the keyboard no longer has "W" pressed is it:
// - because it was pressed before and the user let go of it (adjust analog value)
// - because it was never pressed at all (noop!)
//
// Likewise, when modifiers are let go, we can use this to enumerate through to see what has to be
// invalidated
struct ActiveKeyboardAction {
  u32 sdl_keycode;
  InputBinding binding;
  std::function<void(std::shared_ptr<PadData>, InputBinding bind)> revert_action;
};

class KeyboardDevice : public InputDevice {
 public:
  struct KeyboardKeyStatus {
    bool w = false;
    bool a = false;
    bool s = false;
    bool d = false;
    bool r = false;
    bool q = false;
    bool space = false;
    bool shift = false;
    bool ctrl = false;
    bool alt = false;
    bool esc = false;
    bool k_0 = false;
    bool k_1 = false;
    bool k_2 = false;
    bool k_3 = false;
    bool k_4 = false;
    bool k_5 = false;
    bool k_6 = false;
    bool k_7 = false;
    bool k_8 = false;
    bool k_9 = false;
  };

  KeyboardDevice() {};
  KeyboardDevice(std::shared_ptr<game_settings::InputSettings> settings);
  ~KeyboardDevice() {}

  // Polls the entire state of the keyboard to set the PadData
  void poll_state(std::shared_ptr<PadData> data);
  void clear_actions(std::shared_ptr<PadData> data);
  void process_event(const SDL_Event& event,
                     const CommandBindingGroups& commands,
                     std::shared_ptr<PadData> data,
                     std::optional<InputBindAssignmentMeta>& bind_assignment) override;
  KeyboardKeyStatus get_key_status() const { return m_key_status; }
  // clang-format off
  void close_device() override {
    // there is nothing to close
  };
  // clang-format on

 private:
  std::vector<ActiveKeyboardAction> m_active_actions = {};
  // When we assign a bind on a keydown, we have to ignore it's keyup event
  // for analog binds, or it will adjust the position
  std::optional<u32> m_ignore_key_on_keyup;

  bool is_action_already_active(const u32 sdl_keycode);
  KeyboardKeyStatus m_key_status;
};
