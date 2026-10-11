// defs.h: named constants and macros shared by every module (no code, no variables).
#ifndef INVEN_FILTER_DEFS_H
#define INVEN_FILTER_DEFS_H

// ---- Messages (text\<language>\game\inventory_filter.msg)

#define MSG_SMART_BARTER_PAID   100   // %d caps settled by SmartBarter
#define MSG_CLOSE_CONTAINER     101
#define MSG_SFALL_TOO_OLD       102
#define MSG_SCREEN_TOO_SMALL    103
#define MSG_DISABLED_AT_START   104
#define MSG_ENABLED             105
#define MSG_DISABLED            106
#define MSG_TOO_FAR             107   // appended to a party member's name
#define MSG_SWAP_FILTER_ACTIVE  108
#define MSG_FILTER_TIP          110   // + category index (110..118)
#define MSG_SORT_TIP            119   // + sort index (119..122)
#define MSG_SEARCH_TIP          123
#define MSG_NO_AMMO_WEAPON      124
#define MSG_INTERNAL_ERROR      126   // the script stopped half way
#define MSG_BUILD_FOR_SFALL4    127   // the sFall 4 build runs on sFall 5

// Messages of the game's own inventory.msg (file GAME_MSG_INVENTRY).
#define GAME_MSG_INVENTRY       5
#define GMSG_TOO_HEAVY          26
#define GMSG_TOTAL_WEIGHT       30

// ---- sFall hooks

#define HOOK_REMOVEINVENOBJ     9
#define HOOK_BARTERPRICE        10
#define HOOK_KEYPRESS           19
#define HOOK_MOUSECLICK         20
#define HOOK_INVENTORYMOVE      24
#define HOOK_GAMEMODECHANGE     31
#define HOOK_TOTALWEIGHT        50    // sFall 5 only

// Arguments of the barter price hook.
#define BP_PARTNER              1     // the critter bartered with
#define BP_VALUE                2     // value of the goods requested
#define BP_TRADER_TABLE         3
#define BP_PLAYER_TABLE         6
#define BP_OFFER_VALUE          7     // value of the goods offered
#define BP_OFFER_PRESSED        8     // 1 once the Offer button was pressed
#define BP_PARTY_MEMBER         9     // 1 when trading with a party member

// ---- Game modes (get_game_mode flags)

#define GM_WORLDMAP             0x00000001
#define GM_DIALOG               0x00000004
#define GM_LOADGAME             0x00000020
#define GM_COMBAT               0x00000040
#define GM_INVENTORY            0x00001000
#define GM_INTFACEUSE           0x00008000   // inventory opened to use an item on something
#define GM_INTFACELOOT          0x00010000
#define GM_BARTER               0x00020000
#define GM_COUNTERWIN           0x00100000   // quantity window
// Screens that get the filter windows.
#define GM_FILTER_SCREENS       (GM_INVENTORY bwor GM_INTFACEUSE bwor GM_INTFACELOOT bwor GM_BARTER)
// Screens where the caps are filtered with the other items (they are never filtered in barter).
#define GM_CAPS_SCREENS         (GM_INVENTORY bwor GM_INTFACEUSE bwor GM_INTFACELOOT)

// ---- Game objects

#define INVEN_CMD_INDEX_PTR     13    // inven_cmds: the item at an index of an inventory
#define INVEN_TYPE_INV_COUNT    -2    // critter_inven_obj: number of items in the inventory
#define PID_CAR                 455
#define FID_STRIP_ANIM_MASK     0xFF00FFFF   // clears the animation and weapon codes of a FID
#define WEIGHT_PROBE_BASE       100   // carry weight set to measure the bonus of the perks and traits

#define PID_BOTTLE_CAPS         41
#define PID_TEMP_BOX            467   // object used as a hidden container for the filtered-out items
#define OBJ_TYPE_CRITTER        1
#define ITEM_ARMOR              0
#define ITEM_CONTAINER          1
#define ITEM_DRUG               2
#define ITEM_WEAPON             3
#define ITEM_AMMO               4
#define ITEM_MISC               5
#define ITEM_KEY                6
#define STAT_CARRY_WEIGHT       12
#define TRAIT_OBJECT            1
#define OBJ_TEAM                6     // has_trait(TRAIT_OBJECT, critter, ...): team number
#define CSTATE_DEAD             1     // value of critter_state() for a dead critter
#define OBJ_INVENTORY_WEIGHT    669   // has_trait(TRAIT_OBJECT, obj, ...): weight of the items carried
#define ITEM_FLAGS_EQUIPPED     0x07000000   // object flags: left hand, right hand, worn armor
#define SLOT_ARMOR              0
#define SLOT_RIGHT_HAND         1     // slot "ITEM2"
#define SLOT_LEFT_HAND          2     // slot "ITEM1"
#define WINTYPE_INVENTORY       0     // interface windows (get_window_attribute, interface_overlay)
#define WINTYPE_DIALOG          1
#define WINTYPE_BARTER_OVERLAY  9     // overlay drawn on the barter screen
#define WIN_ATTR_X              1
#define WIN_ATTR_Y              2
#define WIN_ATTR_WIDTH          3
#define WIN_ATTR_HEIGHT         4

// Prototype fields (get_proto_data / proto_data).
#define PROTO_CRITTER_FLAGS     32    // critters: flags
#define CFLG_BARTER             0x00000002   // can trade with
#define CFLG_NOSTEAL            0x00000020   // cannot be stolen from
#define PROTO_WEAPON_RANGE      52
#define PROTO_WEAPON_CALIBER    88
#define PROTO_AMMO_CALIBER      36
#define PROTO_CONTAINER_SIZE    36
#define PDATA_WEAPON_ANIM       7     // proto_data(pid, ...): weapon animation code (low nibble)
#define PDATA_CRITTER_TYPE      11    // non-zero for a critter that cannot hold a bag of items (kept from the original script)
#define PDATA_ITEM_WEIGHT       12
#define PDATA_ITEM_COST         14
#define WEAPON_ANIM_MASK        0x0000000F
// Weapon animation codes: 1..4 melee/thrown, 5 pistol, 6 SMG, 7 rifle, 8 big gun, 9 minigun, 10 launcher.
#define ANIM_FIRST_FIREARM      5
#define ANIM_LAST_LIGHT         7
#define ANIM_FIRST_HEAVY        8
#define ANIM_LAST_HEAVY         10
#define ANIM_LAST_MELEE         4
#define THROWN_MIN_RANGE        2     // melee-class weapon with a longer range counts as thrown (light)

// ---- Engine memory (Fallout 2 v1.02 exe). Reading it is always allowed; writing it and calling the
// engine's functions (the companion's portrait in the swap) need AllowUnsafeScripting=1.

#define ADDR_PC_CURR_STACK      5892460   // 0x59E96C: nesting level of the bag opened in the player's list
#define ADDR_TARGET_CURR_STACK  5892424   // 0x59E948: same for the other inventory
#define ADDR_INVEN_DUDE_FID     5892444   // 0x59E95C: art FID drawn in the inventory window
#define ADDR_DUDE_ROTATION      5345528   // 0x5190F8: body rotation shown in the inventory window
#define ADDR_IS_STEAL           5362736   // 0x51D430: non-zero in the steal screen
#define FN_CONTAINER_EXIT       4678548   // 0x476394 container_exit(keyCode, windowType)
#define FN_DISPLAY_BODY         4654672   // 0x470650 display_body(fid, windowType)
#define KEY_CONTAINER_BACK      2500      // key code container_exit takes to leave a bag
// Engine call sites that report an item removal to the "remove inventory object" hook (the names
// are those of RMOBJ_* in sFall's define_extra.h).
#define RMOBJ_ITEM_MOVE         4683293   // 0x47761D item_move_func: an item goes from one inventory to another
#define RMOBJ_UNLOAD_WEAPON     4667030   // 0x473696 inven_action_cursor: a weapon is unloaded
#define RMOBJ_ITEM_REMOVED_INVEN 4831349  // 0x49B875 obj_remove_from_inven: an item is removed or destroyed
#define INVEN_WINDOW_LOOT       2         // window type of the loot screen; the barter screen is 3

// ---- Filter categories

#define FILTER_COUNT            9
#define FLAG_MELEE              0x00000001
#define FLAG_LIGHT_WEAPON       0x00000002
#define FLAG_HEAVY_WEAPON       0x00000004
#define FLAG_AMMO               0x00000008
#define FLAG_ARMOR              0x00000010
#define FLAG_DRUG               0x00000020
#define FLAG_TOOL               0x00000040
#define FLAG_CUSTOM             0x00000080
#define FLAG_MISC               0x00000100
#define FLAG_AMMO_SEARCH        0x00000200   // not a button: the ammo search of the search button
#define DEFAULT_MAX_ITEMS       1000
#define MAX_ITEMS_LIMIT         9999   // the sort keys leave 4 digits for the position of an item
#define SORT_KEY_SCALE          10000  // sort key = value * SORT_KEY_SCALE + position

// Kinds of the other inventory, each with its own kept filters (KeepLootFilters, ...).
#define TARGET_LOOT             0     // container or body
#define TARGET_COMPANION        1     // party member, looted or bartered with
#define TARGET_MERCHANT         2     // anybody else bartered with
#define TARGET_TYPES            3

// ---- Layout of the filter windows (window coordinates)

#define SIDE_PLAYER             0
#define SIDE_OTHER              1
#define FILTER_WIN_W            50
#define FILTER_WIN_H            376
#define BTN_X                   10
#define BTN_W                   32
#define BTN_H                   20
#define BTN_PITCH               23            // distance between two buttons
#define FILTER_BTN_Y            15            // first filter button
#define FILTER_ROW_Y            14            // row k spans [FILTER_ROW_Y + k * BTN_PITCH, + BTN_PITCH)
#define SORT_BTN_Y              235
#define SORT_COUNT              4
#define SEARCH_BTN_Y            339
#define GROUP_FILTER            0             // button groups, in the order they are stacked in the window
#define GROUP_SORT              1
#define GROUP_SEARCH            2
#define CLICK_MOUSE             0             // how a category button was activated (see FilterClick)
#define CLICK_TOGGLE            1
#define CLICK_KEY               2
#define DIK_LCONTROL            29
#define DROPALL_X               70            // Drop All / Give All button in the inventory window
#define DROPALL_Y               201
#define DROPALL_SIZE            40
#define OVERLAY_TEXT_W          60            // size of the area cleared and centred for a weight or caps text
#define OVERLAY_TEXT_H          24
#define OVERLAY_TEXT_MARGIN     15            // extra width cleared on each side of that area
#define OVERLAY_TEXT_DY         5
#define MSG_WIN_FLAGS           0x20      // transparent window
#define TIP_SHOW_DELAY          600           // ms the mouse must rest on a button before its tip shows
#define TIP_RECHECK_DELAY       200
#define TIP_MOVE_TOLERANCE      3             // px

// ---- Per-side state: one array, FIELDS slots per side (see SideGet / SideSet)

#define SS_FLAGS                0    // active filter bits (FLAG_*)
#define SS_SORT                 1    // active sort mode: 0 none, else sort index + 1
#define SS_SNAPSHOT             2    // array saved by SaveOriginalOrder, 0 if none
#define SS_SEARCH_SLOT          3    // ammo search: weapon slot searched (0 none)
#define SS_FLAGS_BEFORE_SEARCH  4    // filter bits to bring back when the search ends
#define SS_TEMP_BOX             5    // hidden container that holds the filtered-out items
#define SS_BTN_DOWN             6    // array: pressed state of the category buttons
#define SS_X                    7    // x of the filter window (0 for the right one while it does not exist)
#define SS_WEIGHT_X             8    // x of the weight text under the window
#define SIDE_FIELDS             9

#define SideGet(sd, fld)        get_array(sideState, ((sd) * SIDE_FIELDS) + (fld))
#define SideSet(sd, fld, val)   set_array(sideState, ((sd) * SIDE_FIELDS) + (fld), (val))
#define SideWin(sd)             ("inven_filter_right" if (sd) else "inven_filter")
#define MouseSide               (1 if (SideGet(SIDE_OTHER, SS_X) andAlso (get_mouse_x >= SideGet(SIDE_OTHER, SS_X))) else 0)
#define FilterRowAt(y)          (((y) - (menuY + FILTER_ROW_Y)) / BTN_PITCH)
#define SortRowAt(y)            (((y) - (menuY + SORT_BTN_Y)) / BTN_PITCH)

// ---- Fonts, sounds and text

#define FONT_TEXT               101
#define TIP_BORDER_COLOR        74    // palette index of the border of the tooltips
#define TIP_HEIGHT              16
#define FORMAT_CENTRED          2     // flags of format(): centred text
#define SFX_PRESS               "IB2P1XX1"   // button pressed
#define SFX_REFUSED             "IISXXXX1"   // action refused
#define SFX_DROPALL_PRESS       "IB1P1XX1"
#define SFX_DROPALL_RELEASE     "IB1LU1X1"
#define SFX_SWAP_RELEASE        "IB2LU1X1"

#define InvenItemAt(obj, idx)   inven_cmds((obj), INVEN_CMD_INDEX_PTR, (idx))

// ---- Settings file

#define INI_FILE                "mods\\InventoryFilter.dat\\InvenFilter.ini"
#define IniGet(key)             get_ini_setting(INI_FILE + "|InventoryFilter|" + (key))
#define IniGetString(key)       get_ini_string(INI_FILE + "|InventoryFilter|" + (key))
#define IniSet(key, val)        sfall_func2("set_ini_setting", INI_FILE + "|InventoryFilter|" + (key), (val))
#define PCX_DIR                 "pcx\\"
#define LOCALE_PCX_DIR          "pcx_locales\\"   // Drop All / Give All pictures with a text, one folder per language

// ---- Engine state shortcuts

#define ObjWeight(obj)          has_trait(TRAIT_OBJECT, (obj), OBJ_INVENTORY_WEIGHT)
#define CarryWeight(obj)        get_critter_stat((obj), STAT_CARRY_WEIGHT)

// A bag is open in the inventory list of a side: the filters must not be touched then.
#define ContainerOpen(sd)       (read_int(ADDR_TARGET_CURR_STACK if (sd) else ADDR_PC_CURR_STACK) > 0)
#define InvenWindowType         (INVEN_WINDOW_LOOT + pcInBarter)

// ---- Trace (compiled in only with INVDEBUG, see Compile.ps1 -Trace)

#ifdef INVDEBUG
#define Trace(msg)              call DebugTrace(msg);
#else
#define Trace(msg)
#endif

#endif
