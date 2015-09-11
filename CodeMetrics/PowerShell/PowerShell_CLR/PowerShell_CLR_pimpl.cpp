# include "Stdafx.h"
# include "PowerShell_CLR_pimpl.h"

using namespace std;
using System::String;
using System::Management::Automation::PowerShell;
using ManagedCLR::PowerShell_CLR_pimpl;

unique_ptr<vector<char>> StringToChar(String^ str)	// may throw std::runtime_error("wcstombs_s  failed!\n")
													// https://msdn.microsoft.com/library/vstudio/d1ae6tz5
{
	pin_ptr<const wchar_t> wch(PtrToStringChars(str))
		; // not auto, pin memory so GC cannot move it while native function is called

	size_t convertedChars = 0;
	const auto sizeInBytes(static_cast<size_t>((str->Length + 1) * 2));

	auto result(make_unique<vector<char>>(sizeInBytes));
	if (wcstombs_s(&convertedChars, result->data(), sizeInBytes, wch, sizeInBytes))
		throw runtime_error("wcstombs_s  failed!\n");
	return result;
}

unique_ptr<string> PowerShell_CLR_pimpl::StringConvert(String^ str) const
{
	auto ch(StringToChar(str));
	return make_unique<string>(ch->data(), ch->capacity());
}

unique_ptr<vector<string>> PowerShell_CLR_pimpl::PowerShell_Invoke()
{
	auto result(make_unique<vector<string>>());

	auto instance(PowerShell::Create());
	for (const auto& i : commands_)
		instance->AddScript(gcnew String(i.c_str()));
	for each (auto var in instance->Invoke())
		result->push_back(*StringConvert(var->ToString()));

	commands_.clear();
	return result;
}