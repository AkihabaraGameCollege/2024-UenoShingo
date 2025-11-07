#include "TestInput.h"
#include <format>

using namespace GameLibrary;

TestInput::TestInput(const ProjectSettings& settings)
	: Game(settings)
{

}

void TestInput::OnUpdate() noexcept
{
	// Keyboard
	auto OutputButton = [](DigitalInput digitalKey) {
		if (Input::GetButtonDown(digitalKey)) {
			const auto message = std::format(L"DigitalInput::({0:04x})::Down\n", static_cast<unsigned char>(digitalKey));
			OutputDebugStringW(message.c_str());
		}
		//else if (Input::GetButton(digitalKey)) {
		//	const auto message = std::format(L"DigitalInput::({0:04x})::Stay\n", static_cast<unsigned char>(digitalKey));
		//	OutputDebugStringW(message.c_str());
		//}
		else if (Input::GetButtonUp(digitalKey)) {
			const auto message = std::format(L"DigitalInput::({0:04x})::Up\n", static_cast<unsigned char>(digitalKey));
			OutputDebugStringW(message.c_str());
		}};

	OutputButton(DigitalInput::Backspace);
	OutputButton(DigitalInput::Tab);
	OutputButton(DigitalInput::Enter);
	OutputButton(DigitalInput::Pause);
	OutputButton(DigitalInput::CapsLock);
	OutputButton(DigitalInput::Escape);
	OutputButton(DigitalInput::Space);
	OutputButton(DigitalInput::PageUp);
	OutputButton(DigitalInput::PageDown);
	OutputButton(DigitalInput::End);
	OutputButton(DigitalInput::Home);
	OutputButton(DigitalInput::LeftArrow);
	OutputButton(DigitalInput::UpArrow);
	OutputButton(DigitalInput::RightArrow);
	OutputButton(DigitalInput::DownArrow);
	OutputButton(DigitalInput::Sysrq);
	OutputButton(DigitalInput::Insert);
	OutputButton(DigitalInput::Delete);
	OutputButton(DigitalInput::Digit0);
	OutputButton(DigitalInput::Digit1);
	OutputButton(DigitalInput::Digit2);
	OutputButton(DigitalInput::Digit3);
	OutputButton(DigitalInput::Digit4);
	OutputButton(DigitalInput::Digit5);
	OutputButton(DigitalInput::Digit6);
	OutputButton(DigitalInput::Digit7);
	OutputButton(DigitalInput::Digit8);
	OutputButton(DigitalInput::Digit9);
	OutputButton(DigitalInput::A);
	OutputButton(DigitalInput::B);
	OutputButton(DigitalInput::C);
	OutputButton(DigitalInput::D);
	OutputButton(DigitalInput::E);
	OutputButton(DigitalInput::F);
	OutputButton(DigitalInput::G);
	OutputButton(DigitalInput::H);
	OutputButton(DigitalInput::I);
	OutputButton(DigitalInput::J);
	OutputButton(DigitalInput::K);
	OutputButton(DigitalInput::L);
	OutputButton(DigitalInput::M);
	OutputButton(DigitalInput::N);
	OutputButton(DigitalInput::O);
	OutputButton(DigitalInput::P);
	OutputButton(DigitalInput::Q);
	OutputButton(DigitalInput::R);
	OutputButton(DigitalInput::S);
	OutputButton(DigitalInput::T);
	OutputButton(DigitalInput::U);
	OutputButton(DigitalInput::V);
	OutputButton(DigitalInput::W);
	OutputButton(DigitalInput::X);
	OutputButton(DigitalInput::Y);
	OutputButton(DigitalInput::Z);
	OutputButton(DigitalInput::LeftWindows);
	OutputButton(DigitalInput::RightWindows);
	OutputButton(DigitalInput::Apps);
	OutputButton(DigitalInput::Numpad0);
	OutputButton(DigitalInput::Numpad1);
	OutputButton(DigitalInput::Numpad2);
	OutputButton(DigitalInput::Numpad3);
	OutputButton(DigitalInput::Numpad4);
	OutputButton(DigitalInput::Numpad5);
	OutputButton(DigitalInput::Numpad6);
	OutputButton(DigitalInput::Numpad7);
	OutputButton(DigitalInput::Numpad8);
	OutputButton(DigitalInput::Numpad9);
	OutputButton(DigitalInput::NumpadMultiply);
	OutputButton(DigitalInput::NumpadPlus);
	OutputButton(DigitalInput::NumpadMinus);
	OutputButton(DigitalInput::NumpadPeriod);
	OutputButton(DigitalInput::NumpadSlash);
	OutputButton(DigitalInput::F1);
	OutputButton(DigitalInput::F2);
	OutputButton(DigitalInput::F3);
	OutputButton(DigitalInput::F4);
	OutputButton(DigitalInput::F5);
	OutputButton(DigitalInput::F6);
	OutputButton(DigitalInput::F7);
	OutputButton(DigitalInput::F8);
	OutputButton(DigitalInput::F9);
	OutputButton(DigitalInput::F10);
	OutputButton(DigitalInput::F11);
	OutputButton(DigitalInput::F12);
	OutputButton(DigitalInput::NumLock);
	OutputButton(DigitalInput::Scroll);
	OutputButton(DigitalInput::LeftShift);
	OutputButton(DigitalInput::RightShift);
	OutputButton(DigitalInput::LeftCtrl);
	OutputButton(DigitalInput::RightCtrl);
	OutputButton(DigitalInput::LeftAlt);
	OutputButton(DigitalInput::RightAlt);
	OutputButton(DigitalInput::Semicolon);
	OutputButton(DigitalInput::Equals);
	OutputButton(DigitalInput::Comma);
	OutputButton(DigitalInput::Minus);
	OutputButton(DigitalInput::Period);
	OutputButton(DigitalInput::Slash);
	OutputButton(DigitalInput::Grave);
	OutputButton(DigitalInput::LeftBracket);
	OutputButton(DigitalInput::Backslash);
	OutputButton(DigitalInput::RightBracket);
	OutputButton(DigitalInput::Apostrophe);

	OutputButton(DigitalInput::NumpadEnter);
}
