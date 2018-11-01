#if !defined(KEY_MAP_TOOLS_H)
#define KEY_MAP_TOOLS_H

#include "sysstuff.h"

#include "boardio.h"

// TODO: I'm not happy that in order to override "modifier + key" it requires
// that I create an entire layer. I want to be able to make caps+shift+volup
// do something, but not cause the caps+shift thing to be a full layer.
//
// Instead, maybe look at something like the KLL stuff from the InputClub
// a key-action:
// Default action: key, modifier, other
// A key is just what it says.
// A modifier is something like shift/cmd/ctrl/win/alt// other:
//  Trigger, action
//  Trigger: on Key down, or key up
//  Action: keypress (either with exist modifiers, or 'unique')
//          Layer change (latch, lock, or shift)

// Okay, this is just a way to get the 'default' map, which can then be
// modified, "KLL-style"

#if 1

namespace KY {
const uint8_t HM = HOME = 1, PU = PAGEUP = PGUP = 2, PD = PAGEDN = PGDN = 3,
              EN = END = 4, BS = BKSPC = 8, TB = TAB = 9,
              CR = ENTER = RETURN = 13, LS = LSHIFT = 14, RS = RSHIFT = 15,
              LC = LCTRL = 17, RC = RCTRL = 18, LA = LALT = LCMD = 19,
              RA = RALT = RCMD = 20, UP = 21, DN = DOWN = 22, LT = LEFT = 23,
              RT = RIGHT = 24, ES = ESC = 27, CL = CAPS = 29,
              LG = LGUI = LOPT = 30, RG = RGUI = ROPT = 31, DL = DEL = 127,
              _1 = '1', _2 = '2', _3 = '3', _4 = '4', _5 = '5', _6 = '6',
              _7 = '7', _8 = '8', _9 = '9', _0 = '0', MN = MINUS = '-',
              Q_ = 'Q', W_ = 'W', E_ = 'E', R_ = 'R', T_ = 'T', Y_ = 'Y',
              U_ = 'U', I_ = 'I', O_ = 'O', P_ = 'P', PP = PIPE = BKSLSH = '\\',
              A_ = 'A', S_ = 'S', D_ = 'D', F_ = 'F', G_ = 'G', H_ = 'H',
              K_ = 'K', L_ = 'L', SC = SEMI = SCOLOR = ';', QU = QUOTE = '\'',
              Z_ = 'Z', X_ = 'X', C_ = 'C', V_ = 'V', B_ = 'B', N_ = 'N',
              M_ = 'M', CM = COMMA = ',', PR = PERIOD = DOT = '.',
              SL = SLSH = QUESTION = '/', TL = TILDE = BQUOTE = '`',
              SP = SPC = ' ', EQ = EQUAL = '=', OB = OBRACE = '[',
              CB = CBRACE = ']', __ = NIL = 0;
}

// This is the 'base layer' that may or may not be used unmodified
constexpr uint8_t core[BoardIO::nrows][BoardIO::ncols] = {
    // Numbers row:
    {KY::ES, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-'},
    // QWERTY row:
    {KY::TB, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '\\'},
    // ASDF row:
    {KY::CL, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\''},
    // ZXCV row:
    {KY::LS, 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', KY::RS},
    // Modifiers row:
    {KY::LCTRL,
     KY::LOPT,
     KY::LCMD,
     KY::PGUP,
     '`',
     KY::BKSPC,
     ' ',
     '=',
     KY::UP,
     KY::RCMD,
     KY::ROPT,
     KY::RCTRL},
    // Bottom row:
    {KY::__,
     '[',
     KY::HOME,
     KY::PGDN,
     KY::END,
     KY::DEL,
     KY::ENTER,
     KY::LEFT,
     KY::DOWN,
     KY::RIGHT,
     ']',
     KY::__}};

// Actions can be:
// Send a key up, key down, or key stroke, (or modifier)
// Cancel other stuff until back to neutral keyboard
// Layer changes
// A layer-transition can be:
// latch (1-shot layer)
// layer shift
// push-lock
// pop-lock

// key(key_id)
// key_down(key_id)
// key_up(key_id)

// mod(mod_id)
// mod_down(mod_id)
// mod_up(mod_id)

// layer_latch(layer_id)
// layer_shift(layer_id)
// layer_push(layer_id)
// layer_pop()
// layer_swtich(layer_id)  => layer_pop() + layer_push(layer_id)

// cancel()
// All these things can be added together into a list

// A 'layer' is really just a collection of re-maps
// At runtime, a layer also MUST have a parent

// A key can be 'triggered' by:
// "raw" key (only modifiers are 'layer' mods, no other keys pressed)
// "clean" key stroke (key-up occurs after key-down, with no keyup/dn between)
// "modified" key up or down (what it says)
// raw_trigger
// clean_trigger
// down_trigger
// up_trigger

// What I want:
// base_layer[_CAPS_] = action(down_trigger, mod_down(_LCTRL_) +
// layer_push(win_layer)) base_layer[_Q_] = action(raw_trigger, key(_Q_)); <--
// This is the default base_layer[_TAB_] = actdion(raw_trigger, key(_TAB_));
// win_layer[_CAPS_] = action(up_trigger, mod_up(_LCTRL_) +
// layer_pos(win_layer)) win_layer[_Q_] = action(raw_trigger, key(_F4_) +
// mod(_LALT_)) When caps + Q is held, the signals should be thus:
// caps down:
//   ctrl down, layer(win)
// Q down:
//   ctrl up, alt + f4 down
// Q up:
//   alt + f4 up, ctrl down
// caps up
//   ctrl up, layer(base)
// OR
// caps down:
//   ctrl down;
// Q down:
//   ctrl up, alt + f4 down
// caps up:
//   alt + f4 up, layer(base)
// Q up
//   nothing left

#endif

#endif