#pragma once

#include "ofMain.h"
#include <librealsense2/rs.hpp>
#include "ofxCv.h"
#include "example.hpp"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    rs2::pipeline pipe;
    rs2::device device;
    
    rs2::points points;
    rs2::pointcloud pc;
    
    ofVboMesh mesh;
    ofEasyCam cam;
    
//    rs2::depth_frame depth;
    ofPixels pix;
    ofPixels lastPix;
    ofImage img;
    ofImage blurredImg;
    ofImage thresholded;
    float dist;
//    rs2::process_interface& filter;
   
    ofxPanel gui;
    ofParameter<bool> resetBackground;
    ofParameter<float> learningTime, thresholdValue;
    ofParameter<int> radius;
    ofParameter<float> minArea, maxArea, threshold;
    ofParameter<bool> holes;
    ofParameter<float> maxDist;
    ofParameter<float> noiseColorThresh;
//    ofParameter<
    
    // Declare filters
    rs2::decimation_filter dec_filter;  // Decimation - reduces depth frame density
    rs2::spatial_filter spat_filter;    // Spatial    - edge-preserving spatial smoothing
    rs2::temporal_filter temp_filter;   // Temporal   - reduces temporal noise
    
    ofxCv::RunningBackground background;
    ofxCv::ContourFinder contourFinder;
    
    
    
};


//class filter_options
//{
//public:
//    filter_options(const std::string name, rs2::process_interface& filter);
//    filter_options(filter_options&& other);
//    std::string filter_name;                                   //Friendly name of the filter
//    rs2::process_interface& filter;                                       //The filter in use
////    std::map<rs2_option, filter_slider_ui> supported_options;  //maps from an option supported by the filter, to the corresponding slider
//    std::atomic_bool is_enabled;                               //A boolean controlled by the user that determines whether to apply the filter or not
//};

