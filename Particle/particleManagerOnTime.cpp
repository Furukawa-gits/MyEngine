#include"particleManagerOnTime.h"

std::list<particleSet> particleManagerOnTime::particles = {};

particleManagerOnTime::particleManagerOnTime()
{
}

particleManagerOnTime::~particleManagerOnTime()
{
}

void particleManagerOnTime::addParticle(SingleParticle particle, std::string texname)
{
	particleSet set;
	set.particle = particle;
	set.texName = texname;
	particles.push_back(set);
}

void particleManagerOnTime::particlesUpdata()
{
	particles.remove_if([](particleSet& newset)
		{
			return newset.particle.getIsActive() == false;
		});

	for (particleSet& newparticle : particles)
	{
		newparticle.particle.updata();
	}
}

void particleManagerOnTime::particlesDrawTex()
{
	for (particleSet& newparticle : particles)
	{
		newparticle.particle.drawSpecifyTex(newparticle.texName);
	}
}