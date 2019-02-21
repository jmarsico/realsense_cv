#include "ofApp.h"

using namespace ofxCv;
using namespace cv;


void ofApp::setup(){
    
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_NOTICE);
    
    gui.setup();
    gui.add(resetBackground.set("Reset Background", false));
    gui.add(learningTime.set("Learning Time", 30, 0, 500));
    gui.add(thresholdValue.set("Threshold Value", 10, 0, 255));
    gui.add(radius.set("Radius", 50, 0, 100));
    gui.add(minArea.set("Min area", 10, 1, 100));
    gui.add(maxArea.set("Max area", 200, 1, 500));
    gui.add(threshold.set("Threshold", 128, 0, 255));
    gui.add(holes.set("Holes", false));
    gui.add(maxDist.set("Max Dist", 5,5,30));
    gui.add(noiseColorThresh.set("noise thresh", 5, 0, 50));
    gui.loadFromFile("settings.xml");
	// if you see app crash at runtime, please check,
	// 1. Copy Intel.Realsense.dll and realsense2.dll in to /bin? 
	// 2. Unplug and re-connect Realsense camera and restart app.
	pipe.start();
    
    //ABSDIFF, BRIGHTER, DARKER
    background.setDifferenceMode(ofxCv::RunningBackground::DARKER);
    
//    // Declare filters
//    rs2::decimation_filter dec_filter;  // Decimation - reduces depth frame density
////    rs2::threshold_filter thr_filter;   // Threshold  - removes values outside recommended range
//    rs2::spatial_filter spat_filter;    // Spatial    - edge-preserving spatial smoothing
//    rs2::temporal_filter temp_filter;   // Temporal   - reduces temporal noise
//    
    // Declare disparity transform from depth to disparity and vice versa
    const std::string disparity_filter_name = "Disparity";
    rs2::disparity_transform depth_to_disparity(true);
    rs2::disparity_transform disparity_to_depth(false);
    
//    // Initialize a vector that holds filters and their options
//    std::vector<filter_options> filters;
//
//    // The following order of emplacement will dictate the orders in which filters are applied
//    filters.emplace_back("Decimate", dec_filter);
////    filters.emplace_back("Threshold", thr_filter);
//    filters.emplace_back(disparity_filter_name, depth_to_disparity);
//    filters.emplace_back("Spatial", spat_filter);
//    filters.emplace_back("Temporal", temp_filter);

    
}

void ofApp::update(){

    // Get depth data from camera
    auto frames = pipe.wait_for_frames();
    rs2::depth_frame depth = frames.get_depth_frame();
//    auto ir = frames.get_infrared_frame();
    
    points = pc.calculate(depth);
    pix.clear();
    int w = depth.get_width();
    int h = depth.get_height();
    pix.allocate(w, h, 3);
    
    
    for(int x = 0; x < w; x++){
        for(int y = 0; y < h; y++){
            float dist = depth.get_distance(x, y);
            dist = ofClamp(dist, 0, maxDist);
            int color = ofMap(dist, 0, maxDist,0,255);
            if(color < noiseColorThresh) color = 255;
            pix.setColor(x, y, color);
        }
    }
    
   dist = depth.get_distance(ofGetMouseX(), ofGetMouseY());
    
    
    ofSetColor(255);
    img.clear();
    img.allocate(depth.get_width(), depth.get_height(), OF_IMAGE_GRAYSCALE);
    blurredImg.allocate(depth.get_width(), depth.get_height(), OF_IMAGE_GRAYSCALE);
    
    img.setFromPixels(pix);
    
    if(resetBackground) {
        background.reset();
        resetBackground = false;
    }
    
    copy(img, blurredImg);
    blur(blurredImg, radius);
    background.setLearningTime(learningTime);
    background.setThresholdValue(thresholdValue);
    background.update(blurredImg, thresholded);
    thresholded.update();
    
//    GaussianBlur(img, radius);
//    contourFinder.setMinAreaRadius(minArea);
//    contourFinder.setMaxAreaRadius(maxArea);
//    contourFinder.setThreshold(threshold);
//    contourFinder.findContours(thresholded);
//    contourFinder.setFindHoles(holes);
    
    
}

//    // Create oF mesh
//    mesh.clear();
//    int n = points.size();
//    if(n!=0){
//        const rs2::vertex * vs = points.get_vertices();
//        for(int i=0; i<n; i++){
//            if(vs[i].z){
//                const rs2::vertex v = vs[i];
//                glm::vec3 v3(v.x,v.y,v.z);
//                mesh.addVertex(v3);
//                mesh.addColor(ofFloatColor(0,0,ofMap(v.z, 2, 6, 1, 0), 0.8));
//            }
//        }
//    }
//}

void ofApp::draw(){
    
    ofBackground(200);
//    cam.begin();
//    float s = 200;
//    ofScale(s,-s,-s);
//    ofDrawAxis(1);
//
//
//
//    ofPushMatrix();
//    ofTranslate(0, 1, 0);
//    ofRotateZDeg(90);
//    ofSetColor(0,200);
//    ofDrawGridPlane(1, 5, true);
//    ofPopMatrix();
//
//    mesh.drawVertices();
//
//    cam.end();
    
    
   
    ofSetColor(255,255,255,255);
    
    ofImage bg;
    toOf(background.getBackground(), bg);
    bg.draw(0, 0, pix.getWidth()/3,pix.getHeight()/3);
    img.draw(pix.getWidth()/3 + 10, 0, pix.getWidth()/3,pix.getHeight()/3);
    blurredImg.draw(pix.getWidth()/3 + 10, pix.getHeight()/3 + 10, pix.getWidth()/3,pix.getHeight()/3);

    thresholded.draw(pix.getWidth()/3 * 2 + 20, 0, pix.getWidth()/3,pix.getHeight()/3);
    
    ofSetColor(0,255,0);
    contourFinder.draw();
    
    gui.draw();
    
    ofSetColor(0,255,0);
    ofDrawBitmapString(ofToString(dist), ofGetMouseX(), ofGetMouseY());
}

