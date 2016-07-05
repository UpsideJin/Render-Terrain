/*
Scene.cpp

Author:			Chris Serson
Last Edited:	July 2, 2016

Description:	Class for creating, managing, and rendering a scene.
*/
#include "Scene.h"

Scene::Scene(int height, int width, Graphics* GFX) : T(GFX), C(height, width) {
	mpGFX = GFX;

	// create a viewport and scissor rectangle.
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mViewport.Width = (float)width;
	mViewport.Height = (float)height;
	mViewport.MinDepth = 0;
	mViewport.MaxDepth = 1;

	mScissorRect.left = 0;
	mScissorRect.top = 0;
	mScissorRect.right = width;
	mScissorRect.bottom = height;

	// after creating and initializing the heightmap for the terrain, we need to close the command list
	// and tell the Graphics object to execute the command list to actually finish the subresource init.
	CloseCommandLists();
	mpGFX->LoadAssets();

	T.ClearUnusedUploadBuffersAfterInit();
}

Scene::~Scene() {
	mpGFX->ClearAllFrames();
	mpGFX = nullptr;
}

// Close all command lists. Currently there is only the one.
void Scene::CloseCommandLists() {
	// close the command list.
	if (FAILED(mpGFX->GetCommandList()->Close())) {
		throw GFX_Exception("CommandList Close failed.");
	}
}

void Scene::SetViewport() {
	ID3D12GraphicsCommandList* cmdList = mpGFX->GetCommandList();
	cmdList->RSSetViewports(1, &mViewport);
	cmdList->RSSetScissorRects(1, &mScissorRect);
}

void Scene::Draw() {
	mpGFX->ResetPipeline();

	// set a clear color.
	const float clearColor[] = { 0.2f, 0.6f, 1.0f, 1.0f };
	mpGFX->SetBackBufferRender(mpGFX->GetCommandList(), clearColor);

	SetViewport();

	if (mDraw2D) {
		T.Draw2D(mpGFX->GetCommandList());
	} else {
		T.Draw3D(mpGFX->GetCommandList(), C.GetViewProjectionMatrixTransposed(), C.GetEyePosition());
	}

	mpGFX->SetBackBufferPresent(mpGFX->GetCommandList());
	CloseCommandLists();
	mpGFX->Render();
}

// function allowing the main program to pass keyboard input to the scene.
void Scene::HandleKeyboardInput(UINT key) {
	switch (key) {
		case _W:
			if (!mDraw2D) C.Translate(XMFLOAT3(MOVE_STEP, 0.0f, 0.0f));
			break;
		case _S:
			if (!mDraw2D) C.Translate(XMFLOAT3(-MOVE_STEP, 0.0f, 0.0f));
			break;
		case _A:
			if (!mDraw2D) C.Translate(XMFLOAT3(0.0f, MOVE_STEP, 0.0f));
			break;
		case _D:
			if (!mDraw2D) C.Translate(XMFLOAT3(0.0f, -MOVE_STEP, 0.0f));
			break;
		case _Q:
			if (!mDraw2D) C.Translate(XMFLOAT3(0.0f, 0.0f, MOVE_STEP));
			break;
		case _Z:
			if (!mDraw2D) C.Translate(XMFLOAT3(0.0f, 0.0f, -MOVE_STEP));
			break;
		case _T: // toggle 2D/3D
			mDraw2D = !mDraw2D;
			break;
	}
}

// function allowing the main program to pass mouse input to the scene.
void Scene::HandleMouseInput(int x, int y) {
	if (!mDraw2D) {
		C.Pitch(ROT_ANGLE * y);
		C.Yaw(-ROT_ANGLE * x);
	}
}