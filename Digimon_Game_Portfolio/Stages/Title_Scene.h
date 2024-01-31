#pragma once
#include "Stages//Stage.h"

class Title_Scene : public Stage
{
public:
	Title_Scene();
	~Title_Scene();
	void Update()override ;
	virtual void Init_Stage(shared_ptr<class Player> player);


};