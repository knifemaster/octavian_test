# octavian_test

для компиляции нужны следующие программы установленные
sudo apt-get install g++
sudo apt-get install cmake
sudo apt-get install make

в терминале для установки opengl
введите ниже следующие комманды

sudo apt update

установить glfw для оконного режима
sudo apt-get install libglfw3
sudo apt-get install libglfw3-dev

установить opengl
sudo apt install libx11-dev libxext-dev libopengl-dev
sudo apt-get install libgl1-mesa-dev libglu1-mesa-dev
sudo apt install freeglut3-dev
sudo apt-get install libglew-dev
sudo apt-get install libglu-dev
sudo apt-get libfreetype-dev

для преобразования шейдеров
sudo apt-get install glslang-tools

перейдите в директорию octavian_test build
cd octavian_test
cоздайте директорию build
mkdir build
cd build

введите команды для сборки
cmake ..
cкомпилируйте 
make

в директории build будет исполняемый файл Octavian
запускать в терминале
./Octavian
