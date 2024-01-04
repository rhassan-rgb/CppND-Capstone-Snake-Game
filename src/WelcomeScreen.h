#ifndef WELCOMEESCREEN_H
#define WELCOMEESCREEN_H
#include <functional>

#include "IScreen.h"

class WelcomeScreen : public IScreen<TextScreenItem> {
   public:
    WelcomeScreen();
    ~WelcomeScreen();
    bool Update() override;
    void Control(const KeyStroke& key) override;
    void Activate() override;
    void Deactivate() override;
    int GetSelection() override;
    std::string GetTitle() override;
    const std::vector<TextScreenItem>& GetScreenContext() const;

   private:
    enum class Direction { PREVIOUS, NEXT };
    KeyStroke _pressedKey;
    std::mutex _pressedKeyLock;
    std::vector<TextScreenItem> _menuItems_cpy;
    bool _screenUpdated;

    bool _selectAction;
    bool handlePressedKey();
    void changeSelection(Direction dir);
};

#endif /*WELCOMEESCREEN_H*/