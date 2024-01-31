#include "stdafx.h"
#include "Device.h"

#include "Actors/Player.h"
#include "Stages/Stage.h"
#include "Digimon.h"
#include "Bullet.h"
#include "Scene_Manager.h"
#include "CastleGate.h"
#include "Decal.h"
#include "CutManager.h"
#include "UI/Bottom_UI.h"

shared_ptr<Player> player;
D3DXMATRIX V, P;
D3DXVECTOR3 Edit_Rot = { 0,0,0 };

float Edit_Speed;
float Sprite_Speed = 9.5f;

bool State_Clear = false;
bool bDrawBound = false;
bool bStop = false; //Skill Stop

vector<D3DXVECTOR3> MainMap_Speed
{
	{-13.f,0.f,0.f},
	{-70.f,0.f,0.f},
	{-0.029,0,0},
	{-0.079,0,0},
	{-0.010,0,0},
}; 
vector<D3DXVECTOR3> MainMap_GSpeed
{
	{-115.f,0.f,0.f},
	{-1,0,0},
	{-1,0,0},
	{-0.029,0,0},
	{-0.079,0,0},
	{-0.010,0,0},
};

Digimon* modack;
Scene_Manager* stage;

CutScean* Skill_Cut;
class Bottom_UI* bottom_UI;

void InitScene()
{
	player = make_shared<Player>();
	Scene_Manager::Player = player;
	stage = new Scene_Manager();

	//stage = new Stage();
	ViewProjection_Init();

	stage->ViewProjection(V, P);

	Bullet_Manager::Create(BulletPool);
	Effect_Manager::Create(Poolsize);
	CutManager::Create();

	Bullet_Manager::ViewProjection(V, P);
	Effect_Manager::ViewProjection(V, P);
	CutManager::ViewProjection(V, P);
	Skill_Cut = new CutScean();
	Skill_Cut->ViewProjection(V, P); // CutScene BG
	
	/*modack = new Digimon(L"_Textures/Objects/모닥불_0.png", 256, 256, 16, 성장기);
	modack->ViewProjection(V, P);
	modack->Position({20,0,0});
	modack->Scale({512,512,1});*/
	bottom_UI = new Bottom_UI(Layer_Folder + L"Bottom_UI.png");
	bottom_UI->ViewProjection(V, P);
}

void DestroyScene()
{
	//SAFE_DELETE(modack);
	SAFE_DELETE(Skill_Cut);
}

bool chk = false;
void Update()
{
	if (!bStop)
	{
		stage->Update();
		stage->ViewProjection(V, P);

		//modack->Update();
		Bullet_Manager::Update();
		Effect_Manager::Update();
	}
	Skill_Cut->Update();
	bottom_UI->Update();


}
float aspect = float(Width) / Height;


void Render()
{
	D3DXCOLOR bgColor = D3DXCOLOR(0, 0, 0, 0);
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgColor);
	{
		ImGui_Event();
		stage->Render();
		
		//modack->Render();
	}
	Bullet_Manager::Render();
	Effect_Manager::Render();
	bottom_UI->Render();

	Skill_Cut->Render();
	ImGui::Render();
	DirectWrite::GetDC()->BeginDraw();
	{
		wstring text = L"";

		RECT rect; // text 위치
		rect.left = 0;
		rect.top = 0;
		rect.right = 600;
		rect.bottom = 20;

		text = L"Frame Per Second : " + to_wstring((int)ImGui::GetIO().Framerate);
		DirectWrite::RenderText(text, rect);

	}

	DirectWrite::GetDC()->EndDraw();

	SwapChain->Present(0, 0);

}

extern D3DXVECTOR3 Castle_Pos ;
extern float Castle_interver;

void ImGui_Event()
{
	/*ImGui::SliderFloat2("Edit_Pos", Edit_Pos, -Width, Width);
	ImGui::SliderFloat2("Edit_Scale", Edit_Scale, -Width, Width);
	ImGui::SliderFloat3("Edit_Rot", Edit_Rot, -180, 180);
	ImGui::SliderFloat("Edit_Speed", &Edit_Speed, 1, 180);
	
	ImGui::SliderFloat("Castle_interver", &Castle_interver,0.f,500.f);
	*/
	
	if (ImGui::Button("Standby_Stage"))
	{
		Scene_Manager::StandBy_Stage(1);
	}
	if (ImGui::Button("Battle_Start"))
	{
		Scene_Manager::Start_Stage();
	}
	if (ImGui::Button("State_Clear"))
	{
		//stage->Stage_Clear();
	}
	ImGui::Checkbox("DrawBoundBox", &bDrawBound);
	
	/*for (int i = 0; i < LayerSize; i++) 
	{
		string name = "map_speed" + to_string(i);
		ImGui::SliderFloat(name.c_str(), &Speed[i].x, -1, 0);
	}*/
	
}

void ViewProjection_Init()
{
	//View
	D3DXVECTOR3 eye(0, 0, -1);
	D3DXVECTOR3 at(0, 0, 0);
	D3DXVECTOR3 up(0, 1, 0);
	D3DXMatrixLookAtLH(&V, &eye, &at, &up);

	//Projection
	//D3DXMatrixOrthoOffCenterLH(&P, 0, (float)Width, 0, (float)Height, -1, 1);
	D3DXMatrixOrthoOffCenterLH(&P, -(float)Width / 2, (float)Width / 2, -(float)Height / 2, (float)Height / 2, -1, 1);

}