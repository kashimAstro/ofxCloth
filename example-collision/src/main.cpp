#include "ofMain.h"
#include "ofxCloth.h"

class ofApp : public ofBaseApp
{
	public:
		Clothing cloth;
		ofEasyCam camera;
		ofLight light;
		ofMaterial material;
	        float xpos,ypos,zpos;
		bool wire,stop,wind;

		void setup()
		{
		        ofSetVerticalSync(true);
			ofSetSmoothLighting(true);
			camera.setNearClip(.1);
			camera.setDistance(100);
			light.setPosition(0,190,60);
			cloth.setup(45,45,50,50);
                        cloth.constrainPoints( 1, ofVec3f(1,0,0) ) ;
		        cloth.constrainPoints( cloth.getPlaneWidth()/2, ofVec3f(1,0,0) ) ;
		        cloth.constrainPoints( cloth.getPlaneWidth()-1, ofVec3f(-1,0,0) ) ;
			cloth.setRadiusSphere(5);
			stop=true;
			xpos=20;
			ypos=-25;
			zpos=-40;
		}

		void update()
		{
		        ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
			if(stop) zpos+=0.2;
			if(zpos>30) zpos=-40;
	                cloth.update();
	                cloth.addForce( ofVec3f(0,-0.2,0), 1.5f  );
			if(wind) cloth.windForce( ofVec3f(0.06,0,0.03), 1.8f );
			ofVec3f pos(xpos,ypos,zpos);
	                cloth.setPositionSphere(pos);
	        	cloth.sphereCollision( pos, cloth.getRadiusSphere() );
		}

		void draw() {
		        ofBackgroundGradient( ofColor(210), ofColor(10));

			ofEnableDepthTest();
		        camera.begin();

			ofEnableLighting();
               		light.enable();
	 		material.begin();

	                cloth.drawCloth(wire);
		        cloth.drawSphere(cloth.getPositionSphere());

        		material.end();
        		light.disable();
			ofDisableLighting();

		        camera.end();
			ofDisableDepthTest();
		}

		void keyPressed(int key) 
		{
		        switch (key) {
				case 'w':
					wind=!wind;
					break;
				case 'd':
					wire=!wire;
					break;
				case 's':
					stop=!stop;
					break;
		        }
		}
};

int main()
{
       ofSetupOpenGL(1024,768, OF_WINDOW);
       ofRunApp( new ofApp());
}
