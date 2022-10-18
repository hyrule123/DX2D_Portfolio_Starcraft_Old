#pragma once

#include "Editor\EditorWindow.h"

class CAnimation2DWindow :
    public CEditorWindow
{
	friend class CEditorGUIManager;

protected:
	CAnimation2DWindow();
	virtual ~CAnimation2DWindow();

private:
	class CEditorListBox* m_Animation2DClassList;
	CSharedPtr<class CTexture>  m_SelectTexture;


public:

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	void LoadAnimation2DClassList();
	void CreateAnimation2D();
	void Animation2DSelectCallback(int Index, const std::string& Item);
};

