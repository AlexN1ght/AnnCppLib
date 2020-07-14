# Easy to use library to create Neural Networks in C++

## Instalation 
It is a static library *ANN_Lib.hpp*, just download it and include to your project/

## Usage
Create a ann: 

```cpp
FeedforwardANN test({10, 5, 10});
```

Write functions for training (function for input data and function for output data):
```cpp
std::vector<double> InputMack() {
    return testsMask[flagM % MaskTestsCount];
}
std::vector<double> OutputMask() {
    return testsMask_o[flagM++ % MaskTestsCount];
}
```

Train your nethwork with one of the algorithms:
```cpp
StacGradDesend(test, InputMack, OutputMask, 98, MaskTestsCount, false);
RandomHillClimbing(test, InputMack, OutputMask, 98, MaskTestsCount, 100000, false);
MultyRandomHillClimbing(test, InputMack, OutputMask, 99, 5, MaskTestsCount);
```

Execute your ANN:
```cpp
std::vector<double> out = ANN.process(input_vec);
```
-----------------------------------------------------------------------------------
For more information see example files and library sources.


## Examples
This repo includes 2 exemples of what this lib can do!


1. Rigital recognition. Main file is main.cpp. type 

```
make
./main
```

to try it out.

2. Snake. Main file is snake.cpp. type

```
make snake
./snake
```
to try it out.

------------------------------------------------------------------------------------

You can change ann's and try other mods in sourse files (main.cpp and snake.cpp)

**P.S.** You need SFML Library in order to thees examples to work
