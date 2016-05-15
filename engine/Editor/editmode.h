#pragma once

enum class EditorMode
{
	EDIT,
	PLAY
};

class EditMode
{
public:
	void setEditMode(EditorMode mode) { m_editorMode = mode; }
	EditorMode getEditMode() const { return m_editorMode; }

private:
	EditorMode m_editorMode;
};