# Changelog

## 2.4

Renamed InventoryFilter. Works on sFall 4.2.9+ and sFall 5.0.9+ from one source tree.

### New
- sFall 4 support (separate build, shared sources, `Compat*` layer), with sFall 4 specific click handling, `ExpandInventory` / `ExpandBarter` layouts and a trace build for diagnostics.
- Languages: Czech, German, Hungarian, Italian, Polish, Portuguese, Spanish, Traditional Chinese, Korean and Swedish, using the Restoration Project and UPU terms where they exist.
- Ammo search button: left click = weapon 1, middle click = weapon 2, right click or the same click again = off.
- Drop All and Give All buttons: a text in the language of the game by default (`pcx_locales/<language>`, 10 languages), or language-free icons (a backpack with an arrow, a person with an arrow) with the `IconOnlyButtons` setting; the icons mode also gives the game's TAKE ALL button an icon (`frm/invmaup.frm`, `frm/invmadn.frm`).
- New icons (custom, weapon, search, sort) and `main_left.pcx` / `main_right.pcx` backgrounds.
- `MaxItems`, `SwapMaxDistance` and `Language` settings; `InvenFilter_comments.json` and `TranslateIni.ps1` rewrite the INI comments in any language of the mod.
- Messages for an unsupported sFall (too old, or the sFall 4 build on sFall 5) and for a script that stopped half way.
- Swapping with a party member while a filter is on warns first; a second click turns the filter off and swaps.
- `KeepLootFilters`, `KeepCompanionFilters` and `KeepMerchantFilters` (0 by default) keep the filters of the right inventory, separately for a container or body, a party member and a merchant.
- Items moved into a filtered inventory (slots, loot, barter tables, Drop All) go through the filter again.
  The filters are applied again on the next frame after the move (no polling, no delay), and never while the mouse button is held, so dragging an item with a filter on is safe.

### Changed
- Filtering only moves the items whose state changes, and the original order is restored only when it is not already right.
- `AllowUnsafeScripting` is no longer required: only the party member's portrait and the automatic closing of a bag use it.
- The source was split into modules, renamed to one convention, commented, with named constants instead of engine addresses and magic numbers, and the paired left/right code merged around a per-side state.

### Fixed
- Releasing a filter or a sort restores the original order.
- The hooks 24, 10 and 50 are always unregistered when a screen closes.
- Bottle caps moved by the filter no longer leave a stray object on the map.
- An equipped item is never hidden by a filter.
- The swapped carry weight is forgotten when a screen closes.
- The `Language` setting works (its lookup never matched).
- The swap range test (message "too far") now works through `SwapMaxDistance`.
- Right filter window and swap window creation on sFall 4 (an integer 0 is required where a float was passed).
- Weight and filter positions with `ExpandInventory` / `ExpandBarter` on sFall 4.
- sFall 4: the Drop All / Give All button no longer disappears when the game redraws the loot screen (it is a picture in the inventory window, clicked by position), and the weight text no longer shows twice when the filter changes.
- Take All, then disabling the filter of the container, no longer risks losing items: whatever is left in a temporary container goes back to its owner.
- Audit fixes: the barter screen is known as such when the kept filters are applied; the Drop All hotkey follows the rules of the button (not in the steal screen, not on a living critter of another team); a sort click is refused while a bag is open; filter hotkeys and middle clicks wait for a dragged item to be dropped; a refilter asked while a bag is open waits for it to close; hooks are registered only when the initialization is over; the right filter window is always released, even if its creation stopped half way; no weight text in barter when `ShowWeight=0`.
- Packages: zip entries use `/` whatever PowerShell builds them, and a `-Trace` build is packaged under `_trace.zip`.
