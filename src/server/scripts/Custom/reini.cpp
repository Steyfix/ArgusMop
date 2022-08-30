`#include "Define.h"
#include "Chat.h"
#include "GossipDef.h"
#include "Item.h"
#include "Language.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "Chat.h"
#include "DB2Stores.h"
#include "Group.h"
#include "InstanceSaveMgr.h"
#include "InstanceScript.h"
#include "Language.h"
#include "MapManager.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "WorldSession.h"
#include "Creature.h"
#include "Map.h"
class Reini_NPC : public CreatureScript
{
public:
    Reini_NPC() : CreatureScript("Reini_NPC")
    {
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        player->PlayerTalkClass->ClearMenus();
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Reinitialiser les instances", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        if (!player)
            return true;

        if (action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            uint16 counter = 0;
            uint16 MapId = 0;
            int8 diff = -1;
            
            for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
            {
                Player::BoundInstancesMap& binds = player->GetBoundInstances(Difficulty(i));
                for (Player::BoundInstancesMap::iterator itr = binds.begin(); itr != binds.end();)
                {
                    InstanceSave* save = itr->second.save;
                    if (itr->first != player->GetMapId() && (!MapId || MapId == itr->first) && (diff == -1 || diff == save->GetDifficulty()))
                    {
                        player->UnbindInstance(itr, Difficulty(i));
                        counter++;
                    }
                    else
                        ++itr;
                }
            }
            creature->MonsterWhisper("Les instances sont reset !", player, false);
        }

        player->CLOSE_GOSSIP_MENU(player);
        return true;
    }
};

void AddSC_Reini_NPC()
{
    new Reini_NPC();
}`