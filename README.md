# tobii_opencv_on_qt
Integration of Tobii eye tracking and OpenCV head gesture recognition implemented on Qt platform

# Setup guide
1. [Set up QT creator that is compatible with OpenCV](https://www.youtube.com/watch?v=ox6NFWE8xg4&lc=Ughv6GFWxv8XR3gCoAEC)
2. Clone this repository.
3. Open project file (.pro) in Qt Creator.
4. Update include path for OpenCV to corresponding absolute paths.

Note that this build is based on Qt 5.4.1, so some Qt newer functions cannot be used. If running the project results in a large number of errors, it's likely that 'CONFIG += c++11' is not included in the project file.
