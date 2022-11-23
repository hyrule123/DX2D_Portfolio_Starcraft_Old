#pragma once

#include "Editor\EditorWindow.h"
#include <stdint.h>

class CResourceWindow :
    public CEditorWindow
{
	friend class CEditorGUIManager;

protected:
	CResourceWindow();
	virtual ~CResourceWindow();
	

private:
	class CEditorListBox* m_TextureList;
	class CEditorListBox* m_SoundList;
	class CEditorListBox* m_FontList;

	class CEditorImage* m_TextureImageBox;
	CSharedPtr<class CTexture>	m_SelectTexture;

public:
	inline class CTexture* GetSelectTexture()	const;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	void LoadTextureList();
	void TextureClickCallback(int Index, const std::string& Item);
};

inline class CTexture* CResourceWindow::GetSelectTexture()	const
{
	return m_SelectTexture;
}