#include "Player.hpp"
#include "World.hpp"
#include "StateStack.h"

class Game : public D3DApp
{
public:
	Game(HINSTANCE hInstance);
	Game(const Game& rhs) = delete;
	Game& operator=(const Game& rhs) = delete;
	~Game();

	virtual bool Initialize()override;
	void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems);
public:
	virtual void OnResize()override;

	virtual void Update(const GameTimer& gt)override;
	virtual void Draw(const GameTimer& gt)override;

	virtual void OnMouseDown(WPARAM btnState, int x, int y)override;
	virtual void OnMouseUp(WPARAM btnState, int x, int y)override;
	virtual void OnMouseMove(WPARAM btnState, int x, int y)override;
	virtual void OnKeyPressed(WPARAM btnState)override;

	void OnKeyboardInput(const GameTimer& gt);
	void UpdateCamera(const GameTimer& gt);
	void AnimateMaterials(const GameTimer& gt);
	void UpdateObjectCBs(const GameTimer& gt);
	void UpdateMaterialCBs(const GameTimer& gt);
	void UpdateMainPassCB(const GameTimer& gt);

	//step5
	void LoadTextures();

	void BuildRootSignature();

	//step9
	void BuildDescriptorHeaps();

	void BuildShadersAndInputLayout();
	void BuildShapeGeometry();
	void BuildPSOs();
	void BuildFrameResources();
	void BuildMaterials();
	void BuildRenderItems();
	void registerStates();
	void registerMaterial(std::string Name, int index, float Roughness);
	void registerMaterial(std::string Name, int index);
	void registerTexture(std::string Name, std::string fileName);

	void FlushCommandList() { FlushCommandQueue(); }


	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();

public:

	

	

	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

	

	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;
	std::unordered_map<std::string, std::unique_ptr<Material>> mMaterials;

	//step7
	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures;

	std::unordered_map<std::string, ComPtr<ID3DBlob>> mShaders;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	ComPtr<ID3D12PipelineState> mOpaquePSO = nullptr;

	// List of all the render items.
	std::vector<std::unique_ptr<RenderItem>> mAllRitems;



	PassConstants mMainPassCB;

	//XMFLOAT3 mEyePos = { 0.0f, 0.0f, -10.0f };
	//XMFLOAT4X4 mView = MathHelper::Identity4x4();
	//XMFLOAT4X4 mProj = MathHelper::Identity4x4();

	//float mTheta = 1.3f * XM_PI;
	//float mPhi = 0.4f * XM_PI;
	//float mRadius = 2.5f;

	POINT mLastMousePos;
	Camera mCamera;
	World mWorld;
	Player mPlayer;
	StateStack	mStateStack;
	
public:


	int materialIndex = 0;
	UINT mCbvSrvDescriptorSize = 0;
	ComPtr<ID3D12DescriptorHeap> mSrvDescriptorHeap = nullptr;
	std::vector<std::unique_ptr<FrameResource>> mFrameResources;
	FrameResource* mCurrFrameResource = nullptr;
	int mCurrFrameResourceIndex = 0;
	ID3D12GraphicsCommandList*  getCmdList() { return mCommandList.Get(); }
	std::vector<std::unique_ptr<RenderItem>>& getRenderItems() { return mAllRitems; }
	std::unordered_map<std::string, std::unique_ptr<Material>>& getMaterials() { return mMaterials; }
	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>>& getGeometries() { return mGeometries; }
	// Render items divided by PSO.
	std::vector<RenderItem*> mOpaqueRitems;

	public:
		bool ReloadGameState = false;
		XMFLOAT3 playerPos = XMFLOAT3(0,0,0);
		XMFLOAT3 enemy1Pos = XMFLOAT3(0, 0, 0);
		XMFLOAT3 enemy2Pos = XMFLOAT3(0, 0, 0);
		XMFLOAT3 bgPos = XMFLOAT3(0, 0, 0);
};
