#include "App.hpp"

int main(int argc, char *argv[]) {
    App(argc > 1 ? argv[1] : "smb.nes").run();
    return 0;
}
