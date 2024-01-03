#ifndef WELCOMEESCREEN_H
#define WELCOMEESCREEN_H
#include <functional>

#include "IScreen.h"

class WelcomeScreen : public IScreen {
   public:
    WelcomeScreen();
    ~WelcomeScreen();
    void Render() override;
    void Update() override;
    void Control(const KeyStroke& key) override;
    void Activate() override;
    void Deactivate() override;
    const std::vector<MenuItem>& GetScreenContext() const override;

   private:
    int _currentItem;
    std::vector<MenuItem> _menuItems_cpy;
};

#endif /*WELCOMEESCREEN_H*/