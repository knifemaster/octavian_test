# octavian_test

для компиляции нужны следующие программы установленные
sudo apt-get install g++
sudo apt-get install cmake
sudo apt-get install make

в терминале для установки opengl
введите ниже следующие комманды

1) 
sudo apt update

2) установить glfw для оконного режима
sudo apt-get install libglfw3
sudo apt-get install libglfw3-dev

3) установить opengl
sudo apt install libx11-dev libxext-dev libopengl-dev
sudo apt-get install libgl1-mesa-dev libglu1-mesa-dev
sudo apt install freeglut3-dev
sudo apt-get install libglew-dev
sudo apt-get install libglu-dev
sudo apt-get libfreetype-dev

3) для преобразования шейдеров
sudo apt-get install glslang-tools

4) перейдите в директорию octavian_test build
cd octavian_test
5) создайте директорию build
mkdir build
cd build

6) введите команды для сборки
cmake ..
7) скомпилируйте 
make
8) в директории build будет исполняемый файл Octavian
запускать в терминале
./Octavian
