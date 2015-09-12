#include "StdAfx.h"
#include "settings_page.h"
#include "settings.h"

namespace App {
	bool SettingsPage::IsRootPage() const {
		return true;
	}

	bool SettingsPage::PerformOnInitDialog() {
		auto* pSet = dynamic_cast<App::Settings*>(Parent());
		if (pSet == nullptr) {
			DO_THROW(Err::CriticalError, L"Settings page had an invalid parent.");
		}

		return PerformOnInitPage();
	}

	void SettingsPage::UpdateFromSettings(const Reg::Settings& settings) {
		if (IsModelessVisible()) {
			PerformUpdateFromSettings(settings);
		}
	}

	void SettingsPage::WriteSettings(Reg::Settings& settings) {
		onWriteSettings(settings);
	}

	SettingsPage::SettingsPage(int id):
		Win::Dialog{ id }
	{}

	SettingsPage::~SettingsPage() {}
}
