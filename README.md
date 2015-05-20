# ofxKinectProjectorToolkit

This is an [openFrameworks](http://www.openframeworks.cc) addon for calibrating a projector to a Kinect, allowing for automated projection mapping aligned to the Kinect.

This toolkit is also [implemented as a Processing library](https://github.com/genekogan/KinectProjectorToolkit). It is based on the method described by [Jan Hrdlička](http://blog.3dsense.org/programming/kinect-projector-calibration-human-mapping-2/). ofxKinectProjectorToolkit is similar in aim to [ofxCamaraLucida](https://github.com/chparsons/ofxCamaraLucida), [ofxReprojection](https://github.com/luteberget/ofxReprojection), and [ofxProjectorKinectCalibration](https://github.com/Kj1/ofxProjectorKinectCalibration).


## Dependencies

This library and the included calibration app can be coupled with either ofxKinect or ofxOpenNI, and have several dependencies

 * [ofxOpenNI](https://github.com/gameoverhack/ofxOpenNI): optional, only if using the _openni examples/calibration.
 * [ofxCv](https://github.com/kylemcdonald/ofxCv)
 * [ofxSecondWindow](https://github.com/genekogan/ofxSecondWindow)

If using ofxOpenNi, make sure to copy the openni config/lib folders into the data path as described in its README, or you will get compilation errors (they are not included in the example files found here).

## Instructions

### Calibration

Set your display settings to dual-screen so the projector has its own display, and launch the calibration example (or calibration_openni if using ofxOpenNi).

Make note of the screen resolution of the projector's display (e.g. 1280x800) and copy to `PROJECTOR_RESOLUTION_X` and `PROJECTOR_RESOLUTION_Y`.

Follow the instructions in the calibration app to collect a series of point pairs by moving the chessboard to various positions and recording point pairs, and save the calibration file.

### Mapping

The post-calibration example_contourMap and example_contourMap_openni demonstrate how to apply the calibration. They reproject solid colors onto the objects found and tracked from the kinect using ofxCv's contourFinder, which tracks blobs frame to frame, though persistent identification is not 100% reliable.

Make sure you load the calibration file you saved in the previous step in the line:

	kpt.loadCalibration(PATH_TO_YOUR_CALIBRTION_FILE);

The key function for mapping is `getProjectedPoint(ofVec3f worldPoint)`. This function takes any 3d world point from the Kinect and converts it to a pixel point. For example, using ofxKinect, the pixel point associated with the world point inside the depth image at (x, y) is found:

	ofVec3f worldPoint = kinect.getWorldCoordinateAt(x, y);
	ofVec2f projectorPoint = kpt.getProjectedPoint(worldPoint);

Using ofxOpenNi, the world point is acquired slightly differently, directly from the raw depth pixels.

	ofShortPixels depthPixels = kinect.getDepthRawPixels();
    ofPoint depthPoint = ofPoint(x, y, depthPixels[x +y * kinect.getWidth()]);
    ofVec3f worldPoint = kinect.projectiveToWorld(depthPoint);
    ofVec2f projectedPoint = kpt.getProjectedPoint(worldPoint);

