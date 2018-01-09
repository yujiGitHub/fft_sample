#include "ofApp.h"
int i;
float l;
float posn[1000];
float posx[1000];
float posy[1000];
float posz[1000];
float posr[1000];
float posg[1000];
float posb[1000];
float speed;


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    mySound.loadSound("zenryoku.mp3");
    mySound.setLoop(true);
    mySound.play();
    // FFT解析初期化
    fftSmoothed = new float[8192];
    for (int i = 0; i < 8192; i++){
        fftSmoothed[i] = 0;
    }
    nBandsToGet = 32;
    
    ofToggleFullscreen();
    
    ofBackground(0);
    
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 0);
    
    for(i =0; i < 1000; i++){
        posx[i] = ofRandom(-1000,1000);
        posy[i] = ofRandom(-1000,1000);
        posz[i] = ofRandom(-1000,1000);
        posn[i] = ofRandom(5,12);
        posr[i] = ofRandom(0,225);
        posg[i] = ofRandom(0,225);
        posb[i] = ofRandom(0,225);
    }
    ofSetColor(255, 255, 255);
    
    speed = 10;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //再生中のサウンドの音量を取得
    volume = ofSoundGetSpectrum(GetPrecision);
    
    ofSoundUpdate();
    volume=ofSoundGetSpectrum(nBandsToGet);
    //FFT解析を行い、音量の平均を出す
    for (int i = 0; i < nBandsToGet; i++){
        fftSmoothed[i] *= 0.96f;
        if (fftSmoothed[i] < volume[i]) fftSmoothed[i] = volume[i];
        avgSound += fftSmoothed[i];
    }
    //volumeに値を丸める
    avgSound /= nBandsToGet;
    
    myPlayer.update();
    cam.lookAt(ofVec3f(0,0,0));
    
    if(speed == 360*5000){
        speed =0;
    }
    
    speed += 10;
    cam.setPosition( sin(speed/400)*1000,0, cos(speed/400)*1000);

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    cam.begin();
    
    for(i = 0; i < 1000; i++){
        ofSetColor(posr[i], posg[i], posb[i]);
        box.set(/*posn[i]*/40*volume[0]);
        box.setPosition(posx[i],posy[i],posz[i]);
        box.draw();
    }
    cam.end();
    float hue = fmodf(ofGetElapsedTimef()*10, 255);
    
    l = ofRandom(150, 400);
    
    ofColor c;
    c.setHsb(hue, 200, l);//(色相, 彩度, 輝度)
    ofSetColor(c);
    int start_x = 200;
    int base_y = 500;
    ofSetColor(55, 190, 200);
    float width = (float)(800) / nBandsToGet;
    for (int i = 0;i < nBandsToGet; i++){
        ofRect(start_x + i * width, base_y, width, - (fftSmoothed[i] * 400));
    }
    ofSetColor(200, 20, 105, 50);
    for (int i = 0;i < nBandsToGet; i++){
        ofRect(start_x + i * width, base_y, width, + (fftSmoothed[i] * 300));
    }

}
