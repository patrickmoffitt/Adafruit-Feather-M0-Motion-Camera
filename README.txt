If you get compiler errors like this:

    undefined reference to 'std::ios_base::Init::~Init()'

and, a lot more saying undefined reference to something in the namespace std::,
it means that the linking step couldn't find the C++ Standard Library. 

To solve this, edit ${INSTALL_FOLDER}/Arduino15/packages/adafruit/hardware/samd/1.2.1/platform.txt

1. Locate this line:
  
    compiler.c.elf.cmd=arm-none-eabi-gcc

2. Change the line to:

    compiler.c.elf.cmd=arm-none-eabi-g++

You should now be able to compile and link without further problems.

