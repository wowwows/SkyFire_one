/*
 * Copyright (C) 2010-2013 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2010-2013 Oregon <http://www.oregoncore.com/>
 * Copyright (C) 2006-2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
SDName: Boss_Doctor_Theolen_Krastinov
SD%Complete: 100
SDComment:
SDCategory: Scholomance
EndScriptData */

#include "ScriptPCH.h"
#include "scholomance.h"

enum eEnums
{
    EMOTE_GENERIC_FRENZY_KILL   = -1000001,

    SPELL_REND                  = 16509,
    SPELL_BACKHAND              = 18103,
    SPELL_FRENZY                = 8269
};

struct boss_theolenkrastinovAI : public ScriptedAI
{
    boss_theolenkrastinovAI(Creature *c) : ScriptedAI(c) {}

    uint32 m_uiRend_Timer;
    uint32 m_uiBackhand_Timer;
    uint32 m_uiFrenzy_Timer;

    void Reset()
    {
        m_uiRend_Timer = 8000;
        m_uiBackhand_Timer = 9000;
        m_uiFrenzy_Timer = 1000;
    }

    void JustDied(Unit* /*pKiller*/)
    {
        ScriptedInstance* instance = me->GetInstanceScript();
        if (instance)
        {
            instance->SetData(DATA_DOCTORTHEOLENKRASTINOV_DEATH, 0);

            if (instance->GetData(TYPE_GANDLING) == IN_PROGRESS)
                me->SummonCreature(1853, 180.73f, -9.43856f, 75.507f, 1.61399f, TEMPSUMMON_DEAD_DESPAWN, 0);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!UpdateVictim())
            return;

        //Rend_Timer
        if (m_uiRend_Timer <= uiDiff)
        {
            DoCast(me->getVictim(), SPELL_REND);
            m_uiRend_Timer = 10000;
        }
        else
            m_uiRend_Timer -= uiDiff;

        //Backhand_Timer
        if (m_uiBackhand_Timer <= uiDiff)
        {
            DoCast(me->getVictim(), SPELL_BACKHAND);
            m_uiBackhand_Timer = 10000;
        }
        else
            m_uiBackhand_Timer -= uiDiff;

        //Frenzy_Timer
        if (me->GetHealth()*100 / me->GetMaxHealth() < 26)
        {
            if (m_uiFrenzy_Timer <= uiDiff)
            {
                DoCast(me, SPELL_FRENZY);
                DoScriptText(EMOTE_GENERIC_FRENZY_KILL, me);

                m_uiFrenzy_Timer = 120000;
            }
            else
                m_uiFrenzy_Timer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_theolenkrastinov(Creature* creature)
{
    return new boss_theolenkrastinovAI (creature);
}

void AddSC_boss_theolenkrastinov()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_doctor_theolen_krastinov";
    newscript->GetAI = &GetAI_boss_theolenkrastinov;
    newscript->RegisterSelf();
}

