#include "snake.h"
#include "screen.h"

#define START_SPEED 60 /*can be changed*/
#define MAX_LEVEL 30  /*can be changed*/
#define MENU_HEIGTH 8
#define MENU_WIDTH 43


CSnake::CSnake(CRect r, char _c) : CFramedWindow(r, _c) {
    level = 0;
    direction = 2;
    setHead({geom.topleft.x + geom.size.x / 2, geom.topleft.y + geom.size.y / 2});
    generateFood();
    isGameOver = false;
    isWin = false;
    isPause = true;
    isMenu = true;
    isReset = false;
    currMove = 0;
    moveCounter = 0;
}

void CSnake::drawLevel() {
    gotoyx(geom.topleft.y - 1, geom.topleft.x);
    printl("| LEVEL: %d | | OBJECTIVE: %d| | LEFT: %d |", level, MAX_LEVEL, MAX_LEVEL - level);
}

void CSnake::drawMenu() {
    if (!isMenu) {
        removeMenu();
        return;
    }

    for (int i = 0; i < MENU_WIDTH; i++) {
        gotoyx(geom.topleft.y + geom.size.y, geom.topleft.x + i + 1);
        printl("-");
    }

    for (int i = 0; i < MENU_WIDTH; i++) {
        gotoyx(geom.topleft.y + geom.size.y + MENU_HEIGTH, geom.topleft.x + i + 1);
        printl("-");
    }

    for (int i = 0; i < MENU_HEIGTH; i++) {
        gotoyx(geom.topleft.y + geom.size.y + i, geom.topleft.x + geom.size.x - 1);
        printl("|");
    }

    gotoyx(geom.topleft.y + geom.size.y + 1, geom.topleft.x);
    printl("| h - toggle help information               ");
    gotoyx(geom.topleft.y + geom.size.y + 2, geom.topleft.x);
    printl("| p - toggle pause/play mode                ");
    gotoyx(geom.topleft.y + geom.size.y + 3, geom.topleft.x);
    printl("| r - restart game                          ");
    gotoyx(geom.topleft.y + geom.size.y + 4, geom.topleft.x);
    printl("| q - quit                                  ");
    gotoyx(geom.topleft.y + geom.size.y + 5, geom.topleft.x);
    printl("| arrows - move snake (in play mode)        ");
    gotoyx(geom.topleft.y + geom.size.y + 6, geom.topleft.x);
    printl("|          or                               ");
    gotoyx(geom.topleft.y + geom.size.y + 7, geom.topleft.x);
    printl("|          move window (in pause mode)      ");

    gotoyx(geom.topleft.y + geom.size.y + 8, geom.topleft.x);
    printl("+");
    gotoyx(geom.topleft.y + geom.size.y, geom.topleft.x);
    printl("+");
    gotoyx(geom.topleft.y + geom.size.y, geom.topleft.x + geom.size.x - 1);
    printl("+");
    gotoyx(geom.topleft.y + geom.size.y + MENU_HEIGTH, geom.topleft.x + geom.size.x - 1);
    printl("+");
}

void CSnake::drawGameOver() {
    gotoyx(geom.topleft.y + geom.size.y / 2, geom.topleft.x + geom.size.x / 2 - 5);
    printl("| GAME OVER |");
    gotoyx(geom.topleft.y + geom.size.y / 2 + 1, geom.topleft.x + geom.size.x / 2 - 11);
    printl("| YOU REACHED LEVEL: %d |", level);
    gotoyx(geom.topleft.y + geom.size.y / 2 + 2, geom.topleft.x + geom.size.x / 2 - 11);
    printl("| PRESS R TO PLAY AGAIN |");

    string lose = "YOU LOST";
    for (unsigned long int i = 0; i < lose.length(); i++) {
        gotoyx(geom.topleft.y + geom.size.y / 2 - lose.length() / 2 + i, geom.topleft.x + geom.size.x + 2);
        printl("%c", lose[i]);
    }
}

void CSnake::drawWin() {
    gotoyx(geom.topleft.y + geom.size.y / 2, geom.topleft.x + geom.size.x / 2 - 5);
    printl("| YOU WON |");
    gotoyx(geom.topleft.y + geom.size.y / 2 + 1, geom.topleft.x + geom.size.x / 2 - 12);
    printl("| YOU REACHED LEVEL: %d |", level);
    gotoyx(geom.topleft.y + geom.size.y / 2 + 2, geom.topleft.x + geom.size.x / 2 - 12);
    printl("| PRESS R TO PLAY AGAIN |");

    string win = "$$$ WINNER $$$";
    for (unsigned long int i = 0; i < win.length(); i++) {
        gotoyx(geom.topleft.y + geom.size.y / 2 - win.length() / 2 + i, geom.topleft.x + geom.size.x + 2);
        printl("%c", win[i]);
    }
}

void CSnake::drawSnake() {
    for (auto &it: body) {
        gotoyx(it.y, it.x);
        if (it.x == head.x && it.y == head.y)
            printl("@");
        else
            printl("#");
    }
}

inline void CSnake::drawFood() {
    gotoyx(food.y, food.x);
    printl("$");
}

void CSnake::drawRightFrame() {
    for (int y = geom.topleft.y + 1; y < geom.topleft.y + geom.size.y - 1; y++) {
        gotoyx(y, geom.topleft.x + geom.size.x + 1);
        printl("|");
        gotoyx(y, geom.topleft.x + geom.size.x + 2);
        printl(" ");
        gotoyx(y, geom.topleft.x + geom.size.x + 3);
        printl("|");
    }
    gotoyx(geom.topleft.y + geom.size.y - 1, geom.topleft.x + geom.size.x + 2);
    printl("-");
    gotoyx(geom.topleft.y + geom.size.y - 1, geom.topleft.x + geom.size.x + 3);
    printl("+");
    gotoyx(geom.topleft.y, geom.topleft.x + geom.size.x + 2);
    printl("-");
    gotoyx(geom.topleft.y, geom.topleft.x + geom.size.x + 3);
    printl("+");
    gotoyx(geom.topleft.y + geom.size.y - 1, geom.topleft.x + geom.size.x + 1);
    printl("+");
    gotoyx(geom.topleft.y, geom.topleft.x + geom.size.x + 1);
    printl("+");
}

void CSnake::drawPause() {
    string pause = "GAME IS PAUSED";
    if (!isPause) {
        pause = "GAME IS RUNNING";
    }
    for (unsigned long int i = 0; i < pause.length(); i++) {
        gotoyx(geom.topleft.y + geom.size.y / 2 - pause.length() / 2 + i, geom.topleft.x + geom.size.x + 2);
        printl("%c", pause[i]);
    }
}

void CSnake::clearScreenFramedWindow() {
    for (int y = geom.topleft.y + 1; y < geom.topleft.y + geom.size.y - 1; y++)
        for (int x = geom.topleft.x + 1; x < geom.topleft.x + geom.size.x - 1; x++) {
            gotoyx(y, x);
            printl(" ");
        }
}

void CSnake::resetGame() {
    level = 0;
    direction = 2;
    setHead({geom.topleft.x + geom.size.x / 2, geom.topleft.y + geom.size.y / 2});
    body.clear();
    body.push_back(head);
    generateFood();
    isGameOver = false;
    isWin = false;
    isPause = true;
    isReset = false;
    moveCounter = 0;
}

void CSnake::paint() {
    CFramedWindow::paint();

    drawRightFrame();
    drawMenu();
    if (isReset) {
        clearScreenFramedWindow();
        resetGame();
        isReset = false;
    }
    if (isPause) {
        drawPause();
        drawFood();
        drawSnake();
        drawLevel();
        return;
    }

    drawFood();
    drawSnake();
    drawLevel();
    if (isGameOver) {
        clearScreenFramedWindow();
        drawGameOver();
        return;
    }
    if (isWin) {
        clearScreenFramedWindow();
        drawWin();
        return;
    }
    drawPause();
}

bool CSnake::handleMove() {
    moveCounter++;
    if (moveCounter > START_SPEED / (level + 1)) {
        moveCounter = 0;
        return true;
    }
    return false;
}

bool CSnake::handleEvent(int c) {
    currMove = c;
    if (isPause || isGameOver || isWin) {
        if (CFramedWindow::handleEvent(c)) {
            return true;
        }
        switch (c) {
            case 'p':
                if (isGameOver || isWin) {
                    return true;
                }
                if (isPause) {
                    isPause = false;
                } else {
                    isPause = true;
                }
                isMenu = true;
                break;
            case 'r':
                isReset = true;
            case 'h':
                changeMenuVisibility(isMenu);
                break;
            default:
                return false;
        }
        return true;
    }

    switch (c) {
        case KEY_UP:
            if (direction != 2) {
                direction = 1;
            } else {
                return false;
            }
            break;
        case KEY_DOWN:
            if (direction != 1) {
                direction = 2;
            } else {
                return false;
            }
            break;
        case KEY_RIGHT:
            if (direction != 4) {
                direction = 3;
            } else {
                return false;
            }
            break;
        case KEY_LEFT:
            if (direction != 3) {
                direction = 4;
            } else {
                return false;
            }
            break;
        case 'h':
            changeMenuVisibility(isMenu);
            return true;
        case 'p':
            isMenu = true;
            isPause = true;
            return true;
        case 'r':
            isReset = true;
            return true;
        case ERR:
            if (!handleMove()) {
                return false;
            }
            break;
        default:
            break;
    }
    switch (direction) {
        case 1:
            head.y--;
            break;
        case 2:
            head.y++;
            break;
        case 3:
            head.x++;
            break;
        case 4:
            head.x--;
            break;
    }

    if (checkHitBody()) {
        return true;
    }
    handleWall();
    body.push_back(head);
    handleEatFood();
    checkWin();
    return true;
}

void CSnake::move(const CPoint &delta) {
    CView::move(delta);
    for (auto &it: body) {
        it += delta;
    }
    food += delta;
    head += delta;
}

inline void CSnake::setHead(CPoint h) {
    head = h;
    body.push_back(head);
}

CPoint CSnake::generateFood() {
    for (;;) {
        food.x = rand() % (geom.size.x - 2) + geom.topleft.x + 1;
        food.y = rand() % (geom.size.y - 2) + geom.topleft.y + 1;
        bool isOnSnake = false;
        for (auto &it: body) {
            if (it.x == food.x && it.y == food.y) {
                isOnSnake = true;
                break;
            }
        }
        if (!isOnSnake) {
            break;
        }
    }
    return {food.x, food.y};
}

void CSnake::handleWall() {
    if (head.x == geom.topleft.x + geom.size.x - 1) {
        head.x = geom.topleft.x + 1;
    }
    if (head.x == geom.topleft.x) {
        head.x = geom.topleft.x + geom.size.x - 2;
    }
    if (head.y == geom.topleft.y + geom.size.y - 1) {
        head.y = geom.topleft.y + 1;
    }
    if (head.y == geom.topleft.y) {
        head.y = geom.topleft.y + geom.size.y - 2;
    }
}

bool CSnake::checkHitBody() {
    for (auto it = body.begin(); it != body.end(); it++) {
        if (it->x == head.x && it->y == head.y && it != body.begin()) {
            isGameOver = true;
            return true;
        }
    }
    return false;
}

void CSnake::handleEatFood() {
    if (head.x == food.x && head.y == food.y) {
        generateFood();
        level++;
    } else {
        body.erase(body.begin());
    }
}

bool CSnake::checkWin() {
    if (level == MAX_LEVEL) {
        isWin = true;
        return true;
    }
    return false;
}

void CSnake::removeMenu() {
    for (int y = geom.topleft.y + geom.size.y + 1; y < geom.topleft.y + geom.size.y + 6; y++)
        for (int x = geom.topleft.x; x < geom.topleft.x + geom.size.x + 1; x++) {
            gotoyx(y, x);
            printl(".");
        }
}

void CSnake::changeMenuVisibility(bool isMenuTmp) {
    if (isMenuTmp) {
        isMenu = false;
    } else {
        isMenu = true;
    }
}
