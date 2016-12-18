#include "ofAppGLFWWindow.h"
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxFakeShadowMap.h"
#include "ofxGui.h"
#include "ofxCloth.h"

class ofApp : public ofBaseApp{

	public:
		ofPlanePrimitive plane;
		vector<Clothing> cloth;
                ofxFakeShadowMap shadow;
		ofEasyCam camera;
		ofLight pLight,sLight,dLight;
		ofMaterial material;
		ofxAssimpModelLoader model;
	        ofMesh mesh0,mesh1;

		int counter = 0;
		int speedanim = 40;
	        float animationPosition;
		bool wire = false;
		bool stop = true;
		bool wind = false;

		void setup() {
			ofSetSmoothLighting(true);
    			pLight.setDiffuseColor( ofColor(155.f, 255.f, 0.f));
        		pLight.setSpecularColor( ofColor(255.f, 255.f, 0.f));
		        pLight.setPointLight();
			sLight.setDiffuseColor( ofColor(255.f, 0.f, 0.f));
        		sLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));
    			sLight.setSpotlight();
    			sLight.setSpotlightCutOff( 50 );
    			sLight.setSpotConcentration( 45 );
			dLight.setDiffuseColor(ofColor(255.f, 0.f, 255.f));
		        dLight.setSpecularColor(ofColor(255.f, 5.f, 255.f));
		        dLight.setDirectional();
        		dLight.setOrientation( ofVec3f(0, 90, 0) );
        		material.setShininess( 120 );
        		material.setSpecularColor(ofColor(25, 25, 255, 255));
			camera.setNearClip(.1);
			camera.setDistance(50);

			for(int i = 0; i < 4; i++)
			{
				Clothing clo;
				clo.setup((int)ofRandom(2,5),(int)ofRandom(25,40),35,35);
	                        clo.constrainPoints( 1, ofVec3f(1,0,0) ) ;
			        clo.constrainPoints( clo.getPlaneWidth()-1, ofVec3f(-1,0,0) ) ;
				cloth.push_back(clo);
			}

			model.loadModel("model.dae",true);
			mesh0 = model.getMesh(0);
	                mesh1 = model.getMesh(1);
			shadow.setup();
		        shadow.setColors(ofColor(0,0,0));
		        shadow.setAlpha(1.f);

			plane.set(150,150);
			plane.rotate(180,ofVec3f(0,1,1));
		}

		void update() {
			ofSetWindowTitle(ofToString(ofGetFrameRate()));
			model.update();
	                animationPosition = ofMap(counter++,0,speedanim,0.0,1.0);
        	        if(counter>speedanim) counter=0;
	                model.setPositionForAllAnimations(animationPosition);
	                mesh0 = model.getCurrentAnimatedMesh(0);
	                mesh1 = model.getCurrentAnimatedMesh(1);

		        shadow.setDirLight(ofVec3f(0,0,0));
       			for(int i = 0; i < cloth.size(); i++){	
		                cloth[i].update();
		                cloth[i].addForce( ofVec3f(ofRandom(-0.2,0.2),ofRandom(0.4,-0.4),0), ofRandom(-1.f,1.f) );
				cloth[i].windForce( ofVec3f(ofRandom(-0.2,0.2),0,ofRandom(0.4,0.65)), ofRandom(2,4) );
			}
		}

		void draw() {
		        ofBackgroundGradient( ofColor(210), ofColor(10));

		        camera.begin();
			ofEnableDepthTest();
			plane.draw();
			ofDisableDepthTest();

			render();

		        shadow.begin(camera);
		                render();
		        shadow.end();

		        camera.end();
		}

		void render(){

			ofEnableLighting();
        		material.begin();
        		pLight.enable();
		        sLight.enable();
		        dLight.enable();
			ofEnableDepthTest();

			ofPushMatrix();
			ofTranslate(-2,16,-1.2);
			for(int i = 0; i < cloth.size(); i++)
		                cloth[i].drawCloth(wire);
			ofPopMatrix();

			ofPushMatrix();
			ofRotateY(-180);
			ofSetColor(0,0,200);
	                mesh0.draw();
	                ofSetColor(0,150,100);
	                mesh1.draw();
			ofPopMatrix();

			ofDisableLighting();
        		material.end();
        		pLight.disable();
		        sLight.disable();
		        dLight.disable();
			ofDisableDepthTest();
		}


		void keyPressed(int key) {
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
				case 'i':
					cloth.clear();
					for(int i = 0; i < 3; i++){
						Clothing clo;
						clo.setup((int)ofRandom(2,5),(int)ofRandom(25,40),(int)ofRandom(35,50),(int)ofRandom(35,50));
						clo.constrainPoints( 1, ofVec3f(1,0,0) ) ;
						clo.constrainPoints( clo.getPlaneWidth()-1, ofVec3f(-1,0,0) ) ;
						cloth.push_back(clo);
					}
					break;
		        }
		}
};

int main()
{
       ofSetupOpenGL(1024,768, OF_WINDOW);
       ofRunApp( new ofApp());
}
