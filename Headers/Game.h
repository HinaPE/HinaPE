#ifndef ASSIGNMENT_FRAMEWORK_GAME_H
#define ASSIGNMENT_FRAMEWORK_GAME_H

namespace BFU_CATP_FW {

    enum GAME_STATE {
        GAME_ACTIVE,
        GAME_PAUSE
    };

    class Game {
    public:
        GAME_STATE state;
        bool Keys[1024];
        unsigned int width;
        unsigned int height;
    public:
        Game(unsigned int width, unsigned int height);
        void init();
        void processInput();
        void update();
        void render();
    };
}

#endif //ASSIGNMENTFRAMEWORK_GAME_H
