#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include "IScreen.h"

class LeaderBoard : public IScreen<TextScreenItem> {
   public:
    LeaderBoard();
    ~LeaderBoard();
    bool Update() override;
    void Control(const KeyStroke& key) override;
    void Activate() override;
    void Deactivate() override;
    int GetSelection() override;
    std::string GetTitle() override;
    void WriteScore(int Score);
    const std::vector<TextScreenItem>& GetScreenContext() const;

   private:
    enum class Direction { PREVIOUS, NEXT };
    KeyStroke _pressedKey;
    std::mutex _pressedKeyLock;
    std::vector<TextScreenItem> _menuItems_cpy;
    bool _screenUpdated;

    bool _selectAction;
    void ReadLeaderBoard();
    void changeSelection(Direction dir);
};

#endif /*LEADERBOARD_H*/