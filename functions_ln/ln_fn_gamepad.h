#pragma once

DEFINE_COMMAND_PLUGIN(GetController, 0, 0, NULL);
DEFINE_CMD_COND_PLUGIN(IsButtonPressed, 0, 1, kParams_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(GetPressedButtons, GetButtons, 0, 0, NULL);
DEFINE_CMD_ALT_COND_PLUGIN(GetLeftStickX, GetLSX, 0, 0, NULL);
DEFINE_CMD_ALT_COND_PLUGIN(GetLeftStickY, GetLSY, 0, 0, NULL);
DEFINE_CMD_ALT_COND_PLUGIN(GetRightStickX, GetRSX, 0, 0, NULL);
DEFINE_CMD_ALT_COND_PLUGIN(GetRightStickY, GetRSY, 0, 0, NULL);
DEFINE_CMD_ALT_COND_PLUGIN(GetLeftTrigger, GetLTr, 0, 0, NULL);
DEFINE_CMD_ALT_COND_PLUGIN(GetRightTrigger, GetRTr, 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetDeadZoneLS, 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetDeadZoneRS, 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetDeadZoneLS, 0, 1, kParams_OneDouble);
DEFINE_COMMAND_PLUGIN(SetDeadZoneRS, 0, 1, kParams_OneDouble);
DEFINE_COMMAND_PLUGIN(EnableButton, 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(DisableButton, 0, 1, kParams_OneInt);
DEFINE_CMD_COND_PLUGIN(IsButtonDisabled, 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(HoldButton, 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(ReleaseButton, 0, 1, kParams_OneInt);
DEFINE_CMD_COND_PLUGIN(IsButtonHeld, 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(EnableTrigger, 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(DisableTrigger, 0, 1, kParams_OneInt);
DEFINE_CMD_COND_PLUGIN(IsTriggerDisabled, 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(HoldTrigger, 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(ReleaseTrigger, 0, 1, kParams_OneInt);
DEFINE_CMD_COND_PLUGIN(IsTriggerHeld, 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(TapButton, 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(TapTrigger, 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetOnTriggerDownEventHandler, 0, 3, kParams_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(SetOnTriggerUpEventHandler, 0, 3, kParams_OneForm_TwoInts);
DEFINE_CMD_COND_PLUGIN(IsStickDisabled, 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetStickDisabled, 0, 2, kParams_TwoInts);

bool Cmd_GetController_Execute(COMMAND_ARGS)
{
	*result = s_controllerReady;
	DoConsolePrint(result);
	return true;
}

bool Cmd_IsButtonPressed_Execute(COMMAND_ARGS)
{
	UInt32 button;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &button) && (s_gamePad.wButtons & (UInt16)button))
		*result = 1;
	else *result = 0;
	return true;
}

bool Cmd_IsButtonPressed_Eval(COMMAND_ARGS_EVAL)
{
	*result = (s_gamePad.wButtons & (UInt16)arg1) ? 1 : 0;
	return true;
}

bool Cmd_GetPressedButtons_Execute(COMMAND_ARGS)
{
	*result = s_gamePad.wButtons;
	return true;
}

__declspec(naked) double __vectorcall GetLeftStick(SInt16 value)
{
	__asm
	{
		movsx	eax, cx
		test	eax, eax
		jz		retnZero
		js		isNeg
		sub		eax, s_deadZoneLS
		jg		done
	retnZero:
		pxor	xmm0, xmm0
		retn
	isNeg:
		inc		eax
		add		eax, s_deadZoneLS
		jge		retnZero
	done:
		movd	xmm0, eax
		cvtdq2pd	xmm0, xmm0
		mulsd	xmm0, s_deadZoneLSd
		retn
	}
}

__declspec(naked) double __vectorcall GetRightStick(SInt16 value)
{
	__asm
	{
		movsx	eax, cx
		test	eax, eax
		jz		retnZero
		js		isNeg
		sub		eax, s_deadZoneRS
		jg		done
	retnZero:
		pxor	xmm0, xmm0
		retn
	isNeg:
		inc		eax
		add		eax, s_deadZoneRS
		jge		retnZero
	done:
		movd	xmm0, eax
		cvtdq2pd	xmm0, xmm0
		mulsd	xmm0, s_deadZoneRSd
		retn
	}
}

bool Cmd_GetLeftStickX_Execute(COMMAND_ARGS)
{
	*result = GetLeftStick(s_gamePad.sThumbLX);
	return true;
}

bool Cmd_GetLeftStickX_Eval(COMMAND_ARGS_EVAL)
{
	*result = GetLeftStick(s_gamePad.sThumbLX);
	return true;
}

bool Cmd_GetLeftStickY_Execute(COMMAND_ARGS)
{
	*result = GetLeftStick(s_gamePad.sThumbLY);
	return true;
}

bool Cmd_GetLeftStickY_Eval(COMMAND_ARGS_EVAL)
{
	*result = GetLeftStick(s_gamePad.sThumbLY);
	return true;
}

bool Cmd_GetRightStickX_Execute(COMMAND_ARGS)
{
	*result = GetRightStick(s_gamePad.sThumbRX);
	return true;
}

bool Cmd_GetRightStickX_Eval(COMMAND_ARGS_EVAL)
{
	*result = GetRightStick(s_gamePad.sThumbRX);
	return true;
}

bool Cmd_GetRightStickY_Execute(COMMAND_ARGS)
{
	*result = GetRightStick(s_gamePad.sThumbRY);
	return true;
}

bool Cmd_GetRightStickY_Eval(COMMAND_ARGS_EVAL)
{
	*result = GetRightStick(s_gamePad.sThumbRY);
	return true;
}

bool Cmd_GetLeftTrigger_Execute(COMMAND_ARGS)
{
	*result = s_gamePad.bLeftTrigger * (1 / 255.0);
	return true;
}

bool Cmd_GetLeftTrigger_Eval(COMMAND_ARGS_EVAL)
{
	*result = s_gamePad.bLeftTrigger * (1 / 255.0);
	return true;
}

bool Cmd_GetRightTrigger_Execute(COMMAND_ARGS)
{
	*result = s_gamePad.bRightTrigger * (1 / 255.0);
	return true;
}

bool Cmd_GetRightTrigger_Eval(COMMAND_ARGS_EVAL)
{
	*result = s_gamePad.bRightTrigger * (1 / 255.0);
	return true;
}

bool Cmd_GetDeadZoneLS_Execute(COMMAND_ARGS)
{
	*result = s_deadZoneLSg * (1 / 32767.0);
	DoConsolePrint(result);
	return true;
}

bool Cmd_GetDeadZoneRS_Execute(COMMAND_ARGS)
{
	*result = s_deadZoneRSg * (1 / 32767.0);
	DoConsolePrint(result);
	return true;
}

bool Cmd_SetDeadZoneLS_Execute(COMMAND_ARGS)
{
	double deadzone;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &deadzone))
	{
		int value;
		if (deadzone <= 0.1)
			value = 3277;
		else if (deadzone >= 0.9)
			value = 29490;
		else
			value = deadzone * 32767;
		s_deadZoneLS = value;
		s_deadZoneLSg = value;
		s_deadZoneLSd = 1.0 / (32767.0 - s_deadZoneLSg);
		for (UInt32 patchAddr : {0x717B90, 0x717BA5, 0x717C15, 0x717C2A, 0x755552, 0x755569, 0x7AED31, 0x7E7F82, 0x7ECA56, 0x941157, 0x941177, 0xA23E5E, 0xA23E87})
			SafeWrite32(patchAddr, value);
		value = -value;
		for (UInt32 patchAddr : {0x717BBE, 0x717BD3, 0x717BEA, 0x717BFE, 0xA23E73, 0xA23E9C})
			SafeWrite32(patchAddr, value);
	}
	return true;
}

bool Cmd_SetDeadZoneRS_Execute(COMMAND_ARGS)
{
	double deadzone;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &deadzone))
	{
		int value;
		if (deadzone <= 0.1)
			value = 3277;
		else if (deadzone >= 0.9)
			value = 29490;
		else
			value = deadzone * 32767;
		s_deadZoneRS = value;
		s_deadZoneRSg = value;
		s_deadZoneRSd = 1.0 / (32767.0 - s_deadZoneRSg);
		for (UInt32 patchAddr : {0x7AE57A, 0x7AE598, 0x7AE666, 0x7AE687, 0x7AE699, 0x7AE6AE, 0x7AE6C0, 0x7D57C0, 0x7D57D3, 0x7E7FAD, 0x7ECAAC, 0x7ECB3A, 0x7ECB51, 0x941197, 0x9455F5, 0x945644, 0x9456F0, 0x945710})
			SafeWrite32(patchAddr, value);
		value = -value;
		SafeWrite32(0x7ECAEE, value);
	}
	return true;
}

bool Cmd_EnableButton_Execute(COMMAND_ARGS)
{
	UInt32 button;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &button))
		s_XIStateMods.buttonSkip |= button;
	return true;
}

bool Cmd_DisableButton_Execute(COMMAND_ARGS)
{
	UInt32 button;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &button))
		s_XIStateMods.buttonSkip &= ~button;
	return true;
}

bool Cmd_IsButtonDisabled_Execute(COMMAND_ARGS)
{
	UInt32 button;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &button) && !(s_XIStateMods.buttonSkip & button))
		*result = 1;
	else *result = 0;
	return true;
}

bool Cmd_IsButtonDisabled_Eval(COMMAND_ARGS_EVAL)
{
	*result = !(s_XIStateMods.buttonSkip & (UInt16)arg1);
	return true;
}

bool Cmd_HoldButton_Execute(COMMAND_ARGS)
{
	UInt32 button;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &button))
		s_XIStateMods.buttonHold |= button;
	return true;
}

bool Cmd_ReleaseButton_Execute(COMMAND_ARGS)
{
	UInt32 button;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &button))
		s_XIStateMods.buttonHold &= ~button;
	return true;
}

bool Cmd_IsButtonHeld_Execute(COMMAND_ARGS)
{
	UInt32 button;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &button) && (s_XIStateMods.buttonHold & button))
		*result = 1;
	else *result = 0;
	return true;
}

bool Cmd_IsButtonHeld_Eval(COMMAND_ARGS_EVAL)
{
	*result = (s_XIStateMods.buttonHold & (UInt16)arg1) ? 1 : 0;
	return true;
}

bool Cmd_EnableTrigger_Execute(COMMAND_ARGS)
{
	UInt32 trigger;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &trigger) && (trigger < 2))
		s_XIStateMods.triggerMods &= ~(1 << trigger);
	return true;
}

bool Cmd_DisableTrigger_Execute(COMMAND_ARGS)
{
	UInt32 trigger;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &trigger) && (trigger < 2))
		s_XIStateMods.triggerMods |= (1 << trigger);
	return true;
}

bool Cmd_IsTriggerDisabled_Execute(COMMAND_ARGS)
{
	UInt32 trigger;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &trigger) && (s_XIStateMods.triggerMods & (1 << trigger)))
		*result = 1;
	else *result = 0;
	return true;
}

bool Cmd_IsTriggerDisabled_Eval(COMMAND_ARGS_EVAL)
{
	*result = (s_XIStateMods.triggerMods & (1 << (UInt8)arg1)) ? 1 : 0;
	return true;
}

bool Cmd_HoldTrigger_Execute(COMMAND_ARGS)
{
	UInt32 trigger;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &trigger) && (trigger <= 1))
		s_XIStateMods.triggerMods |= (4 << trigger);
	return true;
}

bool Cmd_ReleaseTrigger_Execute(COMMAND_ARGS)
{
	UInt32 trigger;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &trigger) && (trigger <= 1))
		s_XIStateMods.triggerMods &= ~(4 << trigger);
	return true;
}

bool Cmd_IsTriggerHeld_Execute(COMMAND_ARGS)
{
	UInt32 trigger;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &trigger) && (trigger <= 1) && (s_XIStateMods.triggerMods & (4 << trigger)))
		*result = 1;
	else *result = 0;
	return true;
}

bool Cmd_IsTriggerHeld_Eval(COMMAND_ARGS_EVAL)
{
	*result = (s_XIStateMods.triggerMods & (4 << (UInt32)arg1)) ? 1 : 0;
	return true;
}

bool Cmd_TapButton_Execute(COMMAND_ARGS)
{
	UInt32 button;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &button))
		s_XIStateMods.buttonTap |= button;
	return true;
}

bool Cmd_TapTrigger_Execute(COMMAND_ARGS)
{
	UInt32 trigger;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &trigger) && (trigger <= 1))
		s_XIStateMods.triggerMods |= (0x10 << trigger);
	return true;
}

UInt8 s_onTriggerEventType = 0;

bool SetOnTriggerEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt, rTrigger;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt, &rTrigger) || NOT_ID(script, Script) || !s_controllerReady) return true;
	UInt8 idx = rTrigger ? 2 : 0;
	UInt32 eventMask = s_onTriggerEventType ? kLNEventMask_OnTriggerDown : kLNEventMask_OnTriggerUp;
	if (addEvnt)
	{
		if (s_LNOnTriggerEvents[idx + s_onTriggerEventType].Insert(script))
			s_LNEventFlags |= eventMask;
	}
	else if (s_LNOnTriggerEvents[idx + s_onTriggerEventType].Erase(script))
	{
		if (s_LNOnTriggerEvents[s_onTriggerEventType].Empty() && s_LNOnTriggerEvents[s_onTriggerEventType + 2].Empty())
			s_LNEventFlags &= ~eventMask;
	}
	return true;
}

__declspec(naked) bool Cmd_SetOnTriggerDownEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_onTriggerEventType, 1
		jmp		SetOnTriggerEventHandler_Execute
	}
}

__declspec(naked) bool Cmd_SetOnTriggerUpEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_onTriggerEventType, 0
		jmp		SetOnTriggerEventHandler_Execute
	}
}

bool Cmd_IsStickDisabled_Execute(COMMAND_ARGS)
{
	UInt32 stick;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &stick) && (stick <= 1) && (s_XIStateMods.stickSkip & (0xF << (stick << 2))))
		*result = 1;
	else *result = 0;
	return true;
}

bool Cmd_IsStickDisabled_Eval(COMMAND_ARGS_EVAL)
{
	*result = (s_XIStateMods.stickSkip & (0xF << ((UInt32)arg1 << 2))) ? 1 : 0;
	return true;
}

bool Cmd_SetStickDisabled_Execute(COMMAND_ARGS)
{
	UInt32 stick, doSet;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &stick, &doSet) && (stick <= 1))
	{
		if (doSet) s_XIStateMods.stickSkip |= (0xF << (stick << 2));
		else s_XIStateMods.stickSkip &= ~(0xF << (stick << 2));
	}
	return true;
}