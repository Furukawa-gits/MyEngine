#pragma once
#include"Particle.h"

struct particleSet
{
	SingleParticle particle;
	std::string texName;
};

class particleManagerOnTime
{
public:
	particleManagerOnTime();
	~particleManagerOnTime();

	static void addParticle(SingleParticle particle, std::string texname);

	static void particlesUpdata();

	static void particlesDrawTex();

public:
	
	static std::list<particleSet> particles;
};