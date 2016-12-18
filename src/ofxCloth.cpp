#include "ofMain.h"
#include "ofxCloth.h"

using namespace glm;

void Clothing::setup( int w, int h, int numPartWidth, int numPartHeight ){
	cloth1 = new Cloth(  w, h, numPartWidth, numPartHeight );
}

void Clothing::setPositionSphere(vector<ofVec3f> pos){
	ball_pos.clear();
	for(int i = 0; i < pos.size(); i++) 
	{
		vec3 p(pos[i].x,pos[i].y,pos[i].z);
		ball_pos.push_back(p);
		//ball_pos[0] = (float)pos.x;
		//ball_pos[1] = (float)pos.y;
		//ball_pos[2] = (float)pos.z;
	}
}

vector<ofVec3f> Clothing::getPositionSphere() {
	vector<ofVec3f> vec;
	for(int i = 0; i < ball_pos.size(); i++){
		vec.push_back( ofVec3f(ball_pos[i][0],ball_pos[i][1],ball_pos[i][2]) );
	}
	return vec;
}

int Clothing::getPlaneWidth(){
	return cloth1->getPointsWidth();
}

int Clothing::getPlaneHeight(){
	cloth1->getPointsHeight();
}

void Clothing::setRadiusSphere( vector<float> radius ) {
	ball_radius.clear();
	ball_radius=radius;
}

vector<float> Clothing::getRadiusSphere(){
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

void Clothing::sphereCollision( vector<ofVec3f> pos, vector<float> radius ){
	ball_radius=radius;
	vector<vec3> vv;
	vector<float> rr;
	for(int i = 0; i < pos.size(); i++){
		vv.push_back(vec3(pos[i].x,pos[i].y,pos[i].z));
		rr.push_back(radius[i]);
	}
	cloth1->ballCollision(vv,rr); 
}

void Clothing::pointCollision( ofVec3f center, ofVec3f coord, float surface ){
	cloth1->dotCollision(vec3(center.x,center.y,center.z),vec3(coord.x,coord.y,coord.z),surface); 
}

void Clothing::constrainPoints( int index, ofVec3f offSetPos ) {
	cloth1->Blockpoints( index, vec3(offSetPos.x,offSetPos.y,offSetPos.z));
}

void Clothing::drawCloth( bool wire ) {
	ofPushMatrix();
	if(wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	cloth1->drawShaded();
	ofPopMatrix();
}
