#pragma once

#include "Editor\EditorWindow.h"

class CDetailWindow :
    public CEditorWindow
{
	friend class CEditorGUIManager;

protected:
	CDetailWindow();
	virtual ~CDetailWindow();

protected:
	std::vector<class CEditorWidget*>	m_vecSceneComponent;
	std::vector<class CEditorWidget*>	m_vecSpriteComponent;
	std::vector<class CEditorWidget*>	m_vecCameraComponent;
	std::vector<class CEditorWidget*>	m_vecTargetArmComponent;
	CSharedPtr<class CSceneComponent> m_SelectComponent;

public:
	void SetSelectComponent(class CSceneComponent* Component);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	void CreateSceneComponentWidget();
	void CreateSpriteComponentWidget();
	void CreateCameraComponentWidget();
	void CreateTargetArmComponentWidget();
	void ChangeWidget(class CSceneComponent* Component);

private:
	void LoadButtonClick();
};

