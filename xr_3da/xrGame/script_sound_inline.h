////////////////////////////////////////////////////////////////////////////
//	Module 		: script_sound_inline.h
//	Created 	: 06.02.2004
//  Modified 	: 06.02.2004
//	Author		: Dmitriy Iassenev
//	Description : XRay Script sound class inline functions
////////////////////////////////////////////////////////////////////////////

#pragma once

IC	u32	CScriptSound::Length				()
{
	if (!m_sound._handle())
	{
		Msg(fmt::format("* WARNING: CScriptSound::Length There is no sound {0}", *m_caSoundToPlay).c_str());
		return 0;
	}
	//VERIFY					(m_sound._handle());
	return					(m_sound._handle()->length_ms());
}

IC	void CScriptSound::Play					(CScriptGameObject *object)
{
	Play					(object,0.f,0);
}

IC	void CScriptSound::Play					(CScriptGameObject *object, float delay)
{
	Play					(object,delay,0);
}
		
IC	void CScriptSound::PlayAtPos			(CScriptGameObject *object, const Fvector &position)
{
	PlayAtPos				(object,position,0.f,0);
}

IC	void CScriptSound::PlayAtPos			(CScriptGameObject *object, const Fvector &position, float delay)
{
	PlayAtPos				(object,position,delay,0);
}

IC	void CScriptSound::SetMinDistance		(const float fMinDistance)
{
	//VERIFY				(m_sound._handle());
	if (!m_sound._handle())
	{
		Msg(fmt::format("* WARNING: CScriptSound::SetMinDistance There is no sound {0}", *m_caSoundToPlay).c_str());
		m_sound.set_range(0,0);
		return;
	}
	m_sound.set_range(fMinDistance,GetMaxDistance());
}

IC	void CScriptSound::SetMaxDistance		(const float fMaxDistance)
{
	//VERIFY				(m_sound._handle());
	if (!m_sound._handle())
	{
		Msg(fmt::format("* WARNING: CScriptSound::SetMaxDistance There is no sound {0}", *m_caSoundToPlay).c_str());
		m_sound.set_range(0,0);
		return;
	}
	m_sound.set_range(GetMinDistance(),fMaxDistance);
}

IC	const float	CScriptSound::GetFrequency	() const
{
	//VERIFY				(m_sound._handle());
	if (!m_sound._handle())
	{
		Msg(fmt::format("* WARNING: CScriptSound::GetFrequency There is no sound {0}", *m_caSoundToPlay).c_str());
		return 0;
	}
	return				(m_sound.get_params()->freq);
}

IC	const float CScriptSound::GetMinDistance() const
{
	//VERIFY				(m_sound._handle());
	if (!m_sound._handle())
	{
		Msg(fmt::format("* WARNING: CScriptSound::GetMinDistance There is no sound {0}", *m_caSoundToPlay).c_str());
		return 0;
	}
	return				(m_sound.get_params()->min_distance);
}

IC	const float CScriptSound::GetMaxDistance() const
{
	//VERIFY				(m_sound._handle());
	if (!m_sound._handle())
	{
		Msg(fmt::format("* WARNING: CScriptSound::GetMaxDistance There is no sound {0}", *m_caSoundToPlay).c_str());
		return 0;
	}
	return				(m_sound.get_params()->max_distance);
}

IC	const float	CScriptSound::GetVolume		() const
{
	//VERIFY				(m_sound._handle());
	if (!m_sound._handle())
	{
		Msg(fmt::format("* WARNING: CScriptSound::GetVolume There is no sound {0}", *m_caSoundToPlay).c_str());
		return 0;
	}
	return				(m_sound.get_params()->volume);
}

IC	bool CScriptSound::IsPlaying			() const
{

	if (!m_sound._handle())
	{
		Msg(fmt::format("* WARNING: CScriptSound::IsPlaying There is no sound {0}", *m_caSoundToPlay).c_str());
		return false;
	}
	return				(!!m_sound._feedback());
}

IC	void CScriptSound::Stop					()
{
	//VERIFY				(m_sound._handle());
	if (!m_sound._handle())
	{
		return;
	}
	m_sound.stop		();
}

IC	void CScriptSound::StopDeffered			()
{
	//VERIFY				(m_sound._handle());
	if (!m_sound._handle())
	{
		return;
	}
	m_sound.stop_deffered();
}

IC	void CScriptSound::SetPosition			(const Fvector &position)
{
	//VERIFY				(m_sound._handle());
	if (!m_sound._handle())
	{
		Msg(fmt::format("* WARNING: CScriptSound::SetPosition There is no sound {0}", *m_caSoundToPlay).c_str());
		m_sound.set_position(Fvector().set(0,0,0));
		return;
	}
	m_sound.set_position(position);
}

IC	void CScriptSound::SetFrequency			(float frequency)
{
	//VERIFY				(m_sound._handle());
	if (!m_sound._handle())
	{
		Msg(fmt::format("* WARNING: CScriptSound::SetFrequency There is no sound {0}", *m_caSoundToPlay).c_str());
		m_sound.set_frequency(0);
		return;
	}
	m_sound.set_frequency(frequency);
}

IC	void CScriptSound::SetVolume			(float volume)
{
	//VERIFY				(m_sound._handle());
	if (!m_sound._handle())
	{
		Msg(fmt::format("* WARNING: CScriptSound::SetVolume There is no sound {0}", *m_caSoundToPlay).c_str());
		m_sound.set_volume(0);
		return;
	}
	m_sound.set_volume	(volume);
}

IC	const CSound_params *CScriptSound::GetParams	()
{
	VERIFY				(m_sound._handle());
	return				(m_sound.get_params());
}

IC	void CScriptSound::SetParams			(CSound_params *sound_params)
{
	VERIFY				(m_sound._handle());
	m_sound.set_params	(sound_params);
}
