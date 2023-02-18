#include <iostream>
#include <chrono>
#include <thread>

int main() {
    std::cout << "\033[5m"; // set blinking text mode

    const int DELAY_MS = 500; // delay between buffer redraws in milliseconds
    const int NUM_REDRAWS = 10; // number of times to redraw the buffer

    // redraw the buffer multiple times with a delay in between
    for (int i = 0; i < NUM_REDRAWS; i++) {
        std::cout << "Enter your name: \033[0m"; // reset color and redraw the input prompt
        std::cout.flush(); // flush the output stream to ensure prompt is displayed

        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS)); // delay before next redraw

        std::cout << "\033[5m"; // set blinking text mode again
    }

    // read user input after buffer has stopped flashing
    std::string name;
    std::cin >> name;
    std::cout << "Hello, " << name << "!\n";

    return 0;
}
