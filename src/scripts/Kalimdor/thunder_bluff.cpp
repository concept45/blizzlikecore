/*
 * BlizzLikeCore Copyright (C) 2013  BlizzLikeGroup
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Thunder_Bluff
Complete(%): 100
Comment: Quest support: 925
Category: Thunder Bluff
EndScriptData */

#include "ScriptPCH.h"

/*#####
# npc_cairne_bloodhoof
######*/

#define SPELL_BERSERKER_CHARGE  16636
#define SPELL_CLEAVE            16044
#define SPELL_MORTAL_STRIKE     16856
#define SPELL_THUNDERCLAP       23931
#define SPELL_UPPERCUT          22916

#define GOSSIP_HCB "I know this is rather silly but a young ward who is a bit shy would like your hoofprint."
//TODO: verify abilities/timers
struct npc_cairne_bloodhoofAI : public ScriptedAI
{
    npc_cairne_bloodhoofAI(Creature* c) : ScriptedAI(c) {}

    uint32 BerserkerCharge_Timer;
    uint32 Cleave_Timer;
    uint32 MortalStrike_Timer;
    uint32 Thunderclap_Timer;
    uint32 Uppercut_Timer;

    void Reset()
    {
        BerserkerCharge_Timer = 30000;
        Cleave_Timer = 5000;
        MortalStrike_Timer = 10000;
        Thunderclap_Timer = 15000;
        Uppercut_Timer = 10000;
    }

    void EnterCombat(Unit* /*who*/) {}

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        if (BerserkerCharge_Timer <= diff)
        {
            Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (pTarget)
                DoCast(pTarget, SPELL_BERSERKER_CHARGE);
            BerserkerCharge_Timer = 25000;
        } else BerserkerCharge_Timer -= diff;

        if (Uppercut_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_UPPERCUT);
            Uppercut_Timer = 20000;
        } else Uppercut_Timer -= diff;

        if (Thunderclap_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_THUNDERCLAP);
            Thunderclap_Timer = 15000;
        } else Thunderclap_Timer -= diff;

        if (MortalStrike_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_MORTAL_STRIKE);
            MortalStrike_Timer = 15000;
        } else MortalStrike_Timer -= diff;

        if (Cleave_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_CLEAVE);
            Cleave_Timer = 7000;
        } else Cleave_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_npc_cairne_bloodhoof(Creature* pCreature)
{
    return new npc_cairne_bloodhoofAI (pCreature);
}

bool GossipHello_npc_cairne_bloodhoof(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetQuestStatus(925) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HCB, GOSSIP_SENDER_MAIN, GOSSIP_SENDER_INFO);

    pPlayer->SEND_GOSSIP_MENU(7013, pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_cairne_bloodhoof(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
{
    if (uiAction == GOSSIP_SENDER_INFO)
    {
        pPlayer->CastSpell(pPlayer, 23123, false);
        pPlayer->SEND_GOSSIP_MENU(7014, pCreature->GetGUID());
    }
    return true;
}

void AddSC_thunder_bluff()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_cairne_bloodhoof";
    newscript->GetAI = &GetAI_npc_cairne_bloodhoof;
    newscript->pGossipHello = &GossipHello_npc_cairne_bloodhoof;
    newscript->pGossipSelect = &GossipSelect_npc_cairne_bloodhoof;
    newscript->RegisterSelf();
}

