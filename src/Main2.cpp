#include <iostream>
#include <cstdlib>
#include <thread>
#include <atomic>
#include <chrono>
#include <cmath>
#include <SDL2/SDL.h>
#include <vector>
#include "../includes/NeuralNet.h"
#include "../includes/Matrix.h"
#include "../includes/Button.h"

using namespace std;

bool init();
void close();
static void independantTraining();
float Xor(float x, float y);
float Sine(float x, float garbage = 0);
float Cosine(float x, float garbage = 0);
void setXor();
Matrix<double> XorData[] = {
    Matrix<>(2, 1),
    Matrix<>(2, 1),
    Matrix<>(2, 1),
    Matrix<>(2, 1),
    Matrix<>(1, 1),
    Matrix<>(1, 1),
    Matrix<>(1, 1),
    Matrix<>(1, 1),
};
void setSine();
void setCos();
void setReLU();
void setTanh();
void setSigmoid();
std::atomic<bool> calculatingFrame(false); //main set true to start frame, func set false to continue main loop
std::atomic<bool> shutdownRequested(false); //main set true to terminate thread
std::atomic<bool> active(false); //thread sets false just before returning to signal main
std::atomic<float(*)(float, float) > mathfunc(Sine);
std::atomic<NeuralNet::functionType> activationfunc = NeuralNet::tanh;
std::atomic<int> numIn = 1;
std::atomic<int> numNodes = 250;
std::atomic<float> learningRate = .001;
std::thread* networkTraining;
const int FPS = 60;

double randRad();

const int SCREEN_WIDTH = 1280; //2560;
const int SCREEN_HEIGHT = 920; //1375;
const int X_RES = 10;
const int Y_RES = 10;



SDL_Window* window;
SDL_Surface* surface;
SDL_Renderer* gRenderer;
TTF_Font* gFont;

void clicked() {
    cout << "i have been clicked" << endl;
}

int main(int argc, char* argv[]) {
    srand((unsigned) time(NULL));

    chrono::high_resolution_clock::time_point t1;
    chrono::high_resolution_clock::time_point t2;

    if (!init()) {
        return 0;
    }

    vector<Button*> Buttons(6);

    Buttons[0] = new Button(SCREEN_WIDTH - 80, SCREEN_HEIGHT - 190, 20, 100, surface, setSine, gRenderer, string("sin"), gFont);
    Buttons[1] = new Button(SCREEN_WIDTH - 80, SCREEN_HEIGHT - 130, 20, 100, surface, setCos, gRenderer, string("cos"), gFont);
    Buttons[5] = new Button(SCREEN_WIDTH - 80, SCREEN_HEIGHT - 70, 20, 100, surface, setXor, gRenderer, string("xor"), gFont);
    Buttons[2] = new Button(SCREEN_WIDTH - 230, SCREEN_HEIGHT - 190, 20, 100, surface, setTanh, gRenderer, string("tanh"), gFont);
    Buttons[3] = new Button(SCREEN_WIDTH - 230, SCREEN_HEIGHT - 130, 20, 100, surface, setReLU, gRenderer, string("relu"), gFont);
    Buttons[4] = new Button(SCREEN_WIDTH - 230, SCREEN_HEIGHT - 70, 20, 100, surface, setSigmoid, gRenderer, string("sigmoid"), gFont);

    for (Button* b : Buttons) {
        b->setColor(200, 200, 200);
        b->setHighlightColor(100, 100, 100);
        b->setClickColor(30, 30, 30);
        setState(*b, Button::neutral);
    }

    networkTraining = new std::thread(independantTraining);
    bool running = true;
    SDL_Event e;
    while (running) {
        t1 = chrono::high_resolution_clock::now();
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (e.type == SDL_MOUSEMOTION) {
                for (Button* b : Buttons) {
                    setState(*b, e.motion);
                }
                //setState(test, e.motion);
            } else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
                for (Button* b : Buttons) {
                    setState(*b, e.button);
                }
                //setState(test, e.button);
            }
        }

        int waitTime = 0;
        calculatingFrame.store(true);
        while (calculatingFrame.load() == true) {
        }

        for (Button* b : Buttons) {
            b->draw();
        }

        SDL_UpdateWindowSurface(window);

        t2 = chrono::high_resolution_clock::now();
        double duration = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
        if (duration < ((1.0 / FPS) * 1000)) {
            SDL_Delay(((1.0 / FPS * 1000) - duration));
        }
    }

    close();
    networkTraining->join();

    return 0;
}

bool init() {

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            XorData[i * 2 + j].set(0, 0, j);
            XorData[i * 2 + j].set(1, 0, i);
        }
    }

    for (int i = 0; i < 4; i++) {
        XorData[i + 4].set(0, 0, XorData[i].get(0, 0) == XorData[i].get(1, 0) ? 0 : 1);
    }

    bool success = true;
    if (TTF_Init() == -1) {
        cout << "ttf init didn't initialize correctly :P" << endl;
    }
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        success = false;
    } else {
        window = SDL_CreateWindow("Neural Net Test >:3", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            success = false;
        } else {
            surface = SDL_GetWindowSurface(window);
            SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 255));
            SDL_UpdateWindowSurface(window);
        }
    }
    gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    gFont = TTF_OpenFont("resources/TravelingTypewriter.ttf", 30);

    return success;
}

double randRad() {
    double maxradians = 2.0 * 3.14159;
    double random = static_cast<double> (rand()) / static_cast<double> (RAND_MAX);
    random *= maxradians;
    return random;
}

void close() {
    SDL_DestroyWindow(window);
    shutdownRequested.store(true);
    SDL_Quit();
}

void independantTraining() {
    active.store(true);
    int trainingCycle = 0;
    NeuralNet* net = new NeuralNet(numIn.load(), numNodes.load(), 2, 1, learningRate.load(), (int) activationfunc.load());
    Matrix<> input(numIn.load(), 1);
    Matrix<> output(1, 1);
    Matrix<> tinput(numIn.load(), 1);
    Matrix<> toutput(1, 1);
    float(*trainingFunction)(float, float) = mathfunc.load();
    while (shutdownRequested.load() != true) {
        if (calculatingFrame.load()) {
            double accuracy = 0;
            double rmsaccuracy = 0;
            double height = 0;
            double in;
            double out;
            Uint8 color;
            float intensity = 0;
            SDL_Rect region;
            region.w = X_RES;
            region.h = Y_RES;
            int rows = (int) (SCREEN_HEIGHT / Y_RES);
            int cols = (int) (SCREEN_WIDTH / X_RES);
            SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 255));
            if (mathfunc.load() != Xor) {
                for (int i = 0; i < cols; i++) {
                    input.set(0, 0, (2 * 3.14159) * (double) i / (double) cols);
                    in = input.get(0, 0);
                    out = (double) trainingFunction(in, 0) / 2.0 + 0.5;
                    output.copy(net->feedForward(input));
                    height = output.get(0, 0);
                    rmsaccuracy += pow((out - height), 2);
                    double error = (out - height) / out;
                    if (error < 0) {
                        error *= -1;
                    }
                    if (out != 0) {
                        accuracy += error;
                    }
                    height = height * -1 + 1;
                    region.x = i * X_RES;
                    region.y = height * SCREEN_HEIGHT; //for sigmoid
                    region.y = (out * -1 + 1) * SCREEN_HEIGHT;
                    SDL_FillRect(surface, &region, SDL_MapRGB(surface->format, 0, 0, 0));
                    region.y = height * SCREEN_HEIGHT;
                    SDL_FillRect(surface, &region, SDL_MapRGB(surface->format, 0, 0, 255));
                }
            } else {
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        input.set(0, 0, (double) i / (double) rows);
                        input.set(1, 0, (double) j / (double) cols);
                        intensity = net->feedForward(input).get(0, 0);
                        color = (Uint8) (intensity * 255);
                        region.x = j * X_RES;
                        region.y = i * Y_RES;
                        SDL_FillRect(surface, &region, SDL_MapRGB(surface->format, color, color, color));
                    }
                }
            }
            accuracy /= cols;
            rmsaccuracy /= cols;
            rmsaccuracy = sqrtf(rmsaccuracy);
            rmsaccuracy = 1 - rmsaccuracy;
            calculatingFrame.store(false);
        } else {
            trainingCycle++;

            if (mathfunc.load() != Xor) {
                double maxradians = 2.0 * 3.14159;
                double random = static_cast<double> (rand()) / static_cast<double> (RAND_MAX);
                random *= maxradians;
                tinput.set(0, 0, random);
                toutput.set(0, 0, (double) trainingFunction(random, 0) / 2.0 + 0.5);
                net->train(tinput, toutput);
            } else {
                int index = rand() % 4;
                net->train(XorData[index], XorData[index + 4]);
            }
            if (trainingCycle % 1000 == 0) {
                cout << "Completed Training Session " << trainingCycle << endl;
            }
        }
    }
    shutdownRequested.store(false);
    active.store(false);
}

void setXor() {
    shutdownRequested.store(true);
    networkTraining->join();
    mathfunc.store(Xor);
    numIn.store(2);
    numNodes.store(25);
    learningRate.store(.01);
    networkTraining = new std::thread(independantTraining);
}

void setSine() {
    shutdownRequested.store(true);
    networkTraining->join();
    mathfunc.store(Sine);
    numIn.store(1);
    numNodes.store(50);
    learningRate.store(.001);
    networkTraining = new std::thread(independantTraining);
}

void setCos() {
    shutdownRequested.store(true);
    networkTraining->join();
    mathfunc.store(Cosine);
    numIn.store(1);
    numNodes.store(50);
    learningRate.store(.001);
    networkTraining = new std::thread(independantTraining);
}

void setTanh() {
    shutdownRequested.store(true);
    networkTraining->join();
    activationfunc.store(NeuralNet::tanh);
    networkTraining = new std::thread(independantTraining);
}

void setReLU() {
    shutdownRequested.store(true);
    networkTraining->join();
    activationfunc.store(NeuralNet::relu);
    networkTraining = new std::thread(independantTraining);
}

void setSigmoid() {
    shutdownRequested.store(true);
    networkTraining->join();
    activationfunc.store(NeuralNet::sigmoid);
    networkTraining = new std::thread(independantTraining);
}

float Xor(float x, float y) {
    return x != y;
}

float Sine(float x, float garbage) {
    return sin(x);
}

float Cosine(float x, float garbage) {
    return cos(x);
}
