#include "ofMain.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define DAMPING 0.01f 
#define TIME_STEPSIZE2 0.5f*0.5f
#define CONSTRAINT_ITERATIONS 25 
#define GLM_PRECISION_HIGHP_FLOAT

using namespace glm;

class Particle{
private:
	bool movable; 
	float mass; 
	vec3 pos;
	vec3 old_pos; 
	vec3 acceleration; //
	vec3 accumulated_normal; // 
public:
	Particle(vec3 pos) : pos(pos), old_pos(pos),acceleration(vec3(0,0,0)), mass(1), movable(true), accumulated_normal(vec3(0,0,0)){}
	Particle(){}

	void addForce(vec3 f)
	{
		acceleration += f/mass;
	}

	void timeStep()
	{
		if(movable)
		{
			vec3 temp = pos;
			pos = pos + (pos-old_pos)*(1.0f-DAMPING) + acceleration*TIME_STEPSIZE2;
			old_pos = temp;
			acceleration = vec3(0,0,0); //
		}
	}

	vec3& getPos() {return pos;}

	void resetAcceleration() {acceleration = vec3(0,0,0);}

	void offsetPos(const vec3 v) { if(movable) pos += v;}

	void makeUnmovable() {movable = false;}

	void addToNormal(vec3 normal)
	{
		accumulated_normal += normalize(normal);
	}

	vec3& getNormal() { return accumulated_normal;} 

	void resetNormal() {accumulated_normal = vec3(0,0,0);}

};

class Constraint
{
private:
	float rest_distance; 

public:
	Particle *p1, *p2;

	Constraint(Particle *p1, Particle *p2) :  p1(p1),p2(p2)
	{
		vec3 vec = p1->getPos()-p2->getPos();
		rest_distance = length(vec);
	}

	void satisfyConstraint()
	{
		vec3 p1_to_p2 = p2->getPos()-p1->getPos();
		float current_distance = length(p1_to_p2); 
		vec3 correctionVector = p1_to_p2*(1 - rest_distance/current_distance); 
		vec3 correctionVectorHalf = correctionVector*0.5f; //
		p1->offsetPos(correctionVectorHalf); //
		p2->offsetPos(-correctionVectorHalf); //
	}

};

class Cloth
{
private:

	int num_particles_width;
	int num_particles_height; 
	std::vector<Particle> particles; 
	std::vector<Constraint> constraints; 

	Particle* getParticle(int x, int y) {return &particles[y*num_particles_width + x];}

	void makeConstraint(Particle *p1, Particle *p2) {constraints.push_back(Constraint(p1,p2));}

	vec3 calcTriangleNormal(Particle *p1,Particle *p2,Particle *p3)
	{
		vec3 pos1 = p1->getPos();
		vec3 pos2 = p2->getPos();
		vec3 pos3 = p3->getPos();

		vec3 v1 = pos2-pos1;
		vec3 v2 = pos3-pos1;

		return cross(v1,v2);
	}

	void addWindForcesForTriangle(Particle *p1,Particle *p2,Particle *p3, const vec3 direction)
	{
		vec3 normal = calcTriangleNormal(p1,p2,p3);
		vec3 d = normalize(normal);
		vec3 force = normal*(dot(d,direction));
		p1->addForce(force);
		p2->addForce(force);
		p3->addForce(force);
	}

	void drawTriangle(Particle *p1, Particle *p2, Particle *p3, const vec3 color)
	{
		glColor3fv( value_ptr(color) );

		glNormal3fv(value_ptr(normalize(p1->getNormal())));
		glVertex3fv(value_ptr(p1->getPos() ));

		glNormal3fv(value_ptr(normalize(p2->getNormal()) ));
		glVertex3fv(value_ptr(p2->getPos() ));

		glNormal3fv(value_ptr(normalize(p3->getNormal()) ));
		glVertex3fv(value_ptr(p3->getPos() ));
	}

public:
	Cloth(float width, float height, int num_particles_width, int num_particles_height) : num_particles_width(num_particles_width), num_particles_height(num_particles_height)
	{
		particles.resize(num_particles_width*num_particles_height);
		for(int x=0; x<num_particles_width; x++)
		{
			for(int y=0; y<num_particles_height; y++)
			{
				vec3 pos = vec3(width * (x/(float)num_particles_width),
								-height * (y/(float)num_particles_height),
								0);
				particles[y*num_particles_width+x]= Particle(pos);
			}
		}

		for(int x=0; x<num_particles_width; x++)
		{
			for(int y=0; y<num_particles_height; y++)
			{
				if (x<num_particles_width-1) makeConstraint(getParticle(x,y),getParticle(x+1,y));
				if (y<num_particles_height-1) makeConstraint(getParticle(x,y),getParticle(x,y+1));
				if (x<num_particles_width-1 && y<num_particles_height-1) makeConstraint(getParticle(x,y),getParticle(x+1,y+1));
				if (x<num_particles_width-1 && y<num_particles_height-1) makeConstraint(getParticle(x+1,y),getParticle(x,y+1));
			}
		}

		for(int x=0; x<num_particles_width; x++)
		{
			for(int y=0; y<num_particles_height; y++)
			{
				if (x<num_particles_width-2) makeConstraint(getParticle(x,y),getParticle(x+2,y));
				if (y<num_particles_height-2) makeConstraint(getParticle(x,y),getParticle(x,y+2));
				if (x<num_particles_width-2 && y<num_particles_height-2) makeConstraint(getParticle(x,y),getParticle(x+2,y+2));
				if (x<num_particles_width-2 && y<num_particles_height-2) makeConstraint(getParticle(x+2,y),getParticle(x,y+2));			}
		}
	}

	int getPointsWidth(){ return num_particles_width; }
	int getPointsHeight(){ return num_particles_height; }

	void Blockpoints(int index, vec3 offSetPos){
		getParticle(0+index ,0)->offsetPos(offSetPos);
		getParticle(0+index ,0)->makeUnmovable(); 
	}

	/* 
	drawing the cloth as a smooth shaded (and colored according to column) OpenGL triangular mesh
	The cloth is seen as consisting of triangles for four particles in the grid as follows:
	(x,y)   *--* (x+1,y)
	        | /|
	        |/ |
	(x,y+1) *--* (x+1,y+1)
	*/

	void drawShaded()
	{
		std::vector<Particle>::iterator particle;
		for(particle = particles.begin(); particle != particles.end(); particle++)
		{
			(*particle).resetNormal();
		}

		for(int x = 0; x<num_particles_width-1; x++)
		{
			for(int y=0; y<num_particles_height-1; y++)
			{
				vec3 normal = calcTriangleNormal(getParticle(x+1,y),getParticle(x,y),getParticle(x,y+1));
				getParticle(x+1,y)->addToNormal(normal);
				getParticle(x,y)->addToNormal(normal);
				getParticle(x,y+1)->addToNormal(normal);

				normal = calcTriangleNormal(getParticle(x+1,y+1),getParticle(x+1,y),getParticle(x,y+1));
				getParticle(x+1,y+1)->addToNormal(normal);
				getParticle(x+1,y)->addToNormal(normal);
				getParticle(x,y+1)->addToNormal(normal);
			}
		}

		glBegin(GL_TRIANGLES);
		for(int x = 0; x<num_particles_width-1; x++)
		{
			for(int y=0; y<num_particles_height-1; y++)
			{
				vec3 color(0,0,0);
				if (x%2) 
					color = vec3(0.2f,0.2f,0.8f);
				else
					color = vec3(1.0f,1.0f,1.0f);
				drawTriangle(getParticle(x+1,y),getParticle(x,y),getParticle(x,y+1),color);
				drawTriangle(getParticle(x+1,y+1),getParticle(x+1,y),getParticle(x,y+1),color);
			}
		}
		glEnd();
	}

	void timeStep()
	{
		std::vector<Constraint>::iterator constraint;
		for(int i=0; i<CONSTRAINT_ITERATIONS; i++) 
		{
			for(constraint = constraints.begin(); constraint != constraints.end(); constraint++ )
			{
				(*constraint).satisfyConstraint(); 
			}
		}

		std::vector<Particle>::iterator particle;
		for(particle = particles.begin(); particle != particles.end(); particle++)
		{
			(*particle).timeStep(); 
		}
	}

	void addForce(const vec3 direction)
	{
		std::vector<Particle>::iterator particle;
		for(particle = particles.begin(); particle != particles.end(); particle++)
		{
			(*particle).addForce(direction); 
		}

	}

	void windForce(const vec3 direction)
	{
		for(int x = 0; x<num_particles_width-1; x++)
		{
			for(int y=0; y<num_particles_height-1; y++)
			{
				addWindForcesForTriangle(getParticle(x+1,y),getParticle(x,y),getParticle(x,y+1),direction);
				addWindForcesForTriangle(getParticle(x+1,y+1),getParticle(x+1,y),getParticle(x,y+1),direction);
			}
		}
	}

	void ballCollision(const vec3 center,const float radius )
	{
		std::vector<Particle>::iterator particle;
		for(particle = particles.begin(); particle != particles.end(); particle++)
		{
			vec3 v = (*particle).getPos()-center;
			float l = length(v);
			if ( length(v) < radius) 
			{
				(*particle).offsetPos(normalize(v)*(radius-l));
			}
		}
	}
};
