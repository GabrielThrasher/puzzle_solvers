# Patrix

An edge algorithm or a color algorithm -- which one is faster at solving the same 100,000-piece puzzle?

-   Group report: https://docs.google.com/document/d/1CtHTeDsB3ek1YZuyE03X-wqkDd6sa5OKrIwJZWb6L8o/edit?usp=sharing
-   Original specs and documentation (for our reference when designing and building Patrix; NOT our submission for the report part of the project): https://docs.google.com/document/d/1sJvK5-OlWxgxBep6HvHFPAjq5-HgX8BW5DxKM1seIF0/edit?usp=sharing

We highly recommend the following while running the code as that is what it has been proven to work on:

-   IDE/Code Editor: Visual Studio (VS) Code and CLion
-   Complier: Visual Studio complier (which means you'll need Visual Studio + the "Desktop development with C++ workload" extension downloaded on your computer)

# How to Run

1. Make sure you have C++14, Python, and Node.js installed
2. For the project to run properly, you must compile the C++ program first
    1. Before you compile, make sure you have OpenCV installed (follow the sections below on how)
    2. You can compile the C++ program using CLion or an IDE/Editor of your liking or on the terminal using the CMakeLists.txt provided. It doesn't matter which method you choose, but you must produce an executable file called `puzzle_solvers` inside a `build` or `cmake-build-debug` folder.
3. Run the backend
    1. Install all dependencies by running `pip install -r requirements.txt` in the terminal
    2. Next, run the command `fastapi dev server.py`
    3. Make sure the server is running at `http://127.0.0.1:8000` or `http://localhost:8000`
4. Run the frontend
    1. If needed, open a new terminal window and type `cd patrix-app`
    2. Next, run `npm i` to install all dependencies
    3. After everything is installed, run `npm run dev` to launch the app at localhost (copy the url printed in the terminal in your browser to access the app)

# OpenCV Requirment

OpenCV helped us get the RGB colors of each pixel in an image for puzzle. So, you will need OpenCV installed on your computer to be able to run the code. If OpenCV is not installed on your computer, or you want to verify that its set up correctly to be able to run the code, follow these steps below:

## For Windows

1. Open your terminal as administrator or equivalent.
2. If you do not have Chocolatey (what is used to install OpenCV) installed on your computer, run the following command: `Set-ExecutionPolicy Bypass -Scope Process -Force
   [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
   iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))`
3. Run the following command: `choco install opencv`
4. When prompted with "Do you want to run the script?([Y]es/[A]ll - yes to all/[N]o/[P]rint): " during the execution of the previous command, type "yes" and hit enter.
5. Close your terminal and go to your environment variables on your computer (type "env" in the taskbar's search; go into the app that first pops up).
6. Click on "Environment Variables" that's on the bottom right of the pop up app.
7. Under "User variables for {folder name on your computer}", click on "Path" and then "Edit".
8. Delete any instance of OpenCV in preparation for for the new addition.
9. Click on "New", add the following path, `C:\tools\opencv\build\x64\vc16\bin` -- assuming that OpenCV was installed in the default place, which it should be if you followed Steps 3 and 4 (_NOTE_: the numbers after vc might be different -- first see if they are different and then update the path accordingly) -- then click enter.
10. Click "OK" then under "System variables" for each of the following variables listed below, click on "New" add the variable name (chars before |) and add the variable value (chars after |) and click "OK" (again, verify that the numbers after vc are correct for you):
    - `OPENCV_DIR` | `C:\tools\opencv\build\x64\vc16\lib`
    - `OPENCV_INCLUDE_PATHS` | `C:\tools\opencv\build\include`
    - `OPENCV_LINK_LIBS` | `opencv_world4100`
    - `OPENCV_LINK_PATHS` | `C:\tools\opencv\build\x64\vc16\lib`
    - `OPENCV_PATH` | `C:\tools\opencv`
11. Click on "OK" and then click on "OK" again. Restart your computer for the changes to fully take effect.
12. Make sure you are using the Visual Studio complier (which means you'll need Visual Studio + the "Desktop development with C++ workload" extension downloaded on your computer).
13. Use the CMakeList.txt file that we provided in the repo to be able to run the code.

## For MacOS

1. Make sure you have Homebrew installed.
2. Install OpenCV by running `brew install opencv`.
3. Using the provided CMakeLists.txt file, OpenCV should compile.

### You're all set now! Happy exploring!
