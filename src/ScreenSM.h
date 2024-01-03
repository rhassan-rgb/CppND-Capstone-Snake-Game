#ifndef SCREEN_SM_H
#define SCREEN_SM_H

enum class Screens {
    SCREEN_WELCOME = 0,
    SCREEN_PLAYER_NAME,
    SCREEN_LEADER_BOARD,
    SCREEN_PAUSE_GAME,
    SCREEN_RESUME_GAME,
    SCREEN_GAME_START,
    SCREEN_NEW_GAME,
    SCREEN_EXIT,
    TOTAL_ITEMS
};

class ScreenSM {
   private:
    Screens _currentScreen;
    Screens _nextScreen;

   public:
    void setNextScreen(Screens);

    ScreenSM(/* args */);
    ~ScreenSM();
};

ScreenSM::ScreenSM(/* args */)
    : _currentScreen(Screens::SCREEN_WELCOME),
      _nextScreen(Screens::SCREEN_WELCOME) {}

ScreenSM::~ScreenSM() {}

#endif /*SCREEN_SM_H*/