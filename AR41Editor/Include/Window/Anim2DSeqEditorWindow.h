#pragma once
#include "Editor\EditorWindow.h"
class CAnim2DSeqEditorWindow :
    public CEditorWindow
{
	friend class CEditorGUIManager;

protected:
	CAnim2DSeqEditorWindow();
	virtual ~CAnim2DSeqEditorWindow();

private:
	class CEditorListBox* m_Animation2DSequenceList;
	class CEditorImage* m_Sequence2DImage;
	CSharedPtr<class CTexture>  m_SelectTexture;
	class CEditorInput* m_Selected;
	class CEditorInput* m_StartX;
	class CEditorInput* m_StartY;
	class CEditorInput* m_EndX;
	class CEditorInput* m_EndY;
	class CEditorInput* m_Count;
	class CEditorListBox* m_SequenceFrameList;
	std::vector<Animation2DFrameData>	m_vecFrame;

public:

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	void LoadAnimation2DSequenceList();
	void SequenceLoadClick();	//로드 버튼 클릭 시 호출
	void SelectTextureButton();
	void CreateAnimationSequence2D();
	void AddFrameClick();
	void AddAllFrameClick();
	void SequenceSelectCallback(int Index, const std::string& Item);
	void FrameSelectCallback(int Index, const std::string& Item);
	
};

