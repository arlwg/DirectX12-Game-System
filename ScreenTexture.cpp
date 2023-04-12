#include "ScreenTexture.h"
#include "Game.hpp"

ScrenTexture::ScrenTexture(Type type, Game* game) : Entity(game)
, mType(type)
{
	switch (type)
	{
	case (Title):
		mSprite = "Title";
		break;
	case(Menu):
		mSprite = "Menu";
		break;
	case (Pause):
		mSprite = "Pause";
		break;
	default:
		mSprite = "Title";
		break;
	}
}

unsigned int ScrenTexture::getCategory() const
{
	return 1234;
}

void ScrenTexture::drawCurrent() const
{
	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	UINT matCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));

	auto objectCB = game->mCurrFrameResource->ObjectCB->Resource();
	auto matCB = game->mCurrFrameResource->MaterialCB->Resource();

	if (mScreenRitem != nullptr)
	{
		game->getCmdList()->IASetVertexBuffers(0, 1, &mScreenRitem->Geo->VertexBufferView());
		game->getCmdList()->IASetIndexBuffer(&mScreenRitem->Geo->IndexBufferView());
		game->getCmdList()->IASetPrimitiveTopology(mScreenRitem->PrimitiveType);

		CD3DX12_GPU_DESCRIPTOR_HANDLE tex(game->mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
		tex.Offset(mScreenRitem->Mat->DiffuseSrvHeapIndex, game->mCbvSrvDescriptorSize);

		D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + (UINT64)mScreenRitem->ObjCBIndex * objCBByteSize;
		D3D12_GPU_VIRTUAL_ADDRESS matCBAddress = matCB->GetGPUVirtualAddress() + (UINT64)mScreenRitem->Mat->MatCBIndex * matCBByteSize;

		game->getCmdList()->SetGraphicsRootDescriptorTable(0, tex);
		game->getCmdList()->SetGraphicsRootConstantBufferView(1, objCBAddress);
		game->getCmdList()->SetGraphicsRootConstantBufferView(3, matCBAddress);

		game->getCmdList()->DrawIndexedInstanced(mScreenRitem->IndexCount, 1, mScreenRitem->StartIndexLocation, mScreenRitem->BaseVertexLocation, 0);
	}

}

void ScrenTexture::buildCurrent()
{
	auto render = std::make_unique<RenderItem>();
	renderer = render.get();
	mScreenRitem = render.get();
	renderer->World = getTransform();
	renderer->ObjCBIndex = game->getRenderItems().size();
	renderer->Mat = game->getMaterials()[mSprite].get();
	renderer->Geo = game->getGeometries()["boxGeo"].get();
	renderer->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	renderer->IndexCount = renderer->Geo->DrawArgs["box"].IndexCount;
	renderer->StartIndexLocation = renderer->Geo->DrawArgs["box"].StartIndexLocation;
	renderer->BaseVertexLocation = renderer->Geo->DrawArgs["box"].BaseVertexLocation;
	game->getRenderItems().push_back(std::move(render));
}
