# tobii_opencv_on_qt
Integration of Tobii eye tracking and OpenCV head gesture recognition implemented on QT platform

# Setup guide
[Set up QT creator that is compatible with OpenCV](https://www.youtube.com/watch?v=ox6NFWE8xg4&lc=Ughv6GFWxv8XR3gCoAEC)

(Note that this build is based on QT 5.4.1, so some QT newer functions cannot be used.)

Copy and paste everything to QT project file (.pro). Make sure include path for OpenCV is updated.

(If running the project results in a large number of errors, it's likely that 'CONFIG += c++11' is not included in the project file.)
