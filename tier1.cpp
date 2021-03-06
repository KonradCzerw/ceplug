#include "tier1.hpp"

#include "offsets.hpp"

bool Tier1::Init() {
	this->g_pCVar = Interface::Create(this->Name(), "VEngineCvar007", false);
	if(this->g_pCVar) {
		this->RegisterConCommand = this->g_pCVar->Original<_RegisterConCommand>(Offsets::RegisterConCommand);
		this->UnregisterConCommand = this->g_pCVar->Original<_UnregisterConCommand>(Offsets::UnregisterConCommand);
		this->FindCommandBase = this->g_pCVar->Original<_FindCommandBase>(Offsets::FindCommandBase);

		this->m_pConCommandList = reinterpret_cast<ConCommandBase*>((uintptr_t)this->g_pCVar->ThisPtr() + Offsets::m_pConCommandList);

		auto listdemo = reinterpret_cast<ConCommand*>(this->FindCommandBase(this->g_pCVar->ThisPtr(), "listdemo"));
		if(listdemo) {
			this->ConCommand_VTable = listdemo->ConCommandBase_VTable;
		}

		auto sv_lan = reinterpret_cast<ConVar*>(this->FindCommandBase(this->g_pCVar->ThisPtr(), "sv_lan"));
		if(sv_lan) {
			this->ConVar_VTable = sv_lan->ConCommandBase_VTable;
			this->ConVar_VTable2 = sv_lan->ConVar_VTable;

			auto vtable = &ConVar_VTable;
			this->Dtor = Memory::VMT<_Dtor>(vtable, Offsets::Dtor);
			this->Create = Memory::VMT<_Create>(vtable, Offsets::Create);
		}

		this->InstallGlobalChangeCallback = this->g_pCVar->Original<_InstallGlobalChangeCallback>(Offsets::InstallGlobalChangeCallback);
		this->RemoveGlobalChangeCallback = this->g_pCVar->Original<_RemoveGlobalChangeCallback>(Offsets::RemoveGlobalChangeCallback);
	}

	// return this->g_pCVar && this->ConCommand_VTable && this->ConVar_VTable && this->ConVar_VTable2 && this->AutoCompletionFunc;
	return this->g_pCVar && this->ConCommand_VTable;
}

void Tier1::Shutdown() {
	Interface::Delete(g_pCVar);
}

Tier1* tier1;
