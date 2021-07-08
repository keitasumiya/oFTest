#include "CustomCircle.hpp"

void CustomCircle::draw(){
    float radius = getRadius();
    ofPushMatrix();
    ofTranslate(getPosition().x, getPosition().y);
    ofFill();
    ofSetColor(31,127,255,100);
    ofDrawCircle(0, 0, radius);
    ofSetColor(31,127,255,100);
    ofDrawCircle(0, 0, radius*0.7);
    ofPopMatrix();
}
