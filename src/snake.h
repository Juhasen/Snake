#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "winsys.h"
#include "vector"

class CSnake : public CFramedWindow {
private:
    int level;
    CPoint head;
    vector<CPoint> body;
    CPoint food;
    int direction;
    bool isGameOver;
    bool isWin;
    bool isPause;
    bool isMenu;
    bool isReset;
    int currMove;
    int moveCounter;

public:
    CSnake(CRect r, char _c = ' ');

    void paint();

    bool handleEvent(int c);

    void move(const CPoint &delta);
    
    bool handleMove();

    CPoint generateFood();

    inline void setHead(CPoint h);

    void drawLevel();

    void drawGameOver();

    void drawWin();

    void drawSnake();

    inline void drawFood();

    void drawMenu();

    void drawPause();

    void drawRightFrame();

    void setFPS() const;

    void clearScreenFramedWindow();

    void handleWall();

    bool checkHitBody();

    void removeMenu();

    void changeMenuVisibility(bool isHelp);

    bool checkWin();

    void handleEatFood();

    void resetGame();
};


#endif
