#include "ofMain.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cloth_glm.h"

using namespace glm;

class Clothing
{
	public:
	Cloth* cloth1;
	glm::vec3 ball_pos; 
	float ball_radius; 

	void setup( int w, int h, int numPartWidth, int numPartHeight );
	ofVec3f getPositionSphere();
	int getPlaneWidth();
	int getPlaneHeight();
	float getRadiusSphere();

	void update();
	void setPositionSphere(ofVec3f pos);
	void setRadiusSphere( float radius );
        void addForce( ofVec3f dir, float time  );
	void windForce( ofVec3f dir, float time );
	void sphereCollision( ofVec3f pos, float radius );
	void constrainPoints( int index, ofVec3f offSetPos );

	void drawCloth( bool wire );
	void drawSphere( ofVec3f pos );
};
