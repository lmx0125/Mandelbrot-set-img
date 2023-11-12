# Compiling with build.sh

To compile your project using `build.sh`, follow these steps:

1. **Clone the Repository:**
    ```bash
    git clone https://github.com/lmx0125/Mandelbrot-set-img.git
    cd Mandelbrot-set-img

2. **Build:**
    ```bash for linux
    ./build.sh

The build.sh script is designed to automate the compilation process.

3.**Install Dependencies (if needed):**

If the compilation fails due to a missing dependency, you may need to install the required library. For example, if the error mentions libx11-dev, use the following command:

    ```bash
    sudo apt-get update
    sudo apt-get install libx11-dev

This installs the necessary development files for the libx11 library.

4.**Retry the Build:**
After installing the dependencies, rerun the ./build.sh script.

5.**Check for Success:**
If the compilation is successful, you should find executeable file called "set" or "set.exe"(if you change the build.sh)
