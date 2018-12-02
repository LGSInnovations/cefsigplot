1. Download http://opensource.spotify.com/cefbuilds/cef_binary_3.3538.1852.gcb937fc_linux64_minimal.tar.bz2
2. Extract cef_binary_3.3538.1852.gcb937fc_linux64_minimal.tar.bz2
3. export CEF_ROOT=$PWD/cef_binary_3.3538.1852.gcb937fc_linux64_minimal
4. git clone https://github.com/LGSInnovations/cefsigplot.git
5. cd cefsigplot
6. mkdir build
7. cd build
8. cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
9. make
1. Download http://opensource.spotify.com/cefbuilds/cef_binary_3.3538.1852.gcb937fc_linux64_minimal.tar.bz2
2. Extract cef_binary_3.3538.1852.gcb937fc_linux64_minimal.tar.bz2
3. export CEF_ROOT=$PWD/cef_binary_3.3538.1852.gcb937fc_linux64_minimal
4. git clone https://github.com/LGSInnovations/cefsigplot
5. cd cefsigplot
6. mkdir build
7. cd build
8. cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
9. make
10. ./cefsigplot/Release/cefsigplot
11. cd app
12. npm install
13. npm install -g webpack-cli webpack
13. webpack ./main.js --output app.js
