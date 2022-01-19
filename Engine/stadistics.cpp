#include "stadistics.h"



stadistics::stadistics()
{
	Deltalife = 3.0f;
	Bosslife = 15.0f;
	BosslifeSPhase = 15.0f;
	Mylife = 10.0f;
	MylifeLimit = Mylife;
	Torretlife = 4.0f;
}

stadistics::stadistics(const stadistics& other)
{
}


stadistics::~stadistics()
{
}

void stadistics::SetDeltalife(float life) {
	Deltalife = life;
	return;
}

void stadistics::SetDeltalifeMinus() {
	Deltalife -= 1.0f;
}

float stadistics::GetDeltalife() {
	return Deltalife;
}

//Mylife
void stadistics::SetMylife(float life) {
	Mylife = life;
	return;
}

void stadistics::SetMylifeMinus() {
	Mylife -= 1.0f;
}

void stadistics::SetMylifePlus() {
	Mylife += 1.0f;
}

float stadistics::GetMylife() {
	return Mylife;
}

//Boss
void stadistics::SetBosslife(float life) {
	Bosslife = life;
	return;
}

void stadistics::SetBosslifeMinus() {
	Bosslife -= 1.0f;
}

float stadistics::GetBosslife() {
	return Bosslife;
}

//Boss Second Phase
void stadistics::SetBosslifeSPhase(float life) {
	BosslifeSPhase = life;
	return;
}

void stadistics::SetBosslifeMinusSPhase() {
	BosslifeSPhase -= 1.0f;
}

float stadistics::GetBosslifeSPhase() {
	return BosslifeSPhase;
}

//Torret
void stadistics::SetTorretlife(float life) {
	Torretlife = life;
	return;
}

void stadistics::SetTorretlifeMinus() {
	Torretlife -= 1.0f;
}

float stadistics::GetTorretlife() {
	return Torretlife;
}

//Manage Status Life Bar
float stadistics::GetStatusLifeBarRender(float life) {
	life = life / MylifeLimit;
	if (life <= 0) {
		life = 0.0f;
	}
	if (life > 1) {
		life = 1.0f;
	}
	return life;
}

void stadistics::resetStructureStadistics() {
	Deltalife = 3.0f;
	Bosslife = 15.0f;
	BosslifeSPhase = 15.0f;
	Mylife = 10.0f;
	MylifeLimit = Mylife;
	Torretlife = 4.0f;
}
