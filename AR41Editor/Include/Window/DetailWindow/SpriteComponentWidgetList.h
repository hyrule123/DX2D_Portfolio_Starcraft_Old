#pragma once
#include "ComponentWidgetList.h"
class CSpriteComponentWidgetList :
    public CComponentWidgetList
{
	friend class CEditorWindow;

protected:
	CSpriteComponentWidgetList();
	virtual ~CSpriteComponentWidgetList();

private:
	class CEditorInput* m_PlayTime;
	class CEditorInput* m_PlayScale;
	class CEditorInput* m_AnimationClass;
	class CEditorListBox* m_AnimationList;

public:
	void SetSpriteContent(class CSpriteComponent* Sprite);

public:
	virtual bool Init();

private:
	void SelectTextureButtonClick();
	void CreateAnimationButtonClick();
	void AnimationSelectCallback(int Index, const std::string& Item);
};

