#include "ofMain.h"
#include "ofxCloth.h"

class ofApp : public ofBaseApp
{
	public:
		Clothing cloth;
		ofEasyCam camera;
		ofLight light;
		ofMaterial material;
	        float zpos;
		bool wire,stop,wind;

		void setup()
		{
		        ofSetVerticalSync(true);
			ofSetSmoothLighting(true);
			camera.setNearClip(.1);
			camera.setDistance(100);
			light.setPosition(0,190,60);
			cloth.setup(45,45,40,40);
                        cloth.constrainPoints( 1, ofVec3f(1,0,0) ) ;
		        cloth.constrainPoints( cloth.getPlaneWidth()/2, ofVec3f(1,0,0) ) ;
		        cloth.constrainPoints( cloth.getPlaneWidth()-1, ofVec3f(-1,0,0) ) ;
			stop=true;
			zpos=-40;
		}

		void update()
		{
		        ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
			vector<ofVec3f> position;
			vector<float> radius;

			if(stop)    zpos+=0.2;
			if(zpos>30) zpos=-40;
	                cloth.update();
	                cloth.addForce( ofVec3f(0,-0.2,0), 1.5f  );
			if(wind) cloth.windForce( ofVec3f(0.06,0,0.03), 1.8f );

			ofVec3f pos(20,-25,zpos);
			ofVec3f pos1(30,-25,zpos);

			position.push_back(pos);
			position.push_back(pos1);
			radius.push_back(10);
			radius.push_back(10);

			cloth.setRadiusSphere(radius);
	                cloth.setPositionSphere(position);
	        	cloth.sphereCollision(position,radius);
		}

		void draw() 
		{
		        ofBackgroundGradient( ofColor(210), ofColor(10));

			ofEnableDepthTest();
		        camera.begin();

			ofEnableLighting();
               		light.enable();
	 		material.begin();

	                cloth.drawCloth(wire);
			vector<ofVec3f> posphere = cloth.getPositionSphere();
			for(int i = 0; i < posphere.size(); i++){
				ofDrawSphere(posphere[i],cloth.getRadiusSphere()[i]);
			}

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
