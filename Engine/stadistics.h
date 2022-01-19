////////////////////////////////////////////////////////////////////////////////
// Filename: positionclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _STADISTICSCLASS_H_
#define _STADISTICSCLASS_H_

class stadistics
{
public:
	stadistics();
	stadistics(const stadistics&);
	~stadistics();
	//Delta Ship 
	void SetDeltalife(float);
	void SetDeltalifeMinus();
	float GetDeltalife();
	
	//MySpaceship
	void SetMylife(float);
	void SetMylifeMinus();
	float GetMylife();
	void SetMylifePlus();

	//Boss
	void SetBosslife(float);
	void SetBosslifeMinus();
	float GetBosslife();

	//Boss Second Phase
	void SetBosslifeSPhase(float);
	void SetBosslifeMinusSPhase();
	float GetBosslifeSPhase();

	//Torret
	void SetTorretlife(float life);
	void SetTorretlifeMinus();
	float GetTorretlife();

	//Status HUD
	float GetStatusLifeBarRender(float life);

	void resetStructureStadistics();

private:
	float Deltalife, Bosslife, Mylife, Torretlife, BosslifeSPhase, MylifeLimit;

};

#endif

