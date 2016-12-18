#include "ofMain.h"
#include "ofxCloth.h"

using namespace glm;

void Clothing::setup( int w, int h, int numPartWidth, int numPartHeight ){
	ball_pos = vec3(7,-5,0);
	ball_radius = 4;
	cloth1 = new Cloth(  w, h, numPartWidth, numPartHeight );
}

void Clothing::setPositionSphere(ofVec3f pos){
	ball_pos[0] = (float)pos.x;
	ball_pos[1] = (float)pos.y;
	ball_pos[2] = (float)pos.z;
}

ofVec3f Clothing::getPositionSphere() {
	return ofVec3f(ball_pos[0],ball_pos[1],ball_pos[2]);
}

int Clothing::getPlaneWidth(){
	return cloth1->getPointsWidth();
}

int Clothing::getPlaneHeight(){
	cloth1->getPointsHeight();
}

void Clothing::setRadiusSphere( float radius ) {
	ball_radius=radius;
}

float Clothing::getRadiusSphere(){
	return ball_radius;
}

void Clothing::update(){
	cloth1->timeStep();
}

void Clothing::addForce( ofVec3f dir, float time  ){
	cloth1->addForce(vec3(dir.x,dir.y,dir.z)*time);
}

void Clothing::windForce( ofVec3f dir, float time ){
	cloth1->windForce(vec3(dir.x,dir.y,dir.z)*time);
}

void Clothing::sphereCollision( ofVec3f pos, float radius ){
	ball_radius=radius;
	cloth1->ballCollision(vec3(pos.x,pos.y,pos.z),radius); 
}

void Clothing::constrainPoints( int index, ofVec3f offSetPos ) {
	cloth1->Blockpoints( index, vec3(offSetPos.x,offSetPos.y,offSetPos.z));
}

void Clothing::drawCloth( bool wire ) {
	if(wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	cloth1->drawShaded();
}

void Clothing::drawSphere( ofVec3f pos ) {
	ofPushMatrix();
	ofTranslate(pos); 
	ofDrawSphere( ball_radius-0.1 );
	ofPopMatrix();
}
