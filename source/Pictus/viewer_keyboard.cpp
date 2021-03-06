#include "viewer_keyboard.h"
#include "viewer.h"
#include "registry.h"

namespace App {
	Geom::SizeInt ActiveImageSize(Viewer* owner) {
		auto img = owner->ActiveImage();
		if (img)
		{
			return img->GetSize();
		}
		return{ 0, 0 };
	}
	void ViewerKeyboard::Construct(Viewer* owner) {
		m_owner = owner;

		m_owner->Bind(wxEVT_KEY_DOWN, [&](wxKeyEvent& wxe) {
			auto e = Win::KeyEvent(wxe);
			for (auto x : m_cfg.Bindings) {
				if (x.Key.Key == e.Key && x.Key.Alt == e.AltPressed && x.Key.Shift == e.ShiftPressed && x.Key.Ctrl == e.CtrlPressed) {
					switch (x.Action) {
					case KeyAction::FirstImage:
						m_owner->ToStart();
						break;
					case KeyAction::LastImage:
						m_owner->ToEnd();
						break;;
					case KeyAction::NextImage:
						m_owner->ImageNext();
						break;
					case KeyAction::NextSkipImage:
						m_owner->ImageNext(Viewer::ShiftSkipLength);
						break;
					case KeyAction::PreviousImage:
						m_owner->ImagePrev();
						break;
					case KeyAction::PreviousSkipImage:
						m_owner->ImagePrev(Viewer::ShiftSkipLength);
						break;
					case KeyAction::PanDown:
						m_owner->PanVertical(Viewer::KeyPanSize);
						break;
					case KeyAction::PanUp:
						m_owner->PanVertical(-Viewer::KeyPanSize);
						break;
					case KeyAction::PanLeft:
						m_owner->PanHorizontal(-Viewer::KeyPanSize);
						break;
					case KeyAction::PanRight:
						m_owner->PanHorizontal(Viewer::KeyPanSize);
						break;
					case KeyAction::RandomImage:
						m_owner->ImageRandom();
						break;
					case KeyAction::RecycleCurrentFile:
						m_owner->RemoveImage(Viewer::RemoveRecycle);
						break;
					case KeyAction::DeleteCurrentFile:
						m_owner->RemoveImage(Viewer::RemoveDelete);
						break;
					case KeyAction::RemoveCurrentImage:
						m_owner->RemoveImage(Viewer::RemoveOnly);
						break;
					case KeyAction::RenameFile:
						m_owner->RenameCurrent();
						break;
					case KeyAction::ToggleFullscreen:
						m_owner->ToggleFullscreenMode();
						break;
					case KeyAction::ZoomDefault:
						m_owner->ZoomDefault();
						break;
					case KeyAction::ZoomFree:
						m_owner->ZoomMode(ZoomMode::ZoomFree);
						break;
					case KeyAction::ZoomFull:
						m_owner->ZoomMode(ZoomMode::ZoomFullSize);
						break;
					case KeyAction::ZoomIn:
						m_owner->ZoomIn();
						break;
					case KeyAction::ZoomOut:
						m_owner->ZoomOut();
						break;
					case KeyAction::CloseApplication:
						m_owner->Close();
						break;
					case KeyAction::OpenSettings:
						m_owner->ShowSettings();
						break;
					case KeyAction::CopyImage:
						m_owner->CopyToClipboard();
						break;
					case KeyAction::ToggleFullSizeDefaultZoom:
						m_owner->ZoomToggleFullSizeDefaultZoom();
						break;
					case KeyAction::RotateLeft:
						m_owner->RotateLeft();
						break;
					case KeyAction::RotateRight:
						m_owner->RotateRight();
						break;
					case KeyAction::PanScreenUp:
						m_owner->PanVertical(-m_owner->GetVisibleImageSize().Height);
						break;
					case KeyAction::PanScreenDown:
						m_owner->PanVertical(m_owner->GetVisibleImageSize().Height);
						break;
					case KeyAction::PanScreenLeft:
						m_owner->PanHorizontal(-m_owner->GetVisibleImageSize().Width);
						break;
					case KeyAction::PanScreenRight:
						m_owner->PanHorizontal(m_owner->GetVisibleImageSize().Width);
						break;
					case KeyAction::PanUpperEdge:
						m_owner->PanVertical(-ActiveImageSize(m_owner).Height);
						break;
					case KeyAction::PanLowerEdge:
						m_owner->PanVertical(ActiveImageSize(m_owner).Height);
						break;
					case KeyAction::PanLeftEdge:
						m_owner->PanHorizontal(-ActiveImageSize(m_owner).Width);
						break;
					case KeyAction::PanRightEdge:
						m_owner->PanHorizontal(ActiveImageSize(m_owner).Width);
						break;
					}
					return true;
				}
			}
			return false;
		});
	}

	void ViewerKeyboard::SetBindings(Reg::KeyboardSettings cfg) {
		m_cfg = cfg;
	}
}
