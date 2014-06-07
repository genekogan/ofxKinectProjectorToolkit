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

Set your display settings to dual-screen so the projector has its own display, and launch the CALIBRATION example.

Make note of the screen resolution of the projector's display (e.g. 1280x800) and copy to `PROJECTOR_RESOLUTION_X` and `PROJECTOR_RESOLUTION_Y`.

Follow the instructions in the calibration app to collect a series of point pairs by moving the chessboard to various positions and recording point pairs, and save the calibration file.

### Mapping

There are two included examples, example-bodyMapping and example-segmentation. Both project solid colors onto objects found and tracked from the kinect using ofxCv. bodyMapping finds blobs by thresholding the depth image, whereas segmentation determines blobs by segmenting the entire depth image using Canny edges. Both examples track blobs persistently frame to frame, though this is not 100% reliable.

Make sure you load the calibration file you saved in the previous step in the line:

	kpt.loadCalibration(PATH_TO_YOUR_CALIBRTION_FILE);

The key function for mapping is `getProjectedPoint(ofVec3f worldPoint)`. This function takes any 3d world point from `ofxKinect` and converts it to a pixel point. For example, the pixel point associated with the world point inside the depth image at (x, y) is found:

	ofVec3f worldPoint = kinect.getWorldCoordinateAt(x, y);
	ofVec2f projectorPoint = kpt.getProjectedPoint(worldPoint);
