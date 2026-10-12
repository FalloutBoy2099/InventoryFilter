// InvenFilter.h: shared declarations (prototypes + global variables).
// Included by gl_InvenFilter.ssl and by every module.
#ifndef INVEN_FILTER_H
#define INVEN_FILTER_H

#include "defs.h"

// --- gl_InvenFilter.ssl
procedure start;
procedure LocaleFolder(variable lang);
procedure ChooseButtonPictures(variable lang);
procedure EndInit;
procedure InitMod;
procedure IniInt(variable key, variable def);
procedure LoadSettings;
procedure AppendPids(variable list, variable text);
procedure BuildTables;
procedure UpdateLoop;
procedure GameModeChangeHook;
procedure CreateInvenFilterLeftMenu(variable looting);
procedure PlaceLeftMenu(variable mode, variable looting);
procedure LayoutWeights(variable mode, variable looting);
procedure BuildLeftWindow(variable menuX);
procedure HookScreen(variable mode, variable looting);
procedure OtherTargetType;
procedure SaveOtherFilters;
procedure CreateInvenFilterRightMenu(variable looting);
procedure DestroySwapIfAny;
procedure DestroyFilterWin;
procedure ReleasePlayerSide;
procedure ReleaseOtherSide;
procedure ReleaseLootTarget(variable box);
procedure ReleaseBarterPartner(variable box);
procedure ResetSideStates;
procedure KeyPressHook;
procedure MouseClickHook;
procedure CreateMessageWin(variable text);
procedure DestroyMessageWin;
procedure UnsignedComp(variable a, variable b);

// --- filter_logic.ssl
procedure GetFilterFlags(variable index);
procedure WeaponAnimCode(variable pid);
procedure IsHeavyWeapon(variable pid);
procedure IsLightWeapon(variable pid);
procedure IsToolGroup(variable pid);
procedure IsCustomGroup(variable pid);
procedure ItemMatchesFlags(variable pid, variable subtype, variable flags);
procedure FilterRemoveItems(variable src, variable dst, variable item, variable pid);
procedure MoveObjectInventory(variable src, variable dst, variable withCaps);
procedure FilterInventorySub(variable flags, variable owner, variable box);
procedure ShowMatchingItems(variable flags, variable box, variable owner);
procedure RestackCaps(variable owner);
procedure FilterInventory;
procedure FilterInventoryRight;
procedure ApplyFilter(variable side);
procedure GetHandWeaponCaliber(variable slot);
procedure AmmoMatchesSearch(variable pid);

// --- ui_buttons.ssl
procedure ButtonDir(variable group);
procedure ButtonKind(variable group, variable index);
procedure ButtonName(variable group, variable index, variable side);
procedure CreateButton(variable group, variable index, variable side);
procedure CreateButtons(variable side);
procedure ShowFilterMessage(variable text);
procedure WrapMessage(variable text);
procedure ContainerRefused;
procedure RestoreSortButtonStates(variable side);
procedure SortButtonPressed;
procedure SearchButtonPressed;
procedure SearchButtonClicked(variable button);
procedure SearchClick(variable button);
procedure SearchReset(variable side);
procedure RestoreFilterStates(variable side);
procedure ReleaseFilterButtons(variable side, variable keep);
procedure FilterButtonPressed;
procedure FilterClickAt(variable side, variable mode);
procedure FilterClick(variable side, variable index, variable mode);
procedure ButtonHOn;
procedure ButtonHOff;
procedure HoverEnter;
procedure HoverLeave;
procedure CheckHoverTime;
procedure HoverProcess;
procedure ShowTipText;

// --- sort_logic.ssl
procedure SortInventory(variable side, variable key, variable redraw);
procedure SortValue(variable value);
procedure SortKeyOf(variable item, variable key, variable count, variable position);
procedure ReAddSorted(variable items, variable total, variable box, variable owner);
procedure GetInventoryOwner(variable side);
procedure SaveOriginalOrder(variable side);
procedure OrderMatchesSnapshot(variable owner, variable snapshot);
procedure RestoreOriginalOrder(variable side, variable redraw);
procedure FreeOriginalOrder(variable side);
procedure ApplyInventoryOrder(variable side, variable redraw);

// --- hotkeys.ssl
procedure HotKeyValue(variable keys, variable name);
procedure LoadHotKeys;
procedure GetHotKeyIndex(variable side, variable key);
procedure FilterByKey(variable side, variable index);

// --- barter_loot.ssl
procedure CheckLootObject(variable target);
procedure GetAndSaveDudeWeight;
procedure GetDudeWeightBonus;
procedure EquippedWeight;
procedure RefreshPcWeight;
procedure WeightWin(variable weight, variable mode);
procedure WeightLootWin(variable target);
procedure BarterWeightWin(variable extra);
procedure PartyWeightWin(variable extra);
procedure PrintWeight(variable value, variable maxValue, variable x, variable winType);
procedure TraderCostWin;
procedure ClearTextArea(variable winType, variable x, variable y);
procedure PrintText(variable text, variable x, variable y, variable winType);
procedure TotalWeightHook;
procedure CursorBusy;
procedure ScheduleRefilter;
procedure RunPendingRefilter;
procedure ReapplyFilters;
procedure MoveItemPCHook;
procedure MoveItemHook;
procedure BarterHook;
procedure DropAllUsable;
procedure CreateDropAllBtn;
procedure CheckDudeEquipItems(variable full);
procedure KeyDropPress;
procedure KeyDropEnter;
procedure KeyDropLeave;
procedure KeyDropRelease;

// --- party_swap.ssl
procedure GetPartyArray(variable partner);
procedure BarterSwapWin;
procedure LootSwapWin;
procedure CreateSwapWin(variable offsetX, variable offsetY);
procedure SwapName(variable name);
procedure ReleasePlayerFilters;
procedure SwapPress;
procedure SwapButtonNext;
procedure SwapButtonBack;
procedure SwapBtnLeave;
procedure SwapButtonPressed(variable next);
procedure NextSwapPtr;
procedure BackSwapPtr;
procedure SwappingParty(variable refresh, variable direction);
procedure SwapClearOfferTable;
procedure SwapCloseBags;
procedure SwapSkipMembers(variable direction);
procedure SwapMoveInventories(variable member);
procedure SwapRefreshUi(variable memberName);
procedure SwapShowPortrait(variable direction);
procedure SwapTooFar(variable member);
procedure SetDudeSwapWeight(variable member);
procedure DestroySwap;

// --- compat_sfall4.ssl / compat_sfall5.ssl (same procedures in both)
procedure CompatEnableFileOverride;
procedure CompatCarriedWeight(variable obj);
procedure CompatGameLanguage;
procedure CompatSetFloatPrecision(variable digits);
procedure CompatInventoryExtraHeight;
procedure CompatTwoColumnInventory;
procedure CompatBarterExtraHeight;
procedure CompatInitButton(variable btn);
procedure CompatAddButton(variable btn, variable x, variable y, variable w, variable h);
procedure CompatAddButtonGfx(variable btn, variable picOn, variable picOff, variable picHover);
procedure CompatButtonSelected(variable btn);
procedure CompatSetButtonPressed(variable btn, variable pressed);
procedure CompatMouseClick(variable eventType, variable button);
procedure CompatCreateDropAllBtn(variable giveAll);
procedure CompatDrawDropAllBtn(variable pressed);
procedure CompatDropAllMouse(variable eventType, variable button);
procedure CompatFreeDropAllBtn;
procedure CompatOverlayCreate(variable winType);
procedure CompatOverlayDestroy(variable winType, variable id);
procedure CompatOverlayClear(variable winType, variable id, variable x, variable y);
procedure CompatPrintText(variable winType, variable text, variable x, variable y, variable textX);
procedure CompatFreeTextWins;
procedure CompatDrawTipBorder(variable width);
procedure CompatHookReturnCount;
procedure CompatHookReturn(variable index);
procedure CompatRegisterBarterHook;
procedure CompatRegisterWeightHook;
procedure CompatUnregisterWeightHook;

#ifdef INVDEBUG
procedure DebugTrace(variable msg);
#endif

// --- global variables
// --- configuration (InvenFilter.ini)
variable filterMsgFile;
variable maxItems := DEFAULT_MAX_ITEMS;
variable smartBarter;
variable showWeight;
variable showCaps;
variable showPercentSize := 1;
variable dropAllButton;
variable giveAllImage;
variable iconOnlyButtons;
variable buttonPicDir;
variable useBackImage;
variable dropKeyCode;
variable keepFilters;
variable keepOtherFilters;
variable savedOtherFlags;
variable otherKind;
variable swapEnabled;
variable unsafeAllowed;
variable swapAllContainers;
variable swapLootOffset;
variable swapMaxDistance;
variable swapBarterOffset;
variable barterOutsideInterface := 1;
variable weightWinYShift := 318;
variable textRed := 0.00000;
variable textGreen := 0.90000;
variable textBlue := 0.20000;
variable enableState := -1;
variable hotKeyCodes;
variable npcHotkeyPrefix;
variable filterKinds;
variable sortKinds;
variable toolGroupPids;
variable customGroupPids;

// --- startup and screen state
variable sfallVersion;
variable initDone;
variable screenStage;
variable isCreated;
variable filterRightMenu;
variable filterMessageWin;
variable sideState;

// --- layout
variable interfaceX;
variable interfaceY;
variable menuY;
variable weightWinY;
variable barterExtraHeight;
variable swapWinX;
variable swapWinY;

// --- filtering
variable filterDone := 1;
variable pcFilterIsActive;
variable searchCaliber;
variable npcHotkeyArmed;

// --- hover and tooltips
variable hoverButton;
variable enterHover;
variable enterMouseX;
variable enterMouseY := -1;
variable tipShown;
variable showDelayTime;

// --- loot and barter
variable dialogCritter;
variable pcInBarter;
variable isPartyBarter;
variable lootObject;
variable lootFilter;
variable lootIsContainer;
variable lootIsFriendly;
variable overlayId;
variable lootWeightPending;
variable lootWeightShown;
variable partyWeightShown;
variable capsShown;
variable refreshWeight;
variable refilterPending;
variable dropArtName;
variable dropArtDown;
variable traceCount;
variable costRefresh;
variable pcBarterTable;
variable traderBarterTable;
variable dropButtonWin;
variable dropPress;

// --- carry weight
variable dudeBaseMaxWeight;
variable dudeExtraMaxWeight;
variable dudeEquipItemsWeight;
variable dudeWeightBonus;
variable swapMaxWeight := -1;
variable partyMaxWeight;
variable partyExtraWeightStat := -1;
variable npcMaxWeight;
variable npcExtraWeightStat := -1;
variable containerMaxSize := -1;

// --- party swap
variable swapPartyArray;
variable partyArrayLen;
variable currentSwapNumber := -1;
variable swapPtr;
variable swapContainer;
variable swapPressState;
variable swapWarned;
variable swapReadyAt;
variable moveAll;

#endif
