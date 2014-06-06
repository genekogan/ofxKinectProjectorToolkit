# ofxKinectProjectorToolkit

This is an [openFrameworks](http://www.openframeworks.cc) addon for calibrating a projector to a Kinect, allowing for automated projection mapping aligned to the Kinect.

This toolkit is also [implemented as a Processing library](https://github.com/genekogan/KinectProjectorToolkit). It is based on the method described by [Jan Hrdlička](http://blog.3dsense.org/programming/kinect-projector-calibration-human-mapping-2/). ofxKinectProjectorToolkit is similar in aim to [ofxCamaraLucida](https://github.com/chparsons/ofxCamaraLucida), [ofxReprojection](https://github.com/luteberget/ofxReprojection), and [ofxProjectorKinectCalibration](https://github.com/Kj1/ofxProjectorKinectCalibration).


## Dependencies

This library and the included calibration app have several dependencies:

 - ofxKinect (included with openFrameworks)
 - ofxOpenCv (included with openFrameworks)
 - [ofxCv](https://github.com/kylemcdonald/ofxCv)
 - [ofxSecondWindow](https://github.com/genekogan/ofxSecondWindow)

Additionally, the included post-calibration examples require [ofxUI](https://github.com/rezaali/ofxUI).


## Instructions

### Calibration

First, you must create the calibration. Set your display to dual-screen so the projector has its own display (right screen), and launch the CALIBRATION example.

*Very important*: make note of the screen resolution of the projector's display (e.g. 1280x800) and copy those values to the defined variables `PROJECTOR_RESOLUTION_X` and `PROJECTOR_RESOLUTION_Y`.

Follow the instructions in the calibration app to collect a series of point pairs. Move the chessboard display around using the mouse ('q' and 'r' keys resize it). Calibrate and save calibration file using 'c' and 's' keys. This will generate the calibration.xml file in the data folder of the calibration app. Copy this file somewhere.

### Mapping

There are two included examples, example-bodyMapping and example-segmentation. Both project solid colors onto objects found and tracked from the kinect using CV. The difference is that bodyMapping finds blobs by thresholding the depth image, whereas segmentation finds blobs without thresholding, instead segmenting the entire depth image using edge detection. Both examples use ofxCv for persistent blob tracking to keep identity of tracked blobs from frame to frame, though this is not 100% reliable.

Make sure you load the calibration file you saved in the previous step in the line:

	kpt.loadCalibration(PATH_TO_YOUR_CALIBRTION_FILE);

The key function for mapping is `getProjectedPoint(ofVec3f worldPoint)`. This function takes any 3d world point from `ofxKinect` and converts it to a pixel point. For example, the pixel point associated with the world point inside the depth image at (x, y) is found:

	ofVec3f wp = kinect.getWorldCoordinateAt(x, y);
	ofVec2f pp = kpt.getProjectedPoint(wp);
