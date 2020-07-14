#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "ANN_Lib.hpp"

int flagM= 0;
const int MaskTestsCount = 100;
std::vector<std::vector<double>> testsMask(MaskTestsCount);
std::vector<std::vector<double>> testsMask_o(MaskTestsCount);

std::vector<double> InputMack() {
    return testsMask[flagM % MaskTestsCount];
}
std::vector<double> OutputMask() {
    return testsMask_o[flagM++ % MaskTestsCount];
}


void PrepareMaskDataset() {
    for(int k = 0; k < MaskTestsCount; k++) {
        testsMask[k] = std::vector<double>(8, 0);
        testsMask_o[k] = std::vector<double>(2, 0);
        if (rand() % 2) {
            for(int j = 0; j < 8; j++) {
                testsMask[k][j] = rand() % 2;
            }
            testsMask_o[k][0] = 1;
            for(int j = 1; j < 8; j++) {
                if (testsMask[k][j] > testsMask[k][j - 1]) {
                    testsMask_o[k][0] = 0;
                    testsMask_o[k][1] = 1;
                    break;
                }
            }
        } else {
            int c = rand() % 9;
            for(int j = 0; j < c; j++) {
                testsMask[k][j] = 1;
            }
            testsMask_o[k][0] = 1;
        }
    }
}

void TestMaskManual(FeedforwardANN& ANN) {
    while(true) {
        std::vector<double> input_vec;
        for(int i = 0; i < 8; i++) {
            int a;
            std::cin >> a;
            input_vec.push_back(a);
        }
        std::vector<double> out = ANN.process(input_vec);
        for(int i = 0; i < out.size(); i++) {
            std::cout << out[i] << ' ';
        }
        std::cout << '\n';
    }
}


FILE* images = fopen("DigRecData/train-images.idx3-ubyte", "rb");
FILE* labels = fopen("DigRecData/train-labels.idx1-ubyte", "rb");

const int NImageTestsCount = 10;
const int NImageTestsItresPerOneSet = 1;
size_t flagIm = 0;
size_t SetCounter = 0;
size_t CurrentSet = 0; 

std::vector<std::vector<double>> testsNImage(60000);
std::vector<std::vector<double>> testsNImage_o(60000);

void PrepareNImageDataset() {
    int trash;
    fread(&trash, sizeof(int), 1, images);
    int n_of_images;
    fread(&n_of_images, sizeof(int), 1, images);
    int n_of_rows;
    fread(&n_of_rows, sizeof(int), 1, images);
    int n_of_columns;
    fread(&n_of_columns, sizeof(int), 1, images);

    fread(&trash, sizeof(int), 1, labels);
    fread(&trash, sizeof(int), 1, labels);

    unsigned char pixel;
    unsigned char iLab;
    for(int k = 0; k < 60000; k++) {
        testsNImage[k] = std::vector<double>(28 * 28);
        for(int i = 0; i < 28 * 28; i++) {
            fread(&pixel, 1, 1, images);
            testsNImage[k][i] = (double)pixel / 255;
        }
        testsNImage_o[k] = std::vector<double>(10, 0);
        fread(&iLab, 1, 1, labels);
        testsNImage_o[k][(int)iLab] = 1;
    }



}
std::vector<double> InputNImage() {
    return testsNImage[(CurrentSet % ((60000) / NImageTestsCount)) * NImageTestsCount + (flagIm % NImageTestsCount)];
    //return testsNImage[(flagIm % NImageTestsCount)];
}
std::vector<double> OutputNImage() {
    std::vector<double> out = testsNImage_o[(CurrentSet % ((60000) / NImageTestsCount)) * NImageTestsCount + (flagIm++ % NImageTestsCount)];
    //std::vector<double> out = testsNImage_o[(flagIm++ % NImageTestsCount)];
    SetCounter++;

    if (SetCounter == NImageTestsItresPerOneSet * NImageTestsCount) {
        SetCounter = 0;
        CurrentSet++;
    }
    return out;
}

FILE* imagesT = fopen("DigRecData/t10k-images.idx3-ubyte", "rb");
FILE* labelsT = fopen("DigRecData/t10k-labels.idx1-ubyte", "rb");

void PrepareNImageDatasetTest() {
    int trash;
    fread(&trash, sizeof(int), 1, imagesT);
    int n_of_images;
    fread(&n_of_images, sizeof(int), 1, imagesT);
    int n_of_rows;
    fread(&n_of_rows, sizeof(int), 1, imagesT);
    int n_of_columns;
    fread(&n_of_columns, sizeof(int), 1, imagesT);

    fread(&trash, sizeof(int), 1, labelsT);
    fread(&trash, sizeof(int), 1, labelsT);
}
std::vector<double> InputNImageTest() {
    std::vector<double> out(28 * 28);
    unsigned char pixel;
    for(int i = 0; i < 28 * 28; i++) {
        fread(&pixel, 1, 1, imagesT);
        out[i] = (double)pixel / 255;
    }
    return out;
}
int OutputNImageTest() {
    unsigned char i;
    fread(&i, 1, 1, labelsT);
    return (int)i;
}

void TestImagesManual(FeedforwardANN& ANN) {
    std::vector<double> res;

    sf::RenderWindow window(sf::VideoMode(28*10, 28*10), "Dig");
    window.setFramerateLimit(120);
    sf::Vector2i mouce_pos;

    
    std::vector<sf::RectangleShape> rectangles(28 * 28, sf::RectangleShape(sf::Vector2f(9.f, 9.f)));
    for(int i = 0; i < 28; i++) {
        for(int j = 0; j < 28; j++) {
            rectangles[i + j * 28].setPosition(i*10, j*10);
        }
    }

    std::vector<double> dig_vec;
    std::vector<double> to_ann_pic(28 * 28);

    for(int i = 0; i < 28 * 28; i++) {
        rectangles[i].setFillColor(sf::Color(0, 0, 0));
    }

    while (window.isOpen()) {   

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            mouce_pos = sf::Mouse::getPosition(window);
            if (mouce_pos.x > 30 && mouce_pos.x < 240 && mouce_pos.y > 70 && mouce_pos.y < 240) {
                if(rectangles[((int)(mouce_pos.x) / 10 - 1) + ((int)(mouce_pos.y) / 10) * 28].getFillColor().r == 0 ) {
                    rectangles[((int)(mouce_pos.x) / 10 - 1) + ((int)(mouce_pos.y) / 10) * 28].setFillColor(sf::Color(150, 150, 150));
                }
                if(rectangles[((int)(mouce_pos.x) / 10 + 1) + ((int)(mouce_pos.y) / 10) * 28].getFillColor().r == 0 ) {
                    rectangles[((int)(mouce_pos.x) / 10 + 1) + ((int)(mouce_pos.y) / 10) * 28].setFillColor(sf::Color(150, 150, 150));
                }
                if(rectangles[((int)(mouce_pos.x) / 10) + ((int)(mouce_pos.y) / 10 - 1) * 28].getFillColor().r == 0 ) {
                    rectangles[((int)(mouce_pos.x) / 10) + ((int)(mouce_pos.y) / 10 - 1) * 28].setFillColor(sf::Color(150, 150, 150));
                }
                if(rectangles[((int)(mouce_pos.x) / 10) + ((int)(mouce_pos.y) / 10 + 1) * 28].getFillColor().r == 0 ) {
                    rectangles[((int)(mouce_pos.x) / 10) + ((int)(mouce_pos.y) / 10 + 1) * 28].setFillColor(sf::Color(150, 150, 150));
                }
                rectangles[((int)(mouce_pos.x) / 10) + ((int)(mouce_pos.y) / 10) * 28].setFillColor(sf::Color(255, 255, 255));
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            while(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter));
            sf::Color im_c;
            for(int i = 0; i < 28 * 28; i++) {
                to_ann_pic[i] = (double)rectangles[i].getFillColor().r / 255;
            }
            res = ANN.process(to_ann_pic);
            std::pair<int, double> max(0, res[0]);
            for(int j = 0; j < res.size(); j++) {
                std::cout << j << ": " << res[j] << '\n';
                if (max.second < res[j]) {
                    max.second = res[j];
                    max.first = j;
                }
            }
            std::cout << "Guess: " << max.first << "\n\n";
            for(int i = 0; i < 28 * 28; i++) {
                rectangles[i].setFillColor(sf::Color(0, 0, 0));
            }

        }

        for(int i = 0; i < 28 * 28; i++) {
            window.draw(rectangles[i]);
        }

        
        window.display();

    }
    
}
void TestImages(FeedforwardANN& ANN) {
    std::vector<double> res;
    int data;
    int count = 0;
    int pass = 0;
    
    for (int i = 0; i < 10000; i++) {
        res = ANN.process(InputNImageTest());
        data = OutputNImageTest();
        std::pair<int, double> max(0, res[0]);
        for(int j = 0; j < res.size(); j++) {
            //std::cout << res[j] << '\n';
            if (max.second < res[j]) {
                max.second = res[j];
                max.first = j;
            }
        }
        count++;
        //std::cout << data << ' ' << max.first << '\n';
        if (data == max.first) {
            pass++;
        }
    }
    std::cout << "Test accuracy:"  << (double)pass/count * 100 << '\n';
}
void TestImagesGrapf(FeedforwardANN& ANN) {
    std::vector<double> res;
    int data;
    int count = 0;
    int pass = 0;

    sf::RenderWindow window(sf::VideoMode(28*10, 28*10), "Dig");
    
    std::vector<sf::RectangleShape> rectangles(28 * 28, sf::RectangleShape(sf::Vector2f(9.f, 9.f)));
    for(int i = 0; i < 28; i++) {
        for(int j = 0; j < 28; j++) {
            rectangles[i + j * 28].setPosition(i*10, j*10);
        }
    }

    std::vector<double> dig_vec;
    // run the program as long as the window is open
    int data_count = 0;
    while (window.isOpen() && (data_count++ < 60000))
    {   

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        dig_vec = InputNImageTest();
        data = OutputNImageTest();

        for(int i = 0; i < 28 * 28; i++) {
            rectangles[i].setFillColor(sf::Color(dig_vec[i]*255, dig_vec[i]*255, dig_vec[i]*255));
        }

        for(int i = 0; i < 28 * 28; i++) {
            window.draw(rectangles[i]);
        }

        res = ANN.process(dig_vec);
        std::pair<int, double> max(0, res[0]);
        for(int j = 0; j < res.size(); j++) {
            std::cout << j << ": " << res[j] << '\n';
            if (max.second < res[j]) {
                max.second = res[j];
                max.first = j;
            } 
        }
        std::cout << '\n';

        count++;
        std::cout << data << ' ' << max.first << '\n';
        if (data == max.first) {
            pass++;
        }

        std::cout << "Current accuracy:"  << (double)pass/count * 100 << '\n';
        std::cout << '\n';

        // draw everything here...
        // window.draw(...);

        // end the current frame
        window.display();

        if(data ==  max.first) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        }
        //getchar();
    }
}




int main() {
    srand(time(0));

    //PrepareMaskDataset();
    PrepareNImageDataset();
    PrepareNImageDatasetTest();

    FeedforwardANN test({28*28, 20, 10}, 0.001);

    test.load("DigRecExamples/Image91_2.ann");
    //test.load("Autosave.ann");


    //StacGradDesend(test, InputNImage, OutputNImage, 100, NImageTestsCount, false);
    //RandomHillClimbing(test, InputNImage, OutputNImage, 100, NImageTestsCount, NImageTestsItresPerOneSet, false);
    //MultyRandomHillClimbing(test, InputNImage, OutputNImage, 95, 5, NImageTestsCount);

    //test.save("Save.ann");

    //TestImages(test);
    TestImagesGrapf(test);
    //TestImagesManual(test);
    
    //------------------------------------------------------------------------------------------------------------------

    //For bit mask

    //StacGradDesend(test, InputMack, OutputMask, 100, MaskTestsCount, false);
    //RandomHillClimbing(test, InputMack, OutputMask, 98, MaskTestsCount, 100000, false);
    //MultyRandomHillClimbing(test, InputMack, OutputMask, 99, 5, MaskTestsCount);

    //TestMaskManual(test);

    return 0;
}

